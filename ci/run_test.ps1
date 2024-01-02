$BUILD_TYPE="${Env:BUILD_TYPE}"
if (-Not $BUILD_TYPE) {
    $BUILD_TYPE="Debug"
}
Write-Output "BUILD_TYPE=${BUILD_TYPE}"

$USE_VSWHERE="${Env:USE_VSWHERE}"
if (-Not $USE_VSWHERE) {
    $USE_VSWHERE=""
}
Write-Output "USE_VSWHERE=${USE_VSWHERE}"

$CODE_COVERAGE="${Env:CODE_COVERAGE}"
if (-Not $CODE_COVERAGE) {
    $CODE_COVERAGE=""
}
Write-Output "CODE_COVERAGE=${CODE_COVERAGE}"

# Find Visual Studio and set up compile environment, so we can run with ASan
if($USE_VSWHERE) {
    $VSWHERE = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $VSPATH = & "${VSWHERE}" -latest -property installationPath
    & "${VSPATH}\Common7\Tools\Launch-VsDevShell.ps1"
}

if($CODE_COVERAGE) {
    # Run with code coverage.
    cmake --build "${PSScriptRoot}\..\build" --config "${BUILD_TYPE}" --target lexio_test_cov
} else {
    # Run tests by themselves.
    Push-Location "${PSScriptRoot}\..\build\tests"

    & ".\${BUILD_TYPE}\lexio_test.exe"

    Pop-Location
}
