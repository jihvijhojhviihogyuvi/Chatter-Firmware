$ErrorActionPreference = "Stop"
$script = Join-Path $PSScriptRoot "software-emulator.py"
py $script
