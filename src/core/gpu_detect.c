/* 
    GPUPlay
    Copyright © 2025 frostbite3000

    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)

    gpu_detect.c: Code for detecting graphics hardware
*/

#include <gpuplay.h>

// The selected device after detection is done. 
nv_device_t current_device = {0}; 

bool GPU_Detect()
{
    nv_device_info_t current_device_info = supported_devices[0]; 
    int32_t i = 0; 

    // iterate through all the known device info sets
    while (current_device_info.vendor_id != 0x00)
    {
        current_device_info = supported_devices[i];
        
        Logging_Write(log_level_debug, "Trying to find GPU: %s\n", current_device_info.name);

        if (PCI_DevicePresent(current_device_info.device_id, current_device_info.vendor_id))
        {
            Logging_Write(log_level_message, "Detected GPU: %s\n", current_device_info.name);

            // set up current info
            current_device.device_info = current_device_info;
            return true; 
        }

        i++;
    }

    Logging_Write(log_level_error, "No supported Other GPU found\n");
    return false; 

}
