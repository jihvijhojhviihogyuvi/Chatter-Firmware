$ErrorActionPreference = "Stop"
$repo = Split-Path -Parent $PSScriptRoot
$out = Join-Path $PSScriptRoot "wokwi-build"
$cli = "C:\Users\Charlie\.tools\arduino-cli.exe"
$data = "C:\Users\Charlie\.arduino-data"
$config = "C:\Users\Charlie\.arduino-config\arduino-cli.yaml"

if (-not (Test-Path $cli)) { throw "arduino-cli was not found at $cli" }
if (Test-Path $out) { Remove-Item -Recurse -Force $out }
$env:ARDUINO_DATA_DIR = $data
$env:ARDUINO_CONFIG_FILE = $config

& $cli compile --fqbn cm:esp32:chatter --output-dir $out `
  --build-property 'compiler.cpp.extra_flags=-DLOVYAN_PANEL=Panel_ILI9341 -DLOVYAN_WIDTH=240 -DLOVYAN_HEIGHT=320 -DLOVYAN_MOSI=26 -DLOVYAN_SCK=27 -DLOVYAN_CS=15 -DLOVYAN_DC=33 -DLOVYAN_RST=13' `
  (Join-Path $repo "Chatter-Firmware.ino")
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Copy-Item (Join-Path $out "Chatter-Firmware.ino.elf") (Join-Path $PSScriptRoot "firmware.elf") -Force
Write-Host "Real firmware ELF ready for Wokwi: $PSScriptRoot\firmware.elf"
