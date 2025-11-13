/* 
    GPUPlay
    Copyright © 2025 frostbite3000

    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)

    voodoo3_core.c: 3Dfx Voodoo3/Banshee core functions (bringup, shutdown, mainloop)
*/

// Filename: voodoo3_core.c
// Purpose: Voodoo3 initialization and core functions

// Architecture Includes
#include <architecture/voodoo3/voodoo3.h>
#include <architecture/voodoo3/voodoo3_ref.h>

#include "gpuplay.h"
#include "util/util.h"
#include "pc.h"
#include <stdio.h>
#include <stdlib.h>

voodoo3_state_t voodoo3_state = {0};                    // Voodoo3 specific state

// I/O port access functions for Voodoo3
// Voodoo3 uses I/O ports accessed via BAR2 (PCI18)
// The I/O base address is stored as a port number (not a memory address)
static uint16_t voodoo3_io_base_port = 0;

static inline uint8_t voodoo3_io_read8(uint32_t offset)
{
    if (voodoo3_io_base_port == 0)
        return 0;
    
    // Access I/O port at io_base_port + offset
    return inportb(voodoo3_io_base_port + (uint16_t)offset);
}

static inline uint16_t voodoo3_io_read16(uint32_t offset)
{
    if (voodoo3_io_base_port == 0)
        return 0;
    
    // Access I/O port at io_base_port + offset
    return inportw(voodoo3_io_base_port + (uint16_t)offset);
}

static inline uint32_t voodoo3_io_read32(uint32_t offset)
{
    if (voodoo3_io_base_port == 0)
        return 0;
    
    // For 32-bit reads, read two 16-bit values
    // Note: Voodoo3 registers are typically 32-bit aligned
    uint16_t port = voodoo3_io_base_port + (uint16_t)offset;
    uint32_t low = inportw(port);
    uint32_t high = inportw(port + 2);
    
    return low | (high << 16);
}

static inline void voodoo3_io_write8(uint32_t offset, uint8_t value)
{
    if (voodoo3_io_base_port == 0)
        return;
    
    outportb(voodoo3_io_base_port + (uint16_t)offset, value);
}

static inline void voodoo3_io_write16(uint32_t offset, uint16_t value)
{
    if (voodoo3_io_base_port == 0)
        return;
    
    outportw(voodoo3_io_base_port + (uint16_t)offset, value);
}

static inline void voodoo3_io_write32(uint32_t offset, uint32_t value)
{
    if (voodoo3_io_base_port == 0)
        return;
    
    // For 32-bit writes, write two 16-bit values
    // Note: Voodoo3 registers are typically 32-bit aligned
    uint16_t port = voodoo3_io_base_port + (uint16_t)offset;
    outportw(port, (uint16_t)(value & 0xFFFF));
    outportw(port + 2, (uint16_t)((value >> 16) & 0xFFFF));
}

bool voodoo3_init()
{
    // Read PCI BARs
    // Voodoo3 layout:
    // BAR0 = Frame Buffer - 32MB non-prefetchable memory
    // BAR1 = Texture Memory - 32MB prefetchable memory
    // BAR2 = I/O ports (256 bytes) - Register access via PCI18 (ioBaseAddr)
    uint32_t bar0_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR0);
    uint32_t bar1_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR1);
    uint32_t bar2_base = PCI_ReadConfig32(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_BAR2);

    /* According to PCI spec, only the top bits matter for base addresses */
    bar0_base &= 0xFFFFFFF0;  // 32-bit memory space, 4-byte aligned
    bar1_base &= 0xFFFFFFF0;  // 32-bit memory space, 4-byte aligned
    
    // BAR2 for I/O space: bits 0-1 indicate it's I/O space (bit 0 = 1)
    // The actual I/O port address is in bits 2-31
    // For I/O space, we need to extract the port number
    if (bar2_base & 0x01)
    {
        // This is I/O space - extract the port address
        voodoo3_io_base_port = (uint16_t)(bar2_base & 0xFFFC);  // I/O port address (aligned to 4 bytes)
        Logging_Write(log_level_debug, "Voodoo3 - PCI BAR2 (I/O Ports) 0x%04X\n", voodoo3_io_base_port);
    }
    else
    {
        Logging_Write(log_level_error, "Voodoo3 - BAR2 is not I/O space! (0x%08lX)\n", bar2_base);
        return false;
    }

    Logging_Write(log_level_debug, "Voodoo3 - PCI BAR0 (Frame Buffer) 0x%08lX\n", bar0_base);
    Logging_Write(log_level_debug, "Voodoo3 - PCI BAR1 (Texture Memory) 0x%08lX\n", bar1_base);
    
    /* We need to allocate an LDT for memory BARs */
    /* So start by allocating a physical mapping. */

    __dpmi_meminfo meminfo_bar0 = {0};
    __dpmi_meminfo meminfo_bar1 = {0};

    // BAR0 is the Frame Buffer (32MB)
    meminfo_bar0.address = bar0_base;
    meminfo_bar0.size = 0x2000000;  // 32MB
    
    // BAR1 is the Texture Memory (32MB)
    meminfo_bar1.address = bar1_base;
    meminfo_bar1.size = 0x2000000;  // 32MB

    current_device.bar1_dfb_start = bar0_base;  // Frame buffer is like BAR1 on NV cards
    current_device.ramin_start = bar1_base;     // Texture memory

    __dpmi_physical_address_mapping(&meminfo_bar0);
    __dpmi_physical_address_mapping(&meminfo_bar1);
    
    Logging_Write(log_level_debug, "Voodoo3 Init: Mapping BAR0 (Frame Buffer - 32MB) to bar1_selector...\n");

    /* Set up LDT for BAR0 (Frame Buffer) */
    current_device.bar1_selector = __dpmi_allocate_ldt_descriptors(1);
    __dpmi_set_segment_base_address(current_device.bar1_selector, meminfo_bar0.address);
    __dpmi_set_segment_limit(current_device.bar1_selector, 0x2000000 - 1);  // 32MB

    // Note: BAR2 is I/O ports, not memory-mapped, so we don't set up bar0_selector for it
    // I/O access is done directly via inport/outport functions

    /* Read configuration registers */
    uint32_t vendor_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_VENDOR_ID);
    uint32_t device_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_DEVICE_ID);
    uint32_t revision_id = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_REVISION);

    Logging_Write(log_level_debug, "Voodoo3 - Vendor ID: 0x%04X\n", vendor_id);
    Logging_Write(log_level_debug, "Voodoo3 - Device ID: 0x%04X\n", device_id);
    Logging_Write(log_level_debug, "Voodoo3 - Revision ID: 0x%02X\n", revision_id);

    /* Read memory size from lfbMemoryConfig register */
    uint32_t lfb_mem_config = voodoo3_io_read32(VOODOO3_IO_LFBMEMORYCONFIG);
    uint32_t memsize_value = lfb_mem_config & VOODOO3_LFBMEMORYCONFIG_MEMORY_SIZE_MASK;

    if (memsize_value == VOODOO3_LFBMEMORYCONFIG_MEMORY_SIZE_4MB)
        current_device.vram_amount = VOODOO3_VRAM_SIZE_4MB;
    else if (memsize_value == VOODOO3_LFBMEMORYCONFIG_MEMORY_SIZE_8MB)
        current_device.vram_amount = VOODOO3_VRAM_SIZE_8MB;
    else if (memsize_value == VOODOO3_LFBMEMORYCONFIG_MEMORY_SIZE_16MB)
        current_device.vram_amount = VOODOO3_VRAM_SIZE_16MB;
    else if (memsize_value == VOODOO3_LFBMEMORYCONFIG_MEMORY_SIZE_32MB)
        current_device.vram_amount = VOODOO3_VRAM_SIZE_32MB;
    else
    {
        Logging_Write(log_level_warning, "Voodoo3 - Unknown memory size value: 0x%02X, defaulting to 16MB\n", memsize_value);
        current_device.vram_amount = VOODOO3_VRAM_SIZE_16MB;
    }

    Logging_Write(log_level_debug, "Voodoo3 - VRAM Size: %lu MB\n", (current_device.vram_amount / 1048576));

    /* Read initialization registers */
    uint32_t misc_init1 = voodoo3_io_read32(VOODOO3_IO_MISCINIT1);
    current_device.straps = misc_init1;

    /* Store original PCI command for shutdown */
    uint16_t command = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND);
    voodoo3_state.original_pci_command = command;

    /* Enable bus mastering if not already enabled */
    if (!(command & PCI_CFG_OFFSET_COMMAND_BUS_MASTER))
    {
        Logging_Write(log_level_debug, "Voodoo3 Init: Enabling bus mastering...\n");
        command |= PCI_CFG_OFFSET_COMMAND_BUS_MASTER;
        PCI_WriteConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND, command);
    }

    /* Enable memory access if not already enabled */
    if (!(command & PCI_CFG_OFFSET_COMMAND_MEM_ENABLED))
    {
        Logging_Write(log_level_debug, "Voodoo3 Init: Enabling memory access...\n");
        command |= PCI_CFG_OFFSET_COMMAND_MEM_ENABLED;
        PCI_WriteConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND, command);
    }

    /* Enable I/O access if not already enabled */
    if (!(command & PCI_CFG_OFFSET_COMMAND_IO_ENABLED))
    {
        Logging_Write(log_level_debug, "Voodoo3 Init: Enabling I/O access...\n");
        command |= PCI_CFG_OFFSET_COMMAND_IO_ENABLED;
        PCI_WriteConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND, command);
    }

    Logging_Write(log_level_debug, "Voodoo3 Init: Initialization complete!\n");

    return true; 
}

void voodoo3_shutdown()
{
    // Restore original PCI command if needed
    if (voodoo3_state.original_pci_command != 0)
    {
        PCI_WriteConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_COMMAND, voodoo3_state.original_pci_command);
    }

    Logging_Write(log_level_debug, "Voodoo3 Shutdown: Complete\n");
}

bool voodoo3_dump_mfg_info()
{
    Logging_Write(log_level_message, "3Dfx Voodoo3 Manufacture-Time Configuration: \n");
    
    uint32_t vendor_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_VENDOR_ID);
    uint32_t device_id = PCI_ReadConfig16(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_DEVICE_ID);
    uint32_t revision_id = PCI_ReadConfig8(current_device.bus_number, current_device.function_number, PCI_CFG_OFFSET_REVISION);
    
    Logging_Write(log_level_message, "Vendor ID          = 0x%04X\n", vendor_id);
    Logging_Write(log_level_message, "Device ID          = 0x%04X\n", device_id);
    Logging_Write(log_level_message, "Revision ID        = 0x%02X\n", revision_id);
    
    uint32_t lfb_mem_config = voodoo3_io_read32(VOODOO3_IO_LFBMEMORYCONFIG);
    Logging_Write(log_level_message, "lfbMemoryConfig     = 0x%08lX\n", lfb_mem_config);
    Logging_Write(log_level_message, "Video RAM Size     = %lu MB\n", (current_device.vram_amount / 1048576));
    
    uint32_t misc_init0 = voodoo3_io_read32(VOODOO3_IO_MISCINIT0);
    uint32_t misc_init1 = voodoo3_io_read32(VOODOO3_IO_MISCINIT1);
    Logging_Write(log_level_message, "miscInit0          = 0x%08lX\n", misc_init0);
    Logging_Write(log_level_message, "miscInit1          = 0x%08lX\n", misc_init1);
    
    uint32_t dram_init0 = voodoo3_io_read32(VOODOO3_IO_DRAMINIT0);
    uint32_t dram_init1 = voodoo3_io_read32(VOODOO3_IO_DRAMINIT1);
    Logging_Write(log_level_message, "dramInit0          = 0x%08lX\n", dram_init0);
    Logging_Write(log_level_message, "dramInit1          = 0x%08lX\n", dram_init1);
    
    uint32_t status = voodoo3_io_read32(VOODOO3_IO_STATUS);
    Logging_Write(log_level_message, "status             = 0x%08lX\n", status);
    
    return true; 
}

bool voodoo3_dump_mmio()
{
    Logging_Write(log_level_message, "Dumping Voodoo3 I/O register space...\n");
    
    if (voodoo3_io_base_port == 0)
    {
        Logging_Write(log_level_error, "Voodoo3 I/O base port not initialized!\n");
        return false;
    }
    
    FILE* io_dump = fopen("voodoo3_io_dump.bin", "wb");
    if (!io_dump)
    {
        Logging_Write(log_level_error, "Failed to open voodoo3_io_dump.bin for writing\n");
        return false;
    }
    
    uint32_t* io_buffer = calloc(1, 0x1000);
    if (!io_buffer)
    {
        Logging_Write(log_level_error, "Failed to allocate memory for I/O dump\n");
        fclose(io_dump);
        return false;
    }
    
    // Read I/O space (256 bytes = 0x100, but we'll dump 1KB for safety)
    // Voodoo3 I/O space is 256 bytes, but we'll read up to 1KB
    for (uint32_t offset = 0; offset < 0x1000; offset += 4)
    {
        if (offset % 0x100 == 0)
            Logging_Write(log_level_debug, "Dumping I/O ports at offset 0x%04X (port 0x%04X)\n", offset, voodoo3_io_base_port + offset);
        
        // Read 32-bit value from I/O port
        io_buffer[offset >> 2] = voodoo3_io_read32(offset);
    }
    
    fwrite(io_buffer, 0x1000, 1, io_dump);
    fclose(io_dump);
    free(io_buffer);
    
    Logging_Write(log_level_message, "I/O dump complete: voodoo3_io_dump.bin (dumped %d bytes from I/O ports 0x%04X-0x%04X)\n", 
                  0x1000, voodoo3_io_base_port, voodoo3_io_base_port + 0x1000 - 1);
    
    return true;
}

bool voodoo3_gpus_section_applies(uint32_t fourcc)
{
    // Voodoo3 doesn't use GPUS format yet
    return false;
}

bool voodoo3_gpus_parse_section(uint32_t fourcc, FILE* stream)
{
    // Voodoo3 doesn't use GPUS format yet
    return false;
}

