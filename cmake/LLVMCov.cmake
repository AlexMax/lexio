# Copyright (c) 2024, Lexi Mayfield
# All rights reserved.
#
# Based on CodeCoverage.cmake
# Copyright (c) 2012 - 2017, Lars Bilke
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software without
#    specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

include(CMakeParseArguments)

find_program( LLVMCOV_PATH llvm-cov )
find_program( LLVMPROFDATA_PATH llvm-profdata )

set(LLVMCOV_COMPILER_FLAGS "-fprofile-instr-generate -fcoverage-mapping"
    CACHE INTERNAL "")

function(setup_target_for_coverage_llvmcov)
    set(options "")
    set(oneValueArgs BASE_DIRECTORY BUILD_DIRECTORY NAME)
    set(multiValueArgs SOURCES EXECUTABLE DEPENDENCIES)
    cmake_parse_arguments(Coverage "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT LLVMCOV_PATH)
        message(FATAL_ERROR "llvm-cov not found! Aborting...")
    endif() # NOT LLVMCOV_PATH

    if(NOT LLVMPROFDATA_PATH)
        message(FATAL_ERROR "llvm-profdata not found! Aborting...")
    endif() # NOT LLVMPROFDATA_PATH

    # Set base directory (as absolute path), or default to PROJECT_SOURCE_DIR
    if(DEFINED Coverage_BASE_DIRECTORY)
        get_filename_component(BASEDIR ${Coverage_BASE_DIRECTORY} ABSOLUTE)
    else()
        set(BASEDIR ${PROJECT_SOURCE_DIR})
    endif()

    if(DEFINED Coverage_BUILD_DIRECTORY)
        get_filename_component(BUILDDIR ${Coverage_BUILD_DIRECTORY} ABSOLUTE)
    else()
        set(BUILDDIR ${PROJECT_BINARY_DIR})
    endif()

    set(LLVMCOV_MERGE_CMD
        "${LLVMPROFDATA_PATH}" merge -sparse "${BUILDDIR}/default.profraw"
            -o "${BUILDDIR}/default.profdata")

    set(LLVMCOV_REPORT_CMD
        "${LLVMCOV_PATH}" report "${BUILDDIR}/lexio_test.exe"
            "-instr-profile=${BUILDDIR}/default.profdata"
            -sources ${Coverage_SOURCES})

    set(LLVMCOV_SHOW_CMD
        "${LLVMCOV_PATH}" show "${BUILDDIR}/lexio_test.exe"
            "-instr-profile=${BUILDDIR}/default.profdata"
            -sources ${Coverage_SOURCES})

    if(TRUE)
        message(STATUS "Executed command report")
        message(STATUS "Command to merge profile data: ")
        string(REPLACE ";" " " LLVMCOV_MERGE_CMD_SPACED "${LLVMCOV_MERGE_CMD}")
        message(STATUS "${LLVMCOV_MERGE_CMD_SPACED}")

        message(STATUS "Command to merge profile data: ")
        string(REPLACE ";" " " LLVMCOV_REPORT_CMD_SPACED "${LLVMCOV_REPORT_CMD}")
        message(STATUS "${LLVMCOV_REPORT_CMD_SPACED}")

        message(STATUS "Command to merge profile data: ")
        string(REPLACE ";" " " LLVMCOV_SHOW_CMD_SPACED "${LLVMCOV_SHOW_CMD}")
        message(STATUS "${LLVMCOV_SHOW_CMD_SPACED}")
    endif()

    # Setup target
    add_custom_target(${Coverage_NAME}
        COMMAND ${LLVMCOV_MERGE_CMD}
        COMMAND ${LLVMCOV_REPORT_CMD}
        COMMAND ${LLVMCOV_SHOW_CMD}

        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
        DEPENDS ${Coverage_DEPENDENCIES}
        VERBATIM) # Protect arguments to commands
endfunction()

function(append_llvmcov_compiler_flags_to_target name)
    separate_arguments(_flag_list NATIVE_COMMAND "${LLVMCOV_COMPILER_FLAGS}")
    target_compile_options(${name} PRIVATE ${_flag_list})
endfunction()
