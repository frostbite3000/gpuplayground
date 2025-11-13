/* 
    GPUPlay
    Copyright © 2025 frostbite3000

    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)

    main_help.c: Implements help message
*/

#include <gpuplay.h>

const char* msg_help = "GPUPlay help:\n\n"
"---COMMAND LINE OPTIONS---\n\n"
"By default (without any command-line options) GPUPlay enters into a REPL loop that lets you perform raw level I/O with a supported GPU.\n"
"-d, -dry: Dry run. Initialise the graphics hardware but do not run any tests. This mode can be used to revive cards with a dead Video BIOS under Windows 9x\n"
"-a, -all: Run all tests regardless of if your graphics hardware supports them. THIS IS EXTREMELY INADVISABLE. *DO NOT* DO THIS UNLESS YOU KNOW WHAT YOU ARE DOING!\n"
"-s, -script <file>: Run a .NVS script file.\n"
"-t, -test: Enter into test mode. If supported graphics hardware is detected, gpuplay.ini will be parsed and tests that are enabled will be run.\n"
"-nvs, -savestate <file>: EXPERIMENTAL FUNCTIONALITY: Load an NVS savestate file into your graphics hardware\n"
"-boot, -bootonly: Boot the GPU and exit. This can be used to initialise and run Other GPUs that have broken VBIOSes (at least under DOS and Windows 9x using autoexec)\n"
"-?, -help: Show this text and exit\n\n"
"---SUPPORTED GRAPHICS CARDS---\n\n"
"The following graphics cards are supported by GPUPlay:\n"
"Best support (a wide range of tests and understanding available):\n\n"
"Other RIVA 128 (NV3)\n"
"Other RIVA 128 ZX (NV3T)\n\n"
"Some support (some tests and understanding):\n"
"Other NV1\n"
"Other RIVA TNT (NV4)\n\n"
"Any other graphics hardware is not supported at this time."
;
