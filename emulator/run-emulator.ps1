param(
    [Parameter(Mandatory = $true)]
    [string]$Firmware,
    [string]$WokwiCli = "wokwi-cli"
)

$ErrorActionPreference = "Stop"
$emulator = Split-Path -Parent $MyInvocation.MyCommand.Path
$source = (Resolve-Path $Firmware).Path
$extension = [IO.Path]::GetExtension($source).ToLowerInvariant()

if ($extension -ne ".elf") {
    throw "Wokwi needs an ELF file. Build with arduino-cli and pass the generated .elf file."
}

Copy-Item $source (Join-Path $emulator "firmware.elf") -Force
Push-Location $emulator
try {
    & $WokwiCli .
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}
finally {
    Pop-Location
}
