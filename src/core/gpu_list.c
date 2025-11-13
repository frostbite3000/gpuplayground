/* 
    GPUPlay
    Copyright © 2025 frostbite3000

    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)

    gpu_list.c: Enumerates all supported graphics hardware.
*/


// Architecture Includes
#include <architecture/r128/r128.h>
#include <architecture/voodoo3/voodoo3.h>

// Globals
//
nv_device_info_t supported_devices[] = 
{
	{ PCI_DEVICE_RAGE128_PRO_PF, PCI_VENDOR_ATI, "Rage 128 Pro (PF)", r128_init, r128_shutdown, r128_gpus_section_applies, r128_gpus_parse_section, },
	{ PCI_DEVICE_RAGE128_PRO_PR, PCI_VENDOR_ATI, "Rage 128 Pro (PR)", r128_init, r128_shutdown, r128_gpus_section_applies, r128_gpus_parse_section, },
	{ PCI_DEVICE_VOODOO3, PCI_VENDOR_3DFX, "3Dfx Voodoo3", voodoo3_init, voodoo3_shutdown, voodoo3_gpus_section_applies, voodoo3_gpus_parse_section, },
	{ PCI_DEVICE_BANSHEE, PCI_VENDOR_3DFX, "3Dfx Voodoo Banshee", voodoo3_init, voodoo3_shutdown, voodoo3_gpus_section_applies, voodoo3_gpus_parse_section, },
	{ 0, 0, "", NULL, NULL, NULL, NULL, }, // sentinel
};
