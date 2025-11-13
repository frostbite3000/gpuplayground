/* 
    GPUPlay
    Copyright © 2025 frostbite3000

    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)

    r128.h: Rage128 programming environment - GPUPLAYGROUND ONLY, NOT API
*/

#pragma once
#include <gpuplay.h>
#include "r128_ref.h"

//
// Structures
//

// Rage128 specific state
typedef struct r128_state_s
{
    uint32_t original_config_cntl;     // Store original CONFIG_CNTL for shutdown
} r128_state_t;

//
// Functions
//

bool r128_init();
void r128_shutdown();

/* Rage128 Tests */
bool r128_dump_mfg_info();
bool r128_dump_mmio();

// GPUS stuff
bool r128_gpus_section_applies(uint32_t fourcc);
bool r128_gpus_parse_section(uint32_t fourcc, FILE* stream);

// Rage128 specific state
extern r128_state_t r128_state;

