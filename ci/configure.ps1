$USE_VSWHERE="${Env:USE_VSWHERE}"
$GENERATOR="${Env:GENERATOR}"
$CXX_STANDARD="${Env:CXX_STANDARD}"

if (-Not $USE_VSWHERE) {
    $USE_VSWHERE=""
}

if (-Not $GENERATOR) {
    $GENERATOR="Ninja Multi-Config"
}

if (-Not $CXX_STANDARD) {
    $CXX_STANDARD="20"
}

if (-Not $CODE_COVERAGE) {
    $CODE_COVERAGE="OFF"
}

Write-Output "USE_VSWHERE=${USE_VSWHERE}"
Write-Output "GENERATOR=${GENERATOR}"
Write-Output "CXX_STANDARD=${CXX_STANDARD}"
Write-Output "CODE_COVERAGE=${CODE_COVERAGE}"

if(Test-Path "${PSScriptRoot}\..\build") {
    Remove-Item -Recurse -Force "${PSScriptRoot}\..\build"
}

Push-Location .

# Find Visual Studio and set up compile environment.
if($USE_VSWHERE) {
    $VSWHERE = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $VSPATH = & "${VSWHERE}" -latest -property installationPath
    & "${VSPATH}\Common7\Tools\Launch-VsDevShell.ps1"
}

# Configure
cmake `
    -S "${PSScriptRoot}\.." `
    -B "${PSScriptRoot}\..\build" `
    -G "${GENERATOR}" `
    -D "CMAKE_CXX_STANDARD=${CXX_STANDARD}" `
    -D "LEXIO_ENABLE_TESTS=ON" `
    -D "CODE_COVERAGE=${CODE_COVERAGE}" `
    -D "SANITIZE_ADDRESS=ON" `
    -D "SANITIZE_UNDEFINED=ON"

Pop-Location
