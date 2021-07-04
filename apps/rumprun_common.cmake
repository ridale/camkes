#
# Copyright 2018, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.7.2)

RequireFile(RUMPRUN_PATH rumprun PATHS "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/tools")
RequireFile(CAMKES_HELPERS_PATH camkes_helpers.cmake PATHS "${RUMPRUN_PATH}/platform/sel4/camkes/")
add_subdirectory(${RUMPRUN_PATH} ${CMAKE_CURRENT_BINARY_DIR}/rumprun)
include(${CAMKES_HELPERS_PATH})

include(ExternalProject)

function(DeclareExternalRumprunProject external_target_name source_dir files)

    add_custom_target(${external_target_name}-toolchains)
    add_dependencies(${external_target_name}-toolchains rumprun_toplevel_support)
    # Add ExternalProject for building a binary using rumprun toolchains
    set(stamp_dir ${CMAKE_CURRENT_BINARY_DIR}/${external_target_name}-stamp)
    ExternalProject_Add(
        ${external_target_name}
        SOURCE_DIR
        ${source_dir}
        INSTALL_DIR
        ${CMAKE_CURRENT_BINARY_DIR}
        STAMP_DIR
        ${stamp_dir}
        DEPENDS
        ${external_target_name}-toolchains
        BUILD_ALWAYS
        ON
        EXCLUDE_FROM_ALL
        CMAKE_ARGS
        -DCMAKE_TOOLCHAIN_FILE=$<TARGET_PROPERTY:rumprun_toplevel_support,RUMPRUN_TOOLCHAIN_CMAKE>
        -G
        Ninja
        -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_CURRENT_BINARY_DIR}
    )
    DeclareExternalProjObjectFiles(
        ${external_target_name}
        ${CMAKE_CURRENT_BINARY_DIR}
        FILES
        ${files}
    )

endfunction()
