# Tools

This directory contains helper scripts to watermark images and set copyright metadata.

## Overview

- [01_apply_exif_copyright.sh](01_apply_exif_copyright.sh)
  - Updates EXIF/IPTC/XMP copyright-related fields.
  - Sets copyright to `Copyright 2026 Markus Werle`.
  - Sets creator/artist fields to `Markus Werle`.

- [02_apply_invisible_watermark.sh](02_apply_invisible_watermark.sh)
  - Embeds an invisible steganographic watermark payload into images using ImageMagick.
  - Payload text is `Copyright 2026 Markus Werle`.

- [publish_to_website.sh](publish_to_website.sh)
  - Builds the Slidev deck in the project container and publishes it to the `gh-pages` branch.
  - The website includes that branch as a submodule at `talks/capture-this`.

## Recommended Order

1. Run invisible watermark script first.
2. Run EXIF metadata script second.

This keeps metadata updates as the final write step.

## Supported Image Formats

Both scripts process these extensions recursively in a target directory:

- `.jpg`
- `.jpeg`
- `.png`
- `.tif`
- `.tiff`
- `.webp`

Default target directory:

- `talk/public/images`

## Script 1: EXIF/IPTC/XMP Copyright Metadata

File: [01_apply_exif_copyright.sh](01_apply_exif_copyright.sh)

### Prerequisites

- `exiftool`

Install on Debian/Ubuntu:

```bash
sudo apt-get update && sudo apt-get install -y libimage-exiftool-perl
```

### Usage

From repository root:

```bash
./tools/01_apply_exif_copyright.sh
```

Custom target directory:

```bash
./tools/01_apply_exif_copyright.sh path/to/images
```

Auto-install mode (when available):

```bash
./tools/01_apply_exif_copyright.sh --install
```

### Fields Written

- `Copyright`
- `Artist`
- `Creator`
- `Rights`
- `IPTC:CopyrightNotice`
- `XMP-dc:Creator`
- `XMP-dc:Rights`

### Verify Metadata

```bash
exiftool -Copyright -Artist -Creator -Rights -IPTC:CopyrightNotice -XMP-dc:Creator -XMP-dc:Rights talk/public/images/your-file.jpg
```

## Script 2: Invisible Watermark Embedding

File: [02_apply_invisible_watermark.sh](02_apply_invisible_watermark.sh)

### Prerequisites

- `magick` (ImageMagick)
- `composite` (ImageMagick tools)

### Usage

From repository root:

```bash
./tools/02_apply_invisible_watermark.sh
```

Custom target directory:

```bash
./tools/02_apply_invisible_watermark.sh path/to/images
```

Custom steganography offset:

```bash
./tools/02_apply_invisible_watermark.sh path/to/images +17+29
```

## End-to-End Example

```bash
./tools/02_apply_invisible_watermark.sh talk/public/images +17+29
./tools/01_apply_exif_copyright.sh talk/public/images
```

## Notes

- These scripts modify files in place.
- Run them on a clean git working tree if you want to review diffs afterward.
- For JPEG files, additional re-compression after watermarking can reduce hidden payload recoverability.

## Script 3: Publish the Deck to the Website

File: [publish_to_website.sh](publish_to_website.sh)

Builds the slides with Slidev inside the project container and pushes the result
to the `gh-pages` branch of this repository. That branch holds only the built
deck (plus a `.nojekyll` file) and is consumed by
[daixtrose.github.io](https://github.com/daixtrose/daixtrose.github.io) as a git
submodule at `talks/capture-this`, so the deck is served from
`https://www.daixtrose.de/talks/capture-this/`.

The `main` branch never contains build output; `talk/dist` stays ignored.

### Prerequisites

- Docker with the `capture-this-gcc15` image built (see the top-level README)
- Push access to `origin`
- A clean working tree (the script refuses to publish uncommitted state)
- `routerMode: hash` in the front matter of `talk/slides.md`, so that reloads
  and deep links work on GitHub Pages, which has no SPA fallback below the site root

### Usage

From repository root:

```bash
./tools/publish_to_website.sh
```

Overridable via environment variables:

| Variable         | Default                | Meaning                                  |
| ---------------- | ---------------------- | ---------------------------------------- |
| `PUBLISH_BRANCH` | `gh-pages`             | Branch that receives the built deck      |
| `BASE_PATH`      | `/talks/capture-this/` | Public path the deck is served from      |
| `IMAGE`          | `capture-this-gcc15`   | Container image used for the build       |

### What It Does

1. Builds `talk/dist` with `pnpm slidev build --base "$BASE_PATH"`, running the
   container as the current user so no root-owned files are left behind.
2. Clones the publish branch shallowly into a temporary directory under `/tmp`.
   On the very first run the branch does not exist yet and is created as an
   orphan branch.
3. Replaces the branch content with the fresh build, adds `.nojekyll`.
4. Commits as `publish <short sha of main>` and pushes. If nothing changed,
   it exits without committing.

### First Run

GitHub may auto-enable GitHub Pages for this repository when a `gh-pages`
branch appears. Check *Settings → Pages* afterwards and disable it; the deck is
meant to be served through the website repository only.

Then, once, in the website repository:

```bash
git submodule add -b gh-pages https://github.com/daixtrose/capture-this talks/capture-this
```

and add a tile to the "C++ Corner" section of `index.html` with
`url: "/talks/capture-this/"`.

### After Every Publish

In the website repository:

```bash
git submodule update --remote --merge
git commit -am "update capture-this talk"
git push
```
