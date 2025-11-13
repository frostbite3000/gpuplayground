cmake_minimum_required(VERSION 3.26)
project(gpuplay)

set(CMAKE_GENERATOR Ninja)

set(GPUPLAY_OUTPUT_FILE_NAME "${CMAKE_PROJECT_NAME}.exe")
set(GPUPLAY_CONFIG_FILE_NAME "${CMAKE_PROJECT_NAME}.ini")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_compile_options(-Wall -std=gnu99 -save-temps)


file(GLOB sources CONFIGURE_DEPENDS 

# Util
"src/util/ini.c"
"src/util/util_cmdline.c"
"src/util/util_logging.c"
"src/util/util_string.c"

# Main
"src/main.c"
"src/main_help.c"

# PCI
"src/core/pci/pci.c"

# Config
"src/config/config.c"

# NVCore
"src/core/gpu_list.c"
"src/core/gpu_detect.c"
"src/core/gpu_io.c"
"src/core/gpu_repl.c"

# NVCore: Tests
"src/core/tests/tests.c"

# NVCore: Script Engine
"src/core/script/gpu_script_commands.c"
"src/core/script/gpu_script_parser.c"

# NVCore: GPU Savestate format
"src/core/formats/format_gpus.c"

# Architecture: Generic/Shared
"src/architecture/generic/nv_generic_tests.c"

# Architecture: R128
"src/architecture/r128/r128_core.c"

# Architecture: Voodoo3
"src/architecture/voodoo3/voodoo3_core.c"

)

add_executable(gpuplay ${sources})

# Base include directories
include_directories("./src")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  target_compile_definitions(gpuplay PRIVATE DEBUG)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  target_compile_definitions(gpuplay PRIVATE RELEASE)
endif()


# create required folders
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/out)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/out/assets)


add_custom_command(TARGET gpuplay POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_BINARY_DIR}/${GPUPLAY_OUTPUT_FILE_NAME}
  ${CMAKE_BINARY_DIR}/out/${GPUPLAY_OUTPUT_FILE_NAME}
)

add_custom_command(TARGET gpuplay POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_BINARY_DIR}/${GPUPLAY_CONFIG_FILE_NAME}
  ${CMAKE_BINARY_DIR}/out/${GPUPLAY_CONFIG_FILE_NAME}
)

add_custom_command(TARGET gpuplay POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/out/assets
)

# delete duplicate gpuplay.ini (it would be very stupid to move it to the root folder)
add_custom_command(TARGET gpuplay POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/assets/${GPUPLAY_CONFIG_FILE_NAME})

