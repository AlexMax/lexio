$GENERATOR="${Env:GENERATOR}"
$CXX_STANDARD="${Env:CXX_STANDARD}"
$LEXIO_TEST_NONSTD="${Env:LEXIO_TEST_NONSTD}"

if (-Not $GENERATOR) {
    $GENERATOR="Ninja Multi-Config"
}

if (-Not $CXX_STANDARD) {
    $CXX_STANDARD="20"
}

if (-Not $LEXIO_TEST_NONSTD) {
    $LEXIO_TEST_NONSTD="OFF"
}

if(Test-Path "${PSScriptRoot}\..\build") {
    Remove-Item -Recurse -Force "${PSScriptRoot}\..\build"
}

Push-Location .

# Find Visual Studio and set up compile environment.
$VSWHERE = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$VSPATH = & "${VSWHERE}" -latest -property installationPath
& "${VSPATH}\Common7\Tools\Launch-VsDevShell.ps1"

# Configure
cmake `
    -S "${PSScriptRoot}\.." `
    -B "${PSScriptRoot}\..\build" `
    -G "${GENERATOR}" `
    -D "CMAKE_CXX_STANDARD=${CXX_STANDARD}" `
    -D "LEXIO_TEST_NONSTD=${LEXIO_TEST_NONSTD}" `
    -D "SANITIZE_ADDRESS=ON" `
    -D "SANITIZE_UNDEFINED=ON"

Pop-Location
