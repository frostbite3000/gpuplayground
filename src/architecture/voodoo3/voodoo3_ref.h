/* 
    GPUPlay
    Copyright © 2025 frostbite3000

    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)

    voodoo3_ref.h: 3Dfx Voodoo3/Banshee hardware register definitions
    Based on Voodoo3 Programming Guide Rev 1.4 and Data Book Rev 1.9
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>

//
// PCI Configuration
//
#define VOODOO3_PCI_VENDOR_ID                             0x121A          // 3Dfx Interactive, Inc.
#define VOODOO3_PCI_DEVICE_ID_VOODOO3                    0x0005          // Voodoo3
#define VOODOO3_PCI_DEVICE_ID_BANSHEE                    0x0003          // Voodoo Banshee

//
// PCI BAR Layout
//
// BAR0: 32MB non-prefetchable memory (Frame Buffer)
// BAR1: 32MB prefetchable memory (Texture Memory)
// BAR2: I/O ports (256 bytes) - Register access via PCI18 (ioBaseAddr)

//
// Memory Sizes
//
#define VOODOO3_VRAM_SIZE_4MB                             0x400000        // 4MB
#define VOODOO3_VRAM_SIZE_8MB                             0x800000        // 8MB
#define VOODOO3_VRAM_SIZE_16MB                            0x1000000       // 16MB
#define VOODOO3_VRAM_SIZE_32MB                            0x2000000       // 32MB

//
// I/O Register Space (accessed via PCI18 + offset)
//
// Base address is PCI18 (ioBaseAddr) from PCI config space
// VGA registers are accessed at ioBaseAddr + (VGA_ADDR - 0x0300)
// For example: VGA Misc Output (0x03C2) = ioBaseAddr + 0x00C2

// Status and Control Registers
#define VOODOO3_IO_STATUS                                 0x0000          // Status register (read-only)
#define VOODOO3_IO_INTRCTRL                               0x0008          // Interrupt control

// Initialization Registers
#define VOODOO3_IO_PCIINIT0                               0x0010          // PCI initialization 0
#define VOODOO3_IO_SIPMONITOR                             0x0014          // SIP monitor register
#define VOODOO3_IO_LFBMEMORYCONFIG                        0x0018          // LFB memory configuration
#define VOODOO3_IO_MISCINIT0                              0x001C          // Miscellaneous initialization 0
#define VOODOO3_IO_MISCINIT1                              0x0020          // Miscellaneous initialization 1
#define VOODOO3_IO_DRAMINIT0                              0x0024          // DRAM initialization 0
#define VOODOO3_IO_DRAMINIT1                              0x0028          // DRAM initialization 1

// AGP Registers
#define VOODOO3_IO_AGPREQSIZE                             0x0030          // AGP request size
#define VOODOO3_IO_AGPHOSTADDRLOW                         0x0034          // AGP host address low
#define VOODOO3_IO_AGPHOSTADDRHIGH                        0x0038          // AGP host address high
#define VOODOO3_IO_AGPGRAPHICSADDR                        0x003C          // AGP graphics address
#define VOODOO3_IO_AGPGRAPHICSSTRIDE                      0x0040          // AGP graphics stride
#define VOODOO3_IO_AGPMOVECMD                             0x0044          // AGP move command
#define VOODOO3_IO_AGPINIT0                               0x0048          // AGP initialization 0

// TMU/Graphics Registers
#define VOODOO3_IO_TMUGBEINIT                             0x004C          // TMU GBE initialization

// VGA Registers
#define VOODOO3_IO_VGAINIT0                               0x0050          // VGA initialization 0
#define VOODOO3_IO_VGAINIT1                               0x0054          // VGA initialization 1

// 2D Command Register
#define VOODOO3_IO_2DCOMMAND                              0x0058          // 2D command register
#define VOODOO3_IO_2DSRCBASEADDR                          0x005C          // 2D source base address

// DRAM Data Register
#define VOODOO3_IO_DRAMDATA                               0x0060          // DRAM data register

// PLL and DAC Registers
#define VOODOO3_IO_PLLCTRL0                               0x0064          // PLL control 0
#define VOODOO3_IO_PLLCTRL1                               0x0068          // PLL control 1
#define VOODOO3_IO_PLLCTRL2                               0x006C          // PLL control 2
#define VOODOO3_IO_DACMODE                                0x0070          // DAC mode
#define VOODOO3_IO_DACADDR                                0x0074          // DAC address
#define VOODOO3_IO_DACDATA                                0x0078          // DAC data

// Video Registers
#define VOODOO3_IO_VIDTVOUTBLANKVCOUNT                    0x007C          // TV out blank vertical count
#define VOODOO3_IO_RGBMAXDELTA                            0x0080          // RGB max delta
#define VOODOO3_IO_VIDPROCCFG                             0x0084          // Video processor configuration
#define VOODOO3_IO_HWCURPATADDR                           0x0088          // Hardware cursor pattern address
#define VOODOO3_IO_HWCURLOC                               0x008C          // Hardware cursor location
#define VOODOO3_IO_HWCURC0                                0x0090          // Hardware cursor color 0
#define VOODOO3_IO_HWCURC1                                0x0094          // Hardware cursor color 1
#define VOODOO3_IO_VIDINFORMAT                            0x0098          // Video input format
#define VOODOO3_IO_VIDTVOUTBLANKHCOUNT                   0x009C          // TV out blank horizontal count
#define VOODOO3_IO_VIDSERIALPARALLELPORT                  0x00A0          // Video serial/parallel port
#define VOODOO3_IO_VIDINXDECIMDELTAS                      0x00A4          // Video input X decimation deltas
#define VOODOO3_IO_VIDINDECIMINITERR                      0x00A8          // Video input decimation initial errors
#define VOODOO3_IO_VIDINYDECIMDELTAS                      0x00AC          // Video input Y decimation deltas
#define VOODOO3_IO_VIDPIXELBUFFTHOLD                      0x00B0          // Video pixel buffer threshold
#define VOODOO3_IO_VIDCHROMAKEYMIN                        0x00B4          // Video chroma key minimum
#define VOODOO3_IO_VIDCHROMAKEYMAX                        0x00B8          // Video chroma key maximum
#define VOODOO3_IO_VIDSTATUSCURRENTLINE                   0x00BC          // Video status current line
#define VOODOO3_IO_VIDSCREENSIZE                          0x00C0          // Video screen size
#define VOODOO3_IO_VIDOVERLAYSTARTCOORD                   0x00C4          // Video overlay start coordinate
#define VOODOO3_IO_VIDOVERLAYENDSCREEN                    0x00C8          // Video overlay end screen
#define VOODOO3_IO_VIDOVERLAYDUDX                         0x00CC          // Video overlay dU/dX
#define VOODOO3_IO_VIDOVERLAYDUDXOFFSETSCRWIDTH           0x00D0          // Video overlay dU/dX offset screen width
#define VOODOO3_IO_VIDOVERLAYDVDY                         0x00D4          // Video overlay dV/dY
#define VOODOO3_IO_VIDOVERLAYDVDYOFFSET                   0x00D8          // Video overlay dV/dY offset
#define VOODOO3_IO_VIDDESKTOPSTARTADDR                    0x00DC          // Video desktop start address
#define VOODOO3_IO_VIDDESKTOPOVERLAYSTRIDE                0x00E0          // Video desktop overlay stride
#define VOODOO3_IO_VIDINADDR0                             0x00E4          // Video input address 0
#define VOODOO3_IO_VIDINADDR1                             0x00E8          // Video input address 1
#define VOODOO3_IO_VIDINADDR2                             0x00EC          // Video input address 2
#define VOODOO3_IO_VIDINSTRIDE                            0x00F0          // Video input stride
#define VOODOO3_IO_VIDCURROVERLAYSTARTADDR                0x00F4          // Video current overlay start address

// 2D Registers
#define VOODOO3_IO_2D_STATUS                              0x0100          // 2D status register
#define VOODOO3_IO_2D_INTRCTRL                            0x0104          // 2D interrupt control
#define VOODOO3_IO_2D_CLIP0MIN                            0x0108          // 2D clip 0 minimum
#define VOODOO3_IO_2D_CLIP0MAX                            0x010C          // 2D clip 0 maximum
#define VOODOO3_IO_2D_CLIP1MIN                            0x0110          // 2D clip 1 minimum
#define VOODOO3_IO_2D_CLIP1MAX                            0x0114          // 2D clip 1 maximum
#define VOODOO3_IO_2D_DSTBASEADDR                         0x0118          // 2D destination base address
#define VOODOO3_IO_2D_DSTFORMAT                          0x011C          // 2D destination format
#define VOODOO3_IO_2D_SRCCOLORKEYMIN                      0x0120          // 2D source color key minimum
#define VOODOO3_IO_2D_SRCCOLORKEYMAX                      0x0124          // 2D source color key maximum
#define VOODOO3_IO_2D_DSTCOLORKEYMIN                      0x0128          // 2D destination color key minimum
#define VOODOO3_IO_2D_DSTCOLORKEYMAX                      0x012C          // 2D destination color key maximum
#define VOODOO3_IO_2D_BRESERROR0                          0x0130          // 2D Bresenham error 0
#define VOODOO3_IO_2D_BRESERROR1                          0x0134          // 2D Bresenham error 1
#define VOODOO3_IO_2D_ROP                                 0x0138          // 2D ROP
#define VOODOO3_IO_2D_SRCBASEADDR                         0x013C          // 2D source base address
#define VOODOO3_IO_2D_COMMANDEXTRA                        0x0140          // 2D command extra
#define VOODOO3_IO_2D_LINESTIPPLE                         0x0144          // 2D line stipple
#define VOODOO3_IO_2D_LINESTYLE                           0x0148          // 2D line style
#define VOODOO3_IO_2D_PATTERN0ALIAS                       0x014C          // 2D pattern 0 alias
#define VOODOO3_IO_2D_PATTERN1ALIAS                       0x0150          // 2D pattern 1 alias
#define VOODOO3_IO_2D_SRCFORMAT                          0x0154          // 2D source format
#define VOODOO3_IO_2D_SRCSIZE                             0x0158          // 2D source size
#define VOODOO3_IO_2D_SRCXY                               0x015C          // 2D source X/Y
#define VOODOO3_IO_2D_COLORBACK                           0x0160          // 2D color back
#define VOODOO3_IO_2D_COLORFORE                           0x0164          // 2D color fore
#define VOODOO3_IO_2D_DSTSIZE                             0x0168          // 2D destination size
#define VOODOO3_IO_2D_DSTXY                               0x016C          // 2D destination X/Y
#define VOODOO3_IO_2D_COMMAND                             0x0170          // 2D command
#define VOODOO3_IO_2D_LAUNCHAREA                          0x0174          // 2D launch area
#define VOODOO3_IO_2D_COLORPATTERN                        0x0178          // 2D color pattern

// 3D Registers (FBI - Frame Buffer Interface)
// Note: 3D registers use a different addressing scheme with chip field encoding
// Base address for FBI (chip 0) is typically at offset 0x0200+
#define VOODOO3_IO_3D_STATUS                              0x0200          // 3D status register
#define VOODOO3_IO_3D_INTRCTRL                            0x0204          // 3D interrupt control

// VGA Register Offsets (relative to ioBaseAddr)
// These are accessed at ioBaseAddr + (VGA_ADDR - 0x0300)
#define VOODOO3_VGA_MISC_OUTPUT                           0x00C2          // VGA Miscellaneous Output (0x03C2 -> 0x00C2)
#define VOODOO3_VGA_FEATURE_CONTROL                       0x00C0          // VGA Feature Control (0x03C0 -> 0x00C0)
#define VOODOO3_VGA_INPUT_STATUS_0                        0x00C2          // VGA Input Status 0 (0x03C2 -> 0x00C2)
#define VOODOO3_VGA_INPUT_STATUS_1                        0x00DA          // VGA Input Status 1 (0x03DA -> 0x00DA)

// Legacy Enable Registers (not through PCI18)
#define VOODOO3_LEGACY_MOTHERBOARD_ENABLE                0x03C3          // Motherboard enable
#define VOODOO3_LEGACY_ADAPTER_ENABLE                     0x46E8          // Adapter enable
#define VOODOO3_LEGACY_SUBSYSTEM_ENABLE                  0x0102          // Subsystem enable

//
// Register Bit Definitions
//

// MISCINIT1 register bits (from strapping options)
#define VOODOO3_MISCINIT1_DEVSEL_TIMING_SHIFT            24              // DEVSEL timing (0=medium, 1=fast)
#define VOODOO3_MISCINIT1_BIOS_SIZE_SHIFT                25              // BIOS size (0=32K, 1=64K)
#define VOODOO3_MISCINIT1_PCI_66MHZ_SHIFT                26              // PCI 66 MHz (0=33MHz, 1=66MHz)
#define VOODOO3_MISCINIT1_AGP_ENABLE_SHIFT               27              // AGP enable (0=disable, 1=enable)
#define VOODOO3_MISCINIT1_PCI_DEVICE_TYPE_SHIFT          28              // PCI device type (0=VGA, 1=Other Multimedia)
#define VOODOO3_MISCINIT1_SGRAM_COUNT_SHIFT              29              // SGRAM/SDRAM count (0=4 chips, 1=8 chips)
#define VOODOO3_MISCINIT1_SGRAM_SIZE_SHIFT               30              // SGRAM/SDRAM size (0=8Mbit, 1=16Mbit)

// LFBMEMORYCONFIG register bits
#define VOODOO3_LFBMEMORYCONFIG_MEMORY_SIZE_MASK         0x0000000F      // Memory size mask
#define VOODOO3_LFBMEMORYCONFIG_MEMORY_SIZE_4MB          0x00000000      // 4MB
#define VOODOO3_LFBMEMORYCONFIG_MEMORY_SIZE_8MB          0x00000001      // 8MB
#define VOODOO3_LFBMEMORYCONFIG_MEMORY_SIZE_16MB         0x00000002      // 16MB
#define VOODOO3_LFBMEMORYCONFIG_MEMORY_SIZE_32MB         0x00000003      // 32MB

// Status register bits
#define VOODOO3_STATUS_CMD_BUSY                           0x00000001      // Command busy
#define VOODOO3_STATUS_CMD_FIFO_EMPTY                     0x00000002      // Command FIFO empty
#define VOODOO3_STATUS_CMD_FIFO_FULL                      0x00000004      // Command FIFO full
#define VOODOO3_STATUS_CMD_FIFO_NOT_EMPTY                 0x00000008      // Command FIFO not empty

// PCI Configuration Register Offsets
#define VOODOO3_PCI_CFG_DEVICE_VENDOR_ID                 0x0000          // Device/Vendor ID
#define VOODOO3_PCI_CFG_STATUS_COMMAND                   0x0004          // Status/Command
#define VOODOO3_PCI_CFG_CLASS_REVISION                    0x0008          // Class Code/Revision ID
#define VOODOO3_PCI_CFG_BIST                             0x000C          // BIST
#define VOODOO3_PCI_CFG_MEM_BASE_ADDR0                   0x0010          // Memory Base Address 0 (BAR0)
#define VOODOO3_PCI_CFG_MEM_BASE_ADDR1                   0x0014          // Memory Base Address 1 (BAR1)
#define VOODOO3_PCI_CFG_IO_BASE_ADDR                     0x0018          // I/O Base Address (BAR2/PCI18)
#define VOODOO3_PCI_CFG_SUBSYSTEM_ID                     0x002C          // Subsystem ID
#define VOODOO3_PCI_CFG_EXPANSION_ROM                    0x0030          // Expansion ROM Base Address
#define VOODOO3_PCI_CFG_CAPABILITIES_PTR                 0x0034          // Capabilities Pointer
#define VOODOO3_PCI_CFG_INTERRUPT                        0x003C          // Interrupt Register
#define VOODOO3_PCI_CFG_FAB_ID                           0x0040          // Fab ID
#define VOODOO3_PCI_CFG_CFGSTATUS                        0x004C          // Configuration Status
#define VOODOO3_PCI_CFG_CFGSCRATCH                       0x0050          // Configuration Scratch
#define VOODOO3_PCI_CFG_AGP_CAP_ID                       0x0054          // AGP Capability ID Register
#define VOODOO3_PCI_CFG_AGP_STATUS                       0x0058          // AGP Status
#define VOODOO3_PCI_CFG_AGP_COMMAND                      0x005C          // AGP Command
#define VOODOO3_PCI_CFG_ACPI_CAP_ID                      0x0060          // ACPI Capability ID Register
#define VOODOO3_PCI_CFG_ACPI_CMD_STATUS                  0x0064          // ACPI Command/Status

