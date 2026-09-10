# Chatter Firmware Emulator

## Software emulator

For a working local emulator that does not require Wokwi hardware models, run:

```powershell
.\emulator\start-software-emulator.ps1
```

It opens a local browser UI where you can load any `.elf` or `.bin` build, use the virtual buttons, change battery level, inject messages, and inspect a serial-style event log. This is a behavioral emulator; it does not execute Xtensa machine code.

## Real CPU Wokwi build

To build an ELF that executes on Wokwi's ESP32 CPU with the supported ILI9341 display model:

```powershell
.\emulator\build-wokwi.ps1
```

Then start Wokwi from this folder. This executes the firmware CPU, but uses an ILI9341-compatible display override because Wokwi does not model Chatter's ST7735S panel.

This project loads any Chatter firmware ELF into Wokwi. Build a firmware version, then pass its ELF file to the launcher:

```powershell
.\emulator\run-emulator.ps1 .\build_work\compiled\arduino_build.ino.elf
```

The launcher copies that ELF to `emulator/firmware.elf` and starts Wokwi. The copied ELF is ignored by Git.

## Setup

1. Install the Wokwi VS Code extension or Wokwi CLI.
2. Authenticate Wokwi when prompted.
3. Build any firmware revision for `cm:esp32:chatter`.
4. Run the launcher with that revision's `.elf` file.

For example:

```powershell
arduino-cli compile --fqbn cm:esp32:chatter --output-dir build\v1 Chatter-Firmware.ino
.\emulator\run-emulator.ps1 .\build\v1\Chatter-Firmware.ino.elf
```

The generic ESP32 board can execute the firmware CPU and expose serial output, but it cannot fully reproduce Chatter-specific display, LoRa, audio, battery, and button hardware. Those peripherals need Wokwi custom-chip models or a real Chatter device.
