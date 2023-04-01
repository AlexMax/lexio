$BUILD_TYPE="${Env:BUILD_TYPE}"

if (-Not $BUILD_TYPE) {
    $BUILD_TYPE="Debug"
}

Write-Output "CMAKE_BUILD_TYPE=${BUILD_TYPE}"

# Find Visual Studio and set up compile environment, so we can run with ASan
$VSWHERE = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$VSPATH = & "${VSWHERE}" -latest -property installationPath
& "${VSPATH}\Common7\Tools\Launch-VsDevShell.ps1"

Push-Location "${PSScriptRoot}\..\build\tests"

& ".\${BUILD_TYPE}\test.exe"

Pop-Location
