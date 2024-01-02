$BUILD_TYPE="${Env:BUILD_TYPE}"
$USE_VSWHERE="${Env:USE_VSWHERE}"

if (-Not $BUILD_TYPE) {
    $BUILD_TYPE="Debug"
}

if (-Not $USE_VSWHERE) {
    $USE_VSWHERE=""
}

Write-Output "BUILD_TYPE=${BUILD_TYPE}"
Write-Output "USE_VSWHERE=${USE_VSWHERE}"

Push-Location .

# Find Visual Studio and set up compile environment.
if($USE_VSWHERE) {
    $VSWHERE = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $VSPATH = & "${VSWHERE}" -latest -property installationPath
    & "${VSPATH}\Common7\Tools\Launch-VsDevShell.ps1"
}

# Build
cmake --build "${PSScriptRoot}\..\build" --config "${BUILD_TYPE}"

Pop-Location
