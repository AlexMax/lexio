$BUILD_TYPE="${Env:BUILD_TYPE}"

if (-Not $BUILD_TYPE) {
    $BUILD_TYPE="Debug"
}

Write-Output "CMAKE_BUILD_TYPE=${BUILD_TYPE}"

Push-Location "${PSScriptRoot}\..\build"

dir ".\"
dir ".\${BUILD_TYPE}"

& ".\${BUILD_TYPE}\test.exe"

Pop-Location
