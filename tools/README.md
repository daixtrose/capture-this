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
