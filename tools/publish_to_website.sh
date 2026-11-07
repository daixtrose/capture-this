#!/usr/bin/env bash
# Publish the built Slidev deck to the gh-pages branch of this repository.
#
# The website (daixtrose.github.io) includes that branch as a git submodule at
# talks/capture-this, so the deck is served from https://www.daixtrose.de/talks/capture-this/.
# The main branch stays free of build output; talk/dist remains ignored.
set -euo pipefail

PUBLISH_BRANCH="${PUBLISH_BRANCH:-gh-pages}"
BASE_PATH="${BASE_PATH:-/talks/capture-this/}"
IMAGE="${IMAGE:-capture-this-gcc15}"

repo_root="$(git rev-parse --show-toplevel)"
cd "$repo_root"

if [[ -n "$(git status --porcelain)" ]]; then
  echo "ERROR: working tree is not clean. Commit or stash first, so the publish matches a commit." >&2
  exit 1
fi

source_commit="$(git rev-parse --short HEAD)"
remote_url="$(git remote get-url origin)"

echo "Building deck from $source_commit with base path $BASE_PATH ..."
docker run --rm --user "$(id -u):$(id -g)" -e HOME=/tmp -e CI=true \
  -v "$repo_root":/workspace -w /workspace/talk "$IMAGE" \
  bash -c "pnpm install && pnpm slidev build --base '$BASE_PATH'"

if [[ ! -f talk/dist/index.html ]]; then
  echo "ERROR: build produced no talk/dist/index.html" >&2
  exit 1
fi

tmp_dir="$(mktemp -d /tmp/capture-this-publish.XXXXXX)"
trap 'rm -rf "$tmp_dir"' EXIT

first_publish=0
if git ls-remote --exit-code --heads origin "$PUBLISH_BRANCH" >/dev/null 2>&1; then
  echo "Cloning existing branch $PUBLISH_BRANCH ..."
  git clone --quiet --depth 1 --branch "$PUBLISH_BRANCH" --single-branch "$remote_url" "$tmp_dir"
else
  echo "Branch $PUBLISH_BRANCH does not exist yet, creating it as an orphan branch ..."
  git init --quiet -b "$PUBLISH_BRANCH" "$tmp_dir"
  git -C "$tmp_dir" remote add origin "$remote_url"
  first_publish=1
fi

# Replace the branch content with the fresh build.
find "$tmp_dir" -mindepth 1 -maxdepth 1 ! -name .git -exec rm -rf {} +
cp -a talk/dist/. "$tmp_dir"/
touch "$tmp_dir/.nojekyll"

git -C "$tmp_dir" add -A
if git -C "$tmp_dir" diff --cached --quiet; then
  echo "Nothing changed since the last publish."
  exit 0
fi

git -C "$tmp_dir" commit --quiet \
  -m "publish $source_commit" \
  -m "Built from $remote_url commit $source_commit with base path $BASE_PATH."
git -C "$tmp_dir" push --quiet -u origin "$PUBLISH_BRANCH"
echo "Published $source_commit to branch $PUBLISH_BRANCH."

if (( first_publish )); then
  cat <<MSG

First publish of '$PUBLISH_BRANCH'. GitHub may have auto-enabled GitHub Pages
for this repository from that branch. Check Settings -> Pages of the repository
(or run: gh api repos/<owner>/<repo>/pages) and disable it if so.

Then add the submodule once in the website repository:
  git submodule add -b $PUBLISH_BRANCH $remote_url talks/capture-this
MSG
fi

echo
echo "Next step in the website repository:"
echo "  git submodule update --remote --merge && git commit -am 'update capture-this talk'"
