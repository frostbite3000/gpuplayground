/* 
    GPUPlay
    Copyright © 2025 frostbite3000

    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)

    voodoo3.h: 3Dfx Voodoo3/Banshee programming environment - GPUPLAYGROUND ONLY, NOT API
*/

#pragma once
#include <gpuplay.h>
#include "voodoo3_ref.h"

//
// Structures
//

// Voodoo3 specific state
typedef struct voodoo3_state_s
{
    uint32_t original_pci_command;     // Store original PCI command for shutdown
} voodoo3_state_t;

//
// Functions
//

bool voodoo3_init();
void voodoo3_shutdown();

/* Voodoo3 Tests */
bool voodoo3_dump_mfg_info();
bool voodoo3_dump_mmio();

// GPUS stuff
bool voodoo3_gpus_section_applies(uint32_t fourcc);
bool voodoo3_gpus_parse_section(uint32_t fourcc, FILE* stream);

// Voodoo3 specific state
extern voodoo3_state_t voodoo3_state;

