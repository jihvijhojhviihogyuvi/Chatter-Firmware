# Chatter Firmware Emulator

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
