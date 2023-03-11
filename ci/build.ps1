$BUILD_TYPE="${Env:BUILD_TYPE}"

if (-Not $BUILD_TYPE) {
    $BUILD_TYPE="Debug"
}

Write-Output "CMAKE_BUILD_TYPE=${BUILD_TYPE}"

Push-Location .

# Find Visual Studio and set up compile environment.
$VSWHERE = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$VSPATH = & "${VSWHERE}" -latest -property installationPath
& "${VSPATH}\Common7\Tools\Launch-VsDevShell.ps1"

# Build
cmake --build "${PSScriptRoot}\..\build" --config "${BUILD_TYPE}"

Pop-Location
