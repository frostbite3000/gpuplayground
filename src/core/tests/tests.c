/* 
    GPUPlay
    Copyright © 2025 frostbite3000

    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)

    tests.c: Enumerates all available tests in test mode
*/

#include "gpuplay.h"
#include <string.h>

#include <core/tests/tests.h>
#include <config/config.h>

// Architecture includes
#include <architecture/generic/nv_generic.h>
#include <architecture/r128/r128.h>
#include <architecture/voodoo3/voodoo3.h>

nv_test_t nv_tests[] = 
{
    // Generic tests
    { PCI_VENDOR_GENERIC, PCI_DEVICE_GENERIC, "NV_DumpPCI", "NV Generic - Dump PCI", NVGeneric_DumpPCISpace},
    { PCI_VENDOR_GENERIC, PCI_DEVICE_GENERIC, "NV_DumpMMIO", "NV Generic - Dump MMIO", NVGeneric_DumpMMIO},
    { PCI_VENDOR_GENERIC, PCI_DEVICE_GENERIC, "NV_DumpVBIOS", "NV Generic - Dump VBIOS", NVGeneric_DumpVBIOS},

    // Rage128 Pro PF tests
    { PCI_VENDOR_ATI, PCI_DEVICE_RAGE128_PRO_PF, "R128_DumpMfgInfo", "Rage128 Pro PF - Dump Mfg Info", r128_dump_mfg_info},
    { PCI_VENDOR_ATI, PCI_DEVICE_RAGE128_PRO_PF, "R128_DumpMMIO", "Rage128 Pro PF - Dump MMIO", r128_dump_mmio},

    // Rage128 Pro PR tests
    { PCI_VENDOR_ATI, PCI_DEVICE_RAGE128_PRO_PR, "R128_DumpMfgInfo", "Rage128 Pro PR - Dump Mfg Info", r128_dump_mfg_info},
    { PCI_VENDOR_ATI, PCI_DEVICE_RAGE128_PRO_PR, "R128_DumpMMIO", "Rage128 Pro PR - Dump MMIO", r128_dump_mmio},

    // Voodoo3 tests
    { PCI_VENDOR_3DFX, PCI_DEVICE_VOODOO3, "Voodoo3_DumpMfgInfo", "Voodoo3 - Dump Mfg Info", voodoo3_dump_mfg_info},
    { PCI_VENDOR_3DFX, PCI_DEVICE_VOODOO3, "Voodoo3_DumpMMIO", "Voodoo3 - Dump MMIO", voodoo3_dump_mmio},

    // Voodoo Banshee tests
    { PCI_VENDOR_3DFX, PCI_DEVICE_BANSHEE, "Voodoo3_DumpMfgInfo", "Voodoo Banshee - Dump Mfg Info", voodoo3_dump_mfg_info},
    { PCI_VENDOR_3DFX, PCI_DEVICE_BANSHEE, "Voodoo3_DumpMMIO", "Voodoo Banshee - Dump MMIO", voodoo3_dump_mmio},

    { 0x0000, 0x0000, "", "", NULL}, // Sentinel value, do not remove
};

/* Checks if a given test (represented by the test_name parameter) is available for the detected graphics hardware*/
bool Test_IsAvailableForGPU(const char* test_name)
{
    // do we need to sanity check these?
    uint32_t vendor_id = current_device.device_info.vendor_id;
    uint32_t device_id = current_device.device_info.device_id;

    uint32_t test_number = 0; 

    nv_test_t nv_test = nv_tests[test_number];

    // iterate through each test
    while (nv_test.test_function != NULL)
    {
        if (!strcmp(nv_test.name, test_name)
        && vendor_id == nv_test.required_vendor_id
        && device_id == nv_test.required_device_id)
            return true; 

        test_number++;
        nv_test = nv_tests[test_number];
    }

    return false; 
}

/* Acquires the test with the name test_name. The test must be loaded and supported */
nv_config_test_entry_t* Test_Get(const char* test_name)
{
    nv_config_test_entry_t* test_entry = config.test_list_head;

    // iterate through each test
    while (test_entry)
    {
        if (!strcmp(test_entry->name, test_name))
            return test_entry;

        test_entry = test_entry->next;
    }

    return NULL; 
}

bool Test_Run(nv_config_test_entry_t* test)
{
	/* 
		TODO: Ini setting to disable this print in the case of graphical tests.
		Otherwise we'll have to switch back to test mode every test.

		Also, test logging. (after util_logging.c is done)
	*/
	if (test->test_function)
	{
		bool success = test->test_function();	

		if (success)
			Logging_Write(log_level_message, "Test %s succeeded\n", test->name);
		else
			Logging_Write(log_level_message, "Test %s failed! :(\n", test->name);
	
        return success; 
    }
    
    //should never happen because we should explicitly check for this
    return false; 
}