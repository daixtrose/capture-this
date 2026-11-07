#!/usr/bin/env bash
set -euo pipefail

TARGET_DIR="${1:-talk/public/images}"
COPYRIGHT_TEXT="Copyright 2026 Markus Werle"
CREATOR_NAME="Markus Werle"

if [[ "${2:-}" == "--install" || "${1:-}" == "--install" ]]; then
  if command -v apt-get >/dev/null 2>&1; then
    echo "Installing exiftool via apt-get (package: libimage-exiftool-perl)..."
    sudo apt-get update
    sudo apt-get install -y libimage-exiftool-perl
  else
    echo "ERROR: --install requested, but apt-get is not available."
    exit 1
  fi
fi

if ! command -v exiftool >/dev/null 2>&1; then
  cat <<'MSG'
ERROR: exiftool is not installed.
Install it with:
  sudo apt-get update && sudo apt-get install -y libimage-exiftool-perl
Then re-run this script.
MSG
  exit 1
fi

if [[ ! -d "$TARGET_DIR" ]]; then
  echo "ERROR: target directory does not exist: $TARGET_DIR"
  exit 1
fi

echo "Updating metadata under: $TARGET_DIR"
echo "Copyright: $COPYRIGHT_TEXT"
echo "Creator:   $CREATOR_NAME"

# Apply both EXIF/IPTC/XMP-style copyright-related fields where supported.
exiftool \
  -overwrite_original \
  -P \
  -m \
  -r \
  -ext jpg -ext jpeg -ext png -ext tif -ext tiff -ext webp \
  "-Copyright=$COPYRIGHT_TEXT" \
  "-Artist=$CREATOR_NAME" \
  "-Creator=$CREATOR_NAME" \
  "-Rights=$COPYRIGHT_TEXT" \
  "-IPTC:CopyrightNotice=$COPYRIGHT_TEXT" \
  "-XMP-dc:Creator=$CREATOR_NAME" \
  "-XMP-dc:Rights=$COPYRIGHT_TEXT" \
  "$TARGET_DIR"

echo
echo "Done. Example verification command:"
echo "  exiftool -Copyright -Artist -Creator -Rights -IPTC:CopyrightNotice -XMP-dc:Creator -XMP-dc:Rights <image-file>"
