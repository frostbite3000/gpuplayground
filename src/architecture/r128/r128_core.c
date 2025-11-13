/* 
    GPUPlay
    Copyright © 2025 frostbite3000

    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)

    r128_core.c: Rage128 core functions (bringup, shutdown, mainloop)
*/

// Filename: r128_core.c
// Purpose: Rage128 initialization and core functions

// Architecture Includes
#include <architecture/r128/r128.h>
#include <architecture/r128/r128_ref.h>

#include "gpuplay.h"
#include "util/util.h"
#include <stdio.h>
#include <stdlib.h>

r128_state_t r128_state = {0};                    // Rage128 specific state 

bool r128_init()
{
    // Read PCI BARs
    // Rage128 layout:
    // BAR0 = Linear Frame Buffer (LFB) - 64MB prefetchable memory
    // BAR1 = I/O ports (256 bytes) - not used for MMIO
    // BAR2 = Register Map (MMIO) - 16KB non-prefetchable memory
    uint32_t bar0_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar2_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR2);

    /* According to PCI spec, only the top bits matter for base addresses */
    bar0_base &= 0xFFFFFFF0;  // 32-bit memory space, 4-byte aligned
    bar2_base &= 0xFFFFFFF0;  // 32-bit memory space, 4-byte aligned

    Logging_Write(log_level_debug, "R128 - PCI BAR0 (LFB) 0x%08lX\n", bar0_base);
    Logging_Write(log_level_debug, "R128 - PCI BAR2 (MMIO) 0x%08lX\n", bar2_base);
    
    /* We need to allocate an LDT for this */
    /* So start by allocating a physical mapping. */

    __dpmi_meminfo meminfo_bar0 = {0};
    __dpmi_meminfo meminfo_bar2 = {0};

    // BAR0 is the Linear Frame Buffer (64MB)
    meminfo_bar0.address = bar0_base;
    meminfo_bar0.size = 0x4000000;  // 64MB
    
    // BAR2 is the Register Map / MMIO (16KB)
    meminfo_bar2.address = bar2_base;
    meminfo_bar2.size = 0x4000;  // 16KB

    current_device.bar1_dfb_start = bar0_base;  // LFB is like BAR1 on NV cards
    // Rage128 doesn't use RAMIN like NV cards, but we'll set it for compatibility
    current_device.ramin_start = bar2_base;

    __dpmi_physical_address_mapping(&meminfo_bar0);
    __dpmi_physical_address_mapping(&meminfo_bar2);
    
    Logging_Write(log_level_debug, "R128 Init: Mapping BAR2 (MMIO - 16KB) to bar0_selector...\n");

    /* Set up LDT for BAR2 (MMIO) - MMIO goes to bar0_selector since mmio_read32 uses it */
    current_device.bar0_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar0_selector, meminfo_bar2.address);
    __dpmi_set_segment_limit(current_device.bar0_selector, 0x4000 - 1);  // 16KB

    Logging_Write(log_level_debug, "R128 Init: Mapping BAR0 (LFB - 64MB) to bar1_selector...\n");

    /* Set up LDT for BAR0 (LFB) - LFB goes to bar1_selector like DFB on NV cards */
    current_device.bar1_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar1_selector, meminfo_bar0.address);
    __dpmi_set_segment_limit(current_device.bar1_selector, 0x4000000 - 1);  // 64MB

    /* Read configuration registers */
    uint32_t vendor_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_VENDOR_ID);
    uint32_t device_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_DEVICE_ID);
    uint32_t revision_id = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_REVISION);

    Logging_Write(log_level_debug, "R128 - Vendor ID: 0x%04X\n", vendor_id);
    Logging_Write(log_level_debug, "R128 - Device ID: 0x%04X\n", device_id);
    Logging_Write(log_level_debug, "R128 - Revision ID: 0x%02X\n", revision_id);

    /* Read memory size from CONFIG_MEMSIZE register */
    uint32_t memsize = mmio_read32(R128_CONFIG_MEMSIZE);
    uint32_t memsize_value = memsize & R128_CONFIG_MEMSIZE_MASK;

    if (memsize_value == R128_CONFIG_MEMSIZE_8MB)
        current_device.vram_amount = R128_VRAM_SIZE_8MB;
    else if (memsize_value == R128_CONFIG_MEMSIZE_16MB)
        current_device.vram_amount = R128_VRAM_SIZE_16MB;
    else if (memsize_value == R128_CONFIG_MEMSIZE_32MB)
        current_device.vram_amount = R128_VRAM_SIZE_32MB;
    else
    {
        Logging_Write(log_level_warning, "R128 - Unknown memory size value: 0x%02X, defaulting to 16MB\n", memsize_value);
        current_device.vram_amount = R128_VRAM_SIZE_16MB;
    }

    Logging_Write(log_level_debug, "R128 - VRAM Size: %lu MB\n", (current_device.vram_amount / 1048576));

    /* Read external straps */
    current_device.straps = mmio_read32(R128_CONFIG_XSTRAP);

    /* Store original CONFIG_CNTL for shutdown */
    r128_state.original_config_cntl = mmio_read32(R128_CONFIG_CNTL);

    /* Enable bus mastering if not already enabled */
    uint16_t command = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND);
    if (!(command & PCI_CFG_OFFSET_COMMAND_BUS_MASTER))
    {
        Logging_Write(log_level_debug, "R128 Init: Enabling bus mastering...\n");
        command |= PCI_CFG_OFFSET_COMMAND_BUS_MASTER;
        PCI_WriteConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND, command);
    }

    Logging_Write(log_level_debug, "R128 Init: Initialization complete!\n");

    return true; 
}

void r128_shutdown()
{
    // Restore original CONFIG_CNTL if needed
    if (r128_state.original_config_cntl != 0)
    {
        mmio_write32(R128_CONFIG_CNTL, r128_state.original_config_cntl);
    }

    Logging_Write(log_level_debug, "R128 Shutdown: Complete\n");
}

bool r128_dump_mfg_info()
{
    Logging_Write(log_level_message, "Rage128 Manufacture-Time Configuration: \n");
    
    uint32_t vendor_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_VENDOR_ID);
    uint32_t device_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_DEVICE_ID);
    uint32_t revision_id = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_REVISION);
    
    Logging_Write(log_level_message, "Vendor ID          = 0x%04X\n", vendor_id);
    Logging_Write(log_level_message, "Device ID          = 0x%04X\n", device_id);
    Logging_Write(log_level_message, "Revision ID        = 0x%02X\n", revision_id);
    
    uint32_t memsize = mmio_read32(R128_CONFIG_MEMSIZE);
    Logging_Write(log_level_message, "CONFIG_MEMSIZE     = 0x%08lX\n", memsize);
    Logging_Write(log_level_message, "Video RAM Size     = %lu MB\n", (current_device.vram_amount / 1048576));
    
    uint32_t straps = mmio_read32(R128_CONFIG_XSTRAP);
    Logging_Write(log_level_message, "CONFIG_XSTRAP      = 0x%08lX\n", straps);
    
    uint32_t config_cntl = mmio_read32(R128_CONFIG_CNTL);
    Logging_Write(log_level_message, "CONFIG_CNTL        = 0x%08lX\n", config_cntl);
    
    uint32_t bus_cntl = mmio_read32(R128_BUS_CNTL);
    Logging_Write(log_level_message, "BUS_CNTL           = 0x%08lX\n", bus_cntl);
    
    uint32_t mem_cntl = mmio_read32(R128_MEM_CNTL);
    Logging_Write(log_level_message, "MEM_CNTL           = 0x%08lX\n", mem_cntl);
    
    return true; 
}

bool r128_dump_mmio()
{
    Logging_Write(log_level_message, "Dumping Rage128 MMIO registers...\n");
    
    FILE* mmio_dump = fopen("r128_mmio_dump.bin", "wb");
    if (!mmio_dump)
    {
        Logging_Write(log_level_error, "Failed to open r128_mmio_dump.bin for writing\n");
        return false;
    }
    
    uint32_t* mmio_buffer = calloc(1, R128_MMIO_SIZE);
    if (!mmio_buffer)
    {
        Logging_Write(log_level_error, "Failed to allocate memory for MMIO dump\n");
        fclose(mmio_dump);
        return false;
    }
    
    // Read MMIO space (16KB)
    for (uint32_t addr = 0; addr < R128_MMIO_SIZE; addr += 4)
    {
        if (addr % 0x1000 == 0)
            Logging_Write(log_level_debug, "Dumping MMIO up to 0x%08X\n", addr);
        
        mmio_buffer[addr >> 2] = mmio_read32(addr);
    }
    
    fwrite(mmio_buffer, R128_MMIO_SIZE, 1, mmio_dump);
    fclose(mmio_dump);
    free(mmio_buffer);
    
    Logging_Write(log_level_message, "MMIO dump complete: r128_mmio_dump.bin\n");
    
    return true;
}

bool r128_gpus_section_applies(uint32_t fourcc)
{
    // Rage128 doesn't use GPUS format yet
    return false;
}

bool r128_gpus_parse_section(uint32_t fourcc, FILE* stream)
{
    // Rage128 doesn't use GPUS format yet
    return false;
}

