# Lpf2 MicroPython bindings

MicroPython C++ user module exposing the [`Lpf2`](https://github.com/Rbel12b/Lpf2) protocol library.

Provides the `lpf2` module: local + virtual ports, hub emulation, device descriptors, generic device subclassing, and built-in device wrappers (basic motor, encoder motor, color sensor, distance sensor).

## Requirements

- MicroPython source tree (ESP-IDF port), cloned as a sibling
- The [`Lpf2`](https://github.com/Rbel12b/Lpf2) C++ library, cloned as a sibling
- ESP-IDF 5.5.x

## Use as a Git submodule

```bash
git submodule add https://github.com/Rbel12b/Lpf2-micropython-bindings.git modules/Lpf2
git submodule update --init --recursive
```

## Example project structure

```text
my-hub-firmware/
├── lib/
│   └── Lpf2/                 # submodule: github.com/Rbel12b/Lpf2
├── micropython/              # submodule: upstream MicroPython
└── modules/
    ├── micropython.cmake     # CMake entrypoint for all of the user modules
    └── Lpf2/                 # submodule: this repo
        ├── micropython.cmake
        ├── micropython.mk
        ├── mod_*.cpp
        └── stubs/lpf2/       # type stubs for editors
```

`modules/micropython.cmake` is a simple CMake file that includes all user modules. It can be used as-is or modified to include additional modules.

```cmake
# This top-level micropython.cmake is responsible for listing
# the individual modules we want to include.
# Paths are absolute, and ${CMAKE_CURRENT_LIST_DIR} can be
# used to prefix subdirectories.

include(${CMAKE_CURRENT_LIST_DIR}/Lpf2/micropython.cmake)
```

## Build

Activate ESP-IDF, set target, then from `micropython/ports/esp32/`:

```bash
# source ~/.espressif/tools/activate_idf_v5.5.4.sh
# run this from an ESP-IDF shell.
export IDF_TARGET=esp32s3

make USER_C_MODULES=../../../modules/micropython.cmake          # build firmware
make USER_C_MODULES=../../../modules/micropython.cmake deploy   # flash to device
make USER_C_MODULES=../../../modules/micropython.cmake monitor  # serial monitor
make USER_C_MODULES=../../../modules/micropython.cmake clean    # full clean
```

Submodules must be initialised first:

```bash
git submodule update --init --recursive
```

## Python API surface

```text
lpf2
├── port, mode, version, device_descriptor, hub_emulation, device_manager
├── hub_type, device_type, alerts, battery_type, button_state, color, port_num
├── local.port          — direct UART port (Lpf2::Local::Port)
├── virtual.port        — virtual port (Lpf2::Virtual::Port)
├── virtual.device      — subclassable generic device
└── devices.{basic_motor, encoder_motor, color_sensor, distance_sensor, port_expander}
```

## Type stubs

`stubs/lpf2/` ships `.pyi` files covering the full Python API. Point Pyright / Pylance at them by adding to `pyrightconfig.json` in your firmware repo:

```json
{
  "extraPaths": ["modules/Lpf2/stubs"]
}
```

Keep stubs in sync when adding or changing Python-facing APIs.

## License

AGPL-3.0-only. See [LICENSE](LICENSE).
