# Chatter Firmware

> The repository for the core firmware that comes preloaded on every Chatter.

This fork adds more reliable messaging behavior and message-status features while keeping the original Chatter firmware architecture.

## Added messaging features

- **Delivery receipts**: outgoing messages transition to delivered when the recipient acknowledges them.
- **Read receipts**: outgoing messages can show when the recipient has opened the conversation.
- **Automatic retries**: unacknowledged outgoing messages are retried automatically, with a bounded retry count.
- **Failed-message state**: messages that remain unacknowledged after the retry limit are marked as failed.
- **Manual resend**: failed or pending messages can be sent again through the message service.
- **ACK validation**: acknowledgements are accepted only when the message exists, is outgoing, and belongs to the sender that issued the acknowledgement.
- **Duplicate protection**: repeated incoming packets with an existing message UID are not stored again, while a fresh ACK is returned.
- **Unread counts**: the message service can report the number of unread incoming messages in a conversation.
- **Message timestamps**: messages store a timestamp and transmit it with message packets so received messages can retain their original timestamp.
- **Persistent timestamps**: timestamps are stored in the message repository and survive firmware restarts. Older stored records without a timestamp remain readable and load with a zero timestamp.

### Timestamp behavior

The firmware uses the device's Unix time when a valid system clock is available. If the clock has not been initialized, it falls back to seconds since boot. The stored value is preserved across restarts, but a boot-time fallback is not a real-world date/time until the device clock has been initialized.

## Compiling

The firmware is based on the [Chatter Library](https://github.com/CircuitMess/Chatter-Library).

The library is included with other device libraries in the CircuitMess ESP32 Arduino platform.

More info and installation instructions
on [CircuitMess/Arduino-Packages](https://github.com/CircuitMess/Arduino-Packages).

## Using Arduino IDE

### Installing the package

Follow the package installation
instructions [here](https://github.com/CircuitMess/Arduino-Packages?tab=readme-ov-file#installation).

### Building

Open `Chatter-Firmware.ino` using Arduino IDE, set the board to Chatter, and compile.

## Using CMake

To compile and upload, you need to have [CMake](https://cmake.org/)
and [arduino-cli](https://github.com/arduino/arduino-cli) installed. You also need to have both of
them registered in the PATH.

In the `CMakeLists.txt` file, change the port to your desired COM port (default is `COM4` in this fork):

```cmake
set(PORT COM4)
```

Then in the root directory of the repository type:

```shell
mkdir cmake
cd cmake
cmake ..
cmake --build . --target CMBuild
```

This compiles the firmware and places the generated `.bin` and `.elf` files in the `build/` directory in the root of the repository.

To compile and upload the firmware according to the port set in `CMakeLists.txt`, run:

```shell
cmake --build . --target CMUpload
```

# Uploading SPIFFS

The ESP32 contains a Serial Peripheral Interface Flash File System (SPIFFS). SPIFFS is a lightweight filesystem created for microcontrollers with a flash chip.

Here are stored UI and audio assets used in the firmware.

## Using the Arduino ESP32 filesystem uploader plugin (only for Arduino 1.X)

Install the [plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin) following the instructions from the [README.md](https://github.com/me-no-dev/arduino-esp32fs-plugin?tab=readme-ov-file#installation).

Then from the opened sketch select **Tools > ESP32 Sketch Data Upload**. This should start uploading the files into ESP32 flash filesystem.

## Using the mkspiffs utility

When building with CMake or Arduino 2.X, you will need to build and upload the SPIFFS image separately.

First, download the latest [mkspiffs](https://github.com/igrr/mkspiffs) utility for your OS with the `-arduino-esp32` suffix. For example, `mkspiffs-0.2.3-arduino-esp32-win32.zip` for Windows.

Then create the binary SPIFFS image using the command in the root of the project:

```shell
mkspiffs -c data -s 0x1EF000 -b 4096 -p 256 spiffs.bin
```

The block size (`-b`) and page size (`-p`) parameters should stay as-is.

The size parameter (`-s`) can be determined from the board-specific SPIFFS partition size, which can be found in the platform `boards.txt` under `<device>.menu.PartitionScheme.min_spiffs.upload.maximum_size`.

For uploading the image, you will need to download [esptool](https://github.com/espressif/esptool).

Then, flash the compiled image to the board. The SPIFFS partition address is defined alongside the SPIFFS partition size under `<device>.menu.PartitionScheme.min_spiffs.upload.spiffs_start`.

For the default partition layout used by the original project:

```shell
esptool --chip esp32 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0x211000 spiffs.bin
```

# Restoring the stock firmware

There are three main ways to restore the stock firmware.

### 1) Restoring using esptool

For uploading the firmware this way, you will need to download [esptool](https://github.com/espressif/esptool).

Then download the prebuilt binary on the [CircuitMess Chatter Firmware releases page](https://github.com/CircuitMess/Chatter-Firmware/releases) and flash it manually using esptool:

```shell
esptool write_flash 0x0 Codee-Firmware.bin
```

### 2) Restoring using Arduino's burn bootloader option

This Arduino option is usually reserved for bootloader flashing.

For devices included in the CircuitMess ESP32 Arduino platform this will actually restore the firmware.

Open this project in Arduino and select your board in the `Tools > Board` dropdown menu.

Then select the appropriate firmware under `Tools > Programmer` and click the `Tools > Burn bootloader` option.

### 3) Restoring using CircuitBlocks

[CircuitBlocks](https://code.circuitmess.com/) is our educational block-based coding platform.

You can also restore your firmware here by logging in, clicking the **Restore Firmware** button in the top-right corner, and following the on-screen instructions.

---
Copyright © 2025 CircuitMess

Licensed under [MIT License](https://opensource.org/licenses/MIT).
