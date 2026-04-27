# Capture `*this`

A talk about the evolution of capturing `this` in C++ lambda expressions — from C++11 through C++26.

Presented at [C++ User Group Aachen](https://www.meetup.com/c-user-group-aachen/).

Built with [Slidev](https://sli.dev/) and compiled with GCC 15 inside a container.

## Prerequisites

- Docker (or Podman)

Everything else (GCC 15, Node.js 24, pnpm, CMake) is inside the container.

## Quick Start

```bash
# Build the container image
docker build -t capture-this-gcc15 -f container/Containerfile .

# Start the slide presentation
docker run --rm -p 3030:3030 \
    -v "$(pwd)":/workspace \
    -w /workspace/talk \
    -e CI=true \
    capture-this-gcc15 \
    bash -c "pnpm install && pnpm slidev --remote --port 3030"
```

Open **http://localhost:3030** in your browser.

No `--privileged` flag is needed — standard port mapping is sufficient. The `--remote` flag makes Slidev bind to `0.0.0.0` so it's reachable from outside the container.

## C++ Examples

Compile all examples across C++11/14/17/20:

```bash
cd src
bash build.sh          # Build all examples with all standards
bash build.sh 17       # Build all examples with C++17 only
bash build.sh 17 02_star_this  # Build one example with one standard
bash build.sh container        # Only rebuild the container image
```

## VS Code Dev Container

A [Dev Container](.devcontainer/devcontainer.json) configuration is included. With the [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension installed:

1. **Ctrl+Shift+P** → *Dev Containers: Reopen in Container*
2. The container builds automatically, installs dependencies, and forwards port 3030
3. Run `cd talk && pnpm slidev --remote` to start the presentation

The Dev Container includes the **Slidev** and **C++ Tools** extensions. The Slidev extension's slide preview, navigation, and presenter mode all work natively inside the container — no additional configuration needed.

To return to your local environment: **Ctrl+Shift+P** → *Dev Containers: Reopen Folder Locally*.

## Container Contents

| Component | Version |
| --------- | ------- |
| GCC       | 15.2.0  |
| CMake     | 3.31.6  |
| Node.js   | 24.14.1 |
| pnpm      | 10.33   |

## Project Structure

```
├── container/Containerfile       # GCC 15 + Node.js 24 + pnpm
├── .devcontainer/                # VS Code Dev Container config
├── talk/
│   ├── slides.md                 # Presentation source
│   ├── daixtrose-basic/          # Custom Slidev theme
│   │   ├── slide-bottom.vue      # Universal background + brightness detection
│   │   ├── layouts/              # Simplified layout wrappers
│   │   └── styles/               # Font faces, size overrides, color adaptation
│   └── public/
│       ├── fonts/                # WOFF2 fonts (offline, no CDN)
│       └── images/               # Background images
├── src/
│   ├── src/                      # C++ example files (01–06)
│   └── build.sh                  # Container-based compilation script
└── logs/                         # Session logs
```
