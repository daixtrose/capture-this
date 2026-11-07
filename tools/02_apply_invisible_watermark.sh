#!/usr/bin/env bash
set -euo pipefail

TARGET_DIR="${1:-talk/public/images}"
OFFSET="${2:-+17+29}"
WATERMARK_TEXT="Copyright 2026 Markus Werle"

if [[ ! -d "$TARGET_DIR" ]]; then
  echo "ERROR: target directory does not exist: $TARGET_DIR"
  exit 1
fi

if ! command -v magick >/dev/null 2>&1; then
  echo "ERROR: 'magick' is required (ImageMagick)."
  exit 1
fi

if ! command -v composite >/dev/null 2>&1; then
  echo "ERROR: 'composite' is required (ImageMagick legacy tools)."
  exit 1
fi

tmp_dir="$(mktemp -d)"
trap 'rm -rf "$tmp_dir"' EXIT

wm_image="$tmp_dir/watermark_payload.png"
magick \
  -background black \
  -fill white \
  -font DejaVu-Sans \
  -pointsize 26 \
  label:"$WATERMARK_TEXT" \
  "$wm_image"

mapfile -t files < <(find "$TARGET_DIR" -type f \( \
  -iname '*.jpg' -o -iname '*.jpeg' -o -iname '*.png' -o \
  -iname '*.tif' -o -iname '*.tiff' -o -iname '*.webp' \
\) | sort)

if [[ ${#files[@]} -eq 0 ]]; then
  echo "No supported image files found in: $TARGET_DIR"
  exit 0
fi

echo "Embedding invisible watermark into ${#files[@]} files"
echo "Target: $TARGET_DIR"
echo "Offset: $OFFSET"

done_count=0
for file in "${files[@]}"; do
  out="$tmp_dir/$(basename "$file")"

  # Steganographic embedding. The output file keeps the original file extension.
  composite -stegano "$OFFSET" "$wm_image" "$file" "$out"
  mv "$out" "$file"

  done_count=$((done_count + 1))
  echo "[$done_count/${#files[@]}] watermarked: $file"
done

echo
echo "Done."
echo "Note: for JPEG files, re-compression can weaken hidden payload recovery; apply this as a final step on publish-ready assets."
