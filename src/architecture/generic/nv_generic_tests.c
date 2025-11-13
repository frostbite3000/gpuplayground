/*
    GPUPlay
    Copyright © 2025 frostbite3000


    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)
    
    nv_generic_tests.c: Generic test suite implementation
*/

#include "gpuplay.h"
#include <architecture/generic/nv_generic.h>

// Architecture Includes
bool NVGeneric_DumpPCISpace()
{
    // read out the device id
    uint16_t vendor_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_VENDOR_ID);
    uint16_t device_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_DEVICE_ID);
    uint16_t command = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND);
    uint16_t status = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_STATUS);
    uint8_t revision = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_REVISION);
    uint8_t class_id_high = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_CLASS_CODE_HIGH);
    uint16_t class_id_low = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_CLASS_CODE_LOW);
    uint8_t cache_line_size = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_CACHE_LINE_SIZE);
    uint8_t latency_timer = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_LATENCY_TIMER);
    uint8_t header_type = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_HEADER_TYPE);
    uint8_t bist = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BIST);
    uint32_t bar0 = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar1 = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR1);
    uint32_t bar2 = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR2);
    uint32_t bar3 = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR3);
    uint32_t bar4 = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR4);
    uint32_t bar5 = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR5);
    uint32_t cardbus_cis_ptr = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_CARDBUS_CIS_PTR);
    uint16_t subsystem_vendor_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_SUBSYSTEM_VENDOR_ID);
    uint16_t subsystem_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_SUBSYSTEM_ID);
    uint32_t rom_bar = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_EXPANSION_ROM_BASE);
    uint8_t capabilities_ptr = PCI_ReadConfig8(current_device.bus_number, current_device.function_number,  PCI_CFG_OFFSET_CAPABILITIES_PTR);
    uint8_t interrupt_line = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_INTERRUPT_LINE);
    uint8_t interrupt_pin = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_INTERRUPT_PIN);
    uint8_t minimum_grant = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_MINIMUM_GRANT);
    uint8_t maximum_latency = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_MAXIMUM_LATENCY);

    Logging_Write(log_level_message, "[PCI CFG] PCI ID %04x:%04x\n", vendor_id, device_id);
    Logging_Write(log_level_message, "[PCI CFG] Command Register %04x\n", command);
    Logging_Write(log_level_message, "[PCI CFG] Status Register %04x\n", status);
    Logging_Write(log_level_message, "[PCI CFG] Revision %02x\n", revision);
    Logging_Write(log_level_message, "[PCI CFG] Class ID: %06x\n", (class_id_high << 16) | class_id_low);
    Logging_Write(log_level_message, "[PCI CFG] Cache Line Size %02x\n", cache_line_size);
    Logging_Write(log_level_message, "[PCI CFG] Latency Timer %02x\n", latency_timer);
    Logging_Write(log_level_message, "[PCI CFG] Header Type %02x (should be 0)\n", header_type);
    Logging_Write(log_level_message, "[PCI CFG] BIST %02x\n", bist);
    Logging_Write(log_level_message, "[PCI CFG] BAR0 %08x\n", bar0);
    Logging_Write(log_level_message, "[PCI CFG] BAR1 %08x\n", bar1);
    Logging_Write(log_level_message, "[PCI CFG] BAR2 %08x\n", bar2);
    Logging_Write(log_level_message, "[PCI CFG] BAR3 %08x\n", bar3);
    Logging_Write(log_level_message, "[PCI CFG] BAR4 %08x\n", bar4);
    Logging_Write(log_level_message, "[PCI CFG] BAR5 %08x\n", bar5);
    Logging_Write(log_level_message, "[PCI CFG] CardBus CIS Pointer %04x\n", cardbus_cis_ptr);
    Logging_Write(log_level_message, "[PCI CFG] Subsystem ID %04x:%04x\n", subsystem_vendor_id, subsystem_id);
    Logging_Write(log_level_message, "[PCI CFG] ROM BAR %04x\n", rom_bar);
    Logging_Write(log_level_message, "[PCI CFG] Capabilities Pointer %02x\n", capabilities_ptr);
    Logging_Write(log_level_message, "[PCI CFG] Interrupt Line %02x\n", interrupt_line);
    Logging_Write(log_level_message, "[PCI CFG] Interrupt Pin %02x\n", interrupt_pin);
    Logging_Write(log_level_message, "[PCI CFG] Minimum Grant %02x\n", minimum_grant);
    Logging_Write(log_level_message, "[PCI CFG] Maximum Latency %02x\n", maximum_latency);

    return true; 
}

bool NVGeneric_DumpMMIO()
{
    Logging_Write(log_level_message, "DumpMMIO not yet implemented for this GPU architecture\n");
    return false;
}

bool NVGeneric_DumpVBIOS()
{
    Logging_Write(log_level_message, "DumpVBIOS not yet implemented for this GPU architecture\n");
    return false;
}

bool NVGeneric_DumpFIFO()
{
    Logging_Write(log_level_message, "DumpFIFO not yet implemented for this GPU architecture\n");
    return false;
}

// Dump all currently loaded objects in the current channel
bool NVGeneric_DumpRAMHT()
{
    return true; 
}

// Dump all channels that are not context switched to
bool NVGeneric_DumpRAMFC()
{
    return true; 
}

// Dump any errors that may have occurred 
bool NVGeneric_DumpRAMRO()
{
    return true; 
}                    


bool NVGeneric_DumpPGRAPHCache()
{
    return true; 
}