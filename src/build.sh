#!/usr/bin/env bash
set -euo pipefail

# Build and run C++ examples using g++-15 from a container.
#
# Usage:
#   ./build.sh                  # Build all examples with all standards
#   ./build.sh 17               # Build all examples with C++17 only
#   ./build.sh 17 02_star_this  # Build one example with C++17
#   ./build.sh container        # Only build the container image

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
IMAGE_NAME="capture-this-gcc15"

STANDARDS=(11 14 17 20)
EXAMPLES=(
    01_this_pointer
    02_star_this
    03_init_capture
    04_implicit_this
    05_deprecated_eq
    06_dangling
)

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Detect container runtime
if command -v docker &>/dev/null; then
    RUNTIME=docker
elif command -v podman &>/dev/null; then
    RUNTIME=podman
else
    echo "Error: neither docker nor podman found" >&2
    exit 1
fi

build_container() {
    echo -e "${CYAN}Building container image ${IMAGE_NAME}...${NC}"
    ${RUNTIME} build -t "${IMAGE_NAME}" -f "${PROJECT_ROOT}/container/Containerfile" "${PROJECT_ROOT}"
    echo -e "${GREEN}Container image ready.${NC}"
}

compile_one() {
    local std="$1"
    local example="$2"
    local src="/workspace/src/src/${example}.cpp"

    echo -en "  ${CYAN}C++${std}${NC} ${example} ... "

    local output
    if output=$(${RUNTIME} run --rm \
        -v "${PROJECT_ROOT}/src:/workspace/src:ro" \
        "${IMAGE_NAME}" \
        g++ -std=c++${std} -Wall -Wextra -Wpedantic \
        -o /dev/null \
        "${src}" 2>&1); then
        echo -e "${GREEN}OK${NC}"
        if [[ -n "$output" ]]; then
            echo -e "    ${YELLOW}warnings:${NC}"
            echo "$output" | sed 's/^/    /'
        fi
        return 0
    else
        echo -e "${RED}FAIL${NC}"
        echo "$output" | sed 's/^/    /'
        return 1
    fi
}

# --- Main ---

# Ensure image exists
if ! ${RUNTIME} image inspect "${IMAGE_NAME}" &>/dev/null; then
    build_container
fi

# Parse arguments
if [[ "${1:-}" == "container" ]]; then
    build_container
    exit 0
fi

FILTER_STD="${1:-}"
FILTER_EXAMPLE="${2:-}"

if [[ -n "$FILTER_STD" ]]; then
    STANDARDS=("$FILTER_STD")
fi

if [[ -n "$FILTER_EXAMPLE" ]]; then
    EXAMPLES=("$FILTER_EXAMPLE")
fi

echo ""
echo -e "${CYAN}Compiling examples with g++-15 (GCC 15.2, container)${NC}"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

ok=0
fail=0

for example in "${EXAMPLES[@]}"; do
    echo ""
    echo -e "${YELLOW}${example}:${NC}"
    for std in "${STANDARDS[@]}"; do
        if compile_one "$std" "$example"; then
            ok=$((ok + 1))
        else
            fail=$((fail + 1))
        fi
    done
done

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo -e "Results: ${GREEN}${ok} passed${NC}, ${RED}${fail} failed${NC}"
