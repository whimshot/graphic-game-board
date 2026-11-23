# graphic-game-board

A C-based graphical game board library and application built with the Raspberry Pi Pico SDK for the [ClockworkPi PicoCalc](https://github.com/clockworkpi/PicoCalc).

> **Note:** This README was generated from the repository's top-level directory structure and dependencies. Please see [graphic-game-board.md](graphic-game-board.md) for specifics on the API.

## Overview

`graphic-game-board` provides core rendering and game board management primitives designed for embedded display hardware. It integrates hardware drivers and third-party shell utilities (`picocalc-shell`) to run natively on Raspberry Pi Pico / RP2040 / RP2350-based handhelds.

## Repository Structure

```text
├── .vscode/                  # VS Code workspace and build configurations
├── docs/                     # Project documentation and notes
├── drivers/                  # Hardware and display drivers
├── third_party/
│   └── picocalc-shell/       # Submodule/dependency for PicoCalc shell integration
├── CMakeLists.txt            # CMake build configuration
├── graphic-game-board.c      # Core implementation source file
├── graphic-game-board.h      # Public header and API declarations
└── pico_sdk_import.cmake     # Raspberry Pi Pico SDK CMake helper
```

## Prerequisites

To build this project from source, ensure you have the following installed:

- [Raspberry Pi Pico SDK](https://github.com) (`PICO_SDK_PATH` environment variable configured)
- **CMake** (version 3.13 or higher)
- **ARM GCC Toolchain** (`arm-none-eabi-gcc`)
- **Git** (with submodule support)

## Getting Started

### 1. Clone the Repository

Clone the repository along with its `third_party/picocalc-shell` dependency:

```bash
git clone --recursive https://github.com
cd graphic-game-board
```

*(If you already cloned without `--recursive`, run `git submodule update --init --recursive`.)*

### 2. Build the Project

Create a build directory and compile the firmware using CMake:

```bash
mkdir build
cd build
cmake ..
make -j4
```

*(Note: Depending on your target hardware—such as RP2040 vs. RP2350—pass `-DPICO_BOARD=<board_name>` to `cmake` as needed.)*

### 3. Flash to Device

1. Hold the **BOOTSEL** button on your Raspberry Pi Pico / PicoCalc while connecting it to your computer via USB.
2. Copy the generated `.uf2` binary from the `build/` directory onto the mounted `RPI-RP2` (or `RP2350`) mass storage drive.
3. Alternatively, if using a bootloader such as [UF2 Loader](https://github.com/clockworkpi/PicoCalc/releases), copy the `.uf2` file to the appropriate apps directory on your SD card.

## Usage

Include `graphic-game-board.h` in your C/C++ project to access the board rendering and state functions:

```c
#include "graphic-game-board.h"

int main(void) {
    // Initialize hardware, drivers, and game board
    // Render board state and handle input loop
    return 0;
}
```

## Dependencies

- **Raspberry Pi Pico SDK** – Core hardware abstraction and runtime.
- **[picocalc-shell](https://github.com/BlairLeduc/picocalc-shell)** – Shell environment and utilities for the PicoCalc.

## License

*Specify your project license here (e.g., MIT, BSD-3-Clause, GPL-3.0).*
