/* 
    GPUPlay
    Copyright © 2025 frostbite3000

    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)

    r128_ref.h: Rage128 hardware register definitions
    Based on RRG-G04500-C RAGE 128 PRO Register Reference Guide Rev 1.01
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>

//
// General Memory Sizes
//
#define R128_MMIO_SIZE                                 0x4000          // 16KB MMIO space (BAR2)
#define R128_VRAM_SIZE_8MB                             0x800000        // 8MB
#define R128_VRAM_SIZE_16MB                            0x1000000       // 16MB
#define R128_VRAM_SIZE_32MB                            0x2000000       // 32MB

//
// PCI Configuration Registers (mirrored in MMIO)
//
#define R128_PCI_VENDOR_ID                             0x1002          // ATI Technologies
#define R128_PCI_DEVICE_ID_RAGE128_PRO_PF             0x5046          // Rage 128 Pro (385 BGA, AGP 4X, TMDS, PCI)
#define R128_PCI_DEVICE_ID_RAGE128_PRO_PR             0x5052          // Rage 128 Pro (329 BGA, AGP 4x TMDS)

// PCI Config Space offsets (also accessible via MMIO at 0x0F00+)
#define R128_PCI_CFG_VENDOR_ID                        0x0000
#define R128_PCI_CFG_DEVICE_ID                        0x0002
#define R128_PCI_CFG_COMMAND                          0x0004
#define R128_PCI_CFG_STATUS                           0x0006
#define R128_PCI_CFG_REVISION_ID                      0x0008
#define R128_PCI_CFG_REGPROG_INF                      0x0009
#define R128_PCI_CFG_SUB_CLASS                        0x000A
#define R128_PCI_CFG_BASE_CODE                        0x000B
#define R128_PCI_CFG_CACHE_LINE                       0x000C
#define R128_PCI_CFG_LATENCY                          0x000D
#define R128_PCI_CFG_HEADER                           0x000E
#define R128_PCI_CFG_BIST                             0x000F
#define R128_PCI_CFG_MEM_BASE                         0x0010          // Memory Base Address (BAR0)
#define R128_PCI_CFG_IO_BASE                          0x0014          // I/O Base Address
#define R128_PCI_CFG_REG_BASE                         0x0018          // Register Base Address (BAR1)
#define R128_PCI_CFG_ADAPTER_ID                       0x002C
#define R128_PCI_CFG_BIOS_ROM                         0x0030
#define R128_PCI_CFG_CAPABILITIES_PTR                 0x0034
#define R128_PCI_CFG_INTERRUPT_LINE                   0x003C
#define R128_PCI_CFG_INTERRUPT_PIN                    0x003D
#define R128_PCI_CFG_MIN_GRANT                        0x003E
#define R128_PCI_CFG_MAX_LATENCY                      0x003F

// PCI Command Register bits
#define R128_PCI_COMMAND_IO_ACCESS_EN                 0
#define R128_PCI_COMMAND_MEM_ACCESS_EN               1
#define R128_PCI_COMMAND_BUS_MASTER_EN                2

//
// MMIO Register Base Addresses
//
// Configuration Registers (MMR = Memory Mapped Register)
#define R128_MMR_BASE                                 0x0000          // Base of MMIO space
#define R128_MMR_PCI_MIRROR                           0x0F00          // PCI config space mirror

// Configuration Control Registers
#define R128_CONFIG_CNTL                              0x00E0
#define R128_CONFIG_XSTRAP                            0x00E4          // External Straps
#define R128_CONFIG_BONDS                             0x00E8
#define R128_CONFIG_MEMSIZE                           0x00F8          // Memory size configuration

// Aperture Configuration
#define R128_CONFIG_APER_0_BASE                      0x0100         // Aperture 0 base address
#define R128_CONFIG_APER_1_BASE                      0x0104         // Aperture 1 base address

// BIOS Scratch Pad Registers
#define R128_BIOS_0_SCRATCH                          0x0010
#define R128_BIOS_1_SCRATCH                          0x0014
#define R128_BIOS_2_SCRATCH                          0x0018
#define R128_BIOS_3_SCRATCH                          0x001C

// Clock Control Registers
#define R128_CLOCK_CNTL_INDEX                        0x0008
#define R128_CLOCK_CNTL_DATA                         0x000C

// Bus Control
#define R128_BUS_CNTL                                0x0030

// Memory Control Registers
#define R128_MEM_VGA_WP_SEL                          0x0038          // VGA Write Plane Select
#define R128_MEM_VGA_RP_SEL                          0x003C          // VGA Read Plane Select
#define R128_MEM_CNTL                                0x0140          // Memory Control
#define R128_EXT_MEM_CNTL                            0x0144          // Extended Memory Control
#define R128_MEM_ADDR_CONFIG                         0x0148          // Memory Address Configuration
#define R128_MEM_INTF_CNTL                           0x014C          // Memory Interface Control
#define R128_MEM_STR_CNTL                            0x0150          // Memory Stretch Control
#define R128_MEM_INIT_LAT_TIMER                      0x0154          // Memory Initialization Latency Timer

// CRTC Registers
#define R128_CRTC_GEN_CNTL                           0x0050          // CRTC General Control
#define R128_CRTC_EXT_CNTL                           0x0054          // CRTC Extended Control
#define R128_CRTC_STATUS                              0x005C          // CRTC Status
#define R128_CRTC_H_TOTAL_DISP                       0x0200          // Horizontal Total/Display
#define R128_CRTC_H_SYNC_STRT_WID                    0x0204          // Horizontal Sync Start/Width
#define R128_CRTC_V_TOTAL_DISP                       0x0208          // Vertical Total/Display
#define R128_CRTC_V_SYNC_STRT_WID                    0x020C          // Vertical Sync Start/Width
#define R128_CRTC_VLINE_CRNT_VLINE                   0x0210          // Current Vertical Line
#define R128_CRTC_DEBUG                               0x021C          // CRTC Debug
#define R128_CRTC_GUI_TRIG_VLINE                     0x0218          // GUI Trigger Vertical Line
#define R128_CRTC_OFFSET                              0x0224          // CRTC Offset
#define R128_CRTC_OFFSET_CNTL                        0x0228          // CRTC Offset Control
#define R128_CRTC_PITCH                               0x022C          // CRTC Pitch

// DAC Registers
#define R128_DAC_CNTL                                0x0058          // DAC Control
#define R128_DAC_CRC_SIG                             0x02CC          // DAC CRC Signature

// Hardware Cursor Registers
#define R128_CUR_OFFSET                              0x0260          // Cursor Offset
#define R128_CUR_HORZ_VERT_POSN                      0x0264          // Cursor Horizontal/Vertical Position
#define R128_CUR_HORZ_VERT_OFF                       0x0268          // Cursor Horizontal/Vertical Offset
#define R128_CUR_CLR0                                0x026C          // Cursor Color 0
#define R128_CUR_CLR1                                0x0270          // Cursor Color 1

// Overscan Registers
#define R128_OVR_CLR                                 0x0230          // Overscan Color
#define R128_OVR_WID_LEFT_RIGHT                      0x0234          // Overscan Width Left/Right
#define R128_OVR_WID_TOP_BOTTOM                      0x0238          // Overscan Width Top/Bottom

// DDA (Digital Differential Analyzer) Registers
#define R128_DDA_CONFIG                               0x02E0
#define R128_DDA_ON_OFF                               0x02E4
#define R128_VGA_DDA_CONFIG                          0x02E8
#define R128_VGA_DDA_ON_OFF                          0x02EC

// 2D GUI Engine Registers
#define R128_DST_OFFSET                              0x1404          // Destination Offset
#define R128_DST_PITCH                               0x1408          // Destination Pitch
#define R128_DST_PITCH_OFFSET                        0x142C          // Destination Pitch/Offset
#define R128_DST_X                                   0x141C          // Destination X
#define R128_DST_Y                                   0x1420          // Destination Y
#define R128_DST_X_Y                                 0x1594          // Destination X/Y
#define R128_DST_WIDTH                               0x140C          // Destination Width
#define R128_DST_HEIGHT                              0x1410          // Destination Height
#define R128_DST_HEIGHT_WIDTH                        0x143C          // Destination Height/Width
#define R128_DST_WIDTH_HEIGHT                        0x1598          // Destination Width/Height
#define R128_DST_HEIGHT_WIDTH_8                      0x158C          // Destination Height/Width (8-bit)
#define R128_DST_HEIGHT_Y                            0x15A0          // Destination Height/Y
#define R128_DST_WIDTH_X                             0x1588          // Destination Width/X
#define R128_DST_WIDTH_X_INCY                        0x159C          // Destination Width/X Increment Y
#define R128_DST_BRES_LNTH                           0x1634          // Bresenham Line Length
#define R128_DST_BRES_INC                            0x162C          // Bresenham Increment
#define R128_DST_BRES_ERR                            0x1628          // Bresenham Error
#define R128_DST_BRES_DEC                            0x1630          // Bresenham Decrement
#define R128_DST_X_SUB                               0x15A4          // Destination X Sub-pixel
#define R128_DST_Y_SUB                               0x15A8          // Destination Y Sub-pixel

// Test and Debug Registers
#define R128_TEST_DEBUG_CNTL                         0x0120          // Test Debug Control
#define R128_TEST_DEBUG_MUX                          0x0124          // Test Debug Multiplexer
#define R128_HW_DEBUG                                0x0128          // Hardware Debug

//
// CONFIG_MEMSIZE register bits
//
#define R128_CONFIG_MEMSIZE_MASK                      0x0000000F
#define R128_CONFIG_MEMSIZE_8MB                       0x00000000
#define R128_CONFIG_MEMSIZE_16MB                      0x00000001
#define R128_CONFIG_MEMSIZE_32MB                      0x00000002

//
// CRTC_GEN_CNTL register bits
//
#define R128_CRTC_GEN_CNTL_DISP_REQ_ENB               0
#define R128_CRTC_GEN_CNTL_VGA_OVERLAY_ENB           1
#define R128_CRTC_GEN_CNTL_VGA_OVERLAY_MODE          2
#define R128_CRTC_GEN_CNTL_VGA_MEMORY_DIS            3
#define R128_CRTC_GEN_CNTL_CRTC_DISP_REQ_ENB         4
#define R128_CRTC_GEN_CNTL_CRTC_ON                   5
#define R128_CRTC_GEN_CNTL_CRTC_DBL_SCAN_ENB         6
#define R128_CRTC_GEN_CNTL_CRTC_INTERLACE_ENB        7
#define R128_CRTC_GEN_CNTL_CRTC_HSYNC_DIS            8
#define R128_CRTC_GEN_CNTL_CRTC_VSYNC_DIS            9
#define R128_CRTC_GEN_CNTL_CRTC_CSYNC_ENB             10
#define R128_CRTC_GEN_CNTL_CRTC_VSYNC_POL            11
#define R128_CRTC_GEN_CNTL_CRTC_HSYNC_POL            12
#define R128_CRTC_GEN_CNTL_CRTC_BLANK_DATA_ENB       13
#define R128_CRTC_GEN_CNTL_CRTC_ON_OFF_REQ_ENB       14
#define R128_CRTC_GEN_CNTL_CRTC_TEST_ENB             15
#define R128_CRTC_GEN_CNTL_CRTC_INV_PCLK             16
#define R128_CRTC_GEN_CNTL_CRTC_PIX_WIDTH             17
#define R128_CRTC_GEN_CNTL_CRTC_DISP_REQ_ENB2        18
#define R128_CRTC_GEN_CNTL_CRTC_HSYNC_DIS2           19
#define R128_CRTC_GEN_CNTL_CRTC_VSYNC_DIS2           20
#define R128_CRTC_GEN_CNTL_CRTC_DISP_REQ_ENB3        21
#define R128_CRTC_GEN_CNTL_CRTC_HSYNC_DIS3           22
#define R128_CRTC_GEN_CNTL_CRTC_VSYNC_DIS3           23
#define R128_CRTC_GEN_CNTL_CRTC_DISP_REQ_ENB4        24
#define R128_CRTC_GEN_CNTL_CRTC_HSYNC_DIS4           25
#define R128_CRTC_GEN_CNTL_CRTC_HSYNC_DIS5           26
#define R128_CRTC_GEN_CNTL_CRTC_VSYNC_DIS5           27
#define R128_CRTC_GEN_CNTL_CRTC_DISP_REQ_ENB6        28
#define R128_CRTC_GEN_CNTL_CRTC_HSYNC_DIS6           29
#define R128_CRTC_GEN_CNTL_CRTC_VSYNC_DIS6           30
#define R128_CRTC_GEN_CNTL_CRTC_DISP_REQ_ENB7        31

//
// BUS_CNTL register bits
//
#define R128_BUS_CNTL_BUS_MASTER_DIS                 0
#define R128_BUS_CNTL_BUS_MASTER_REQ_DIS             1
#define R128_BUS_CNTL_BUS_MASTER_GNT_DIS              2
#define R128_BUS_CNTL_BUS_MASTER_ENB                  3
#define R128_BUS_CNTL_BUS_MASTER_REQ_ENB              4
#define R128_BUS_CNTL_BUS_MASTER_GNT_ENB              5
#define R128_BUS_CNTL_BUS_MASTER_EXT_ENB              6
#define R128_BUS_CNTL_BUS_MASTER_EXT_REQ_ENB          7
#define R128_BUS_CNTL_BUS_MASTER_EXT_GNT_ENB          8
#define R128_BUS_CNTL_BUS_MASTER_INT_ENB              9
#define R128_BUS_CNTL_BUS_MASTER_INT_REQ_ENB          10
#define R128_BUS_CNTL_BUS_MASTER_INT_GNT_ENB          11
#define R128_BUS_CNTL_BUS_MASTER_EXT_DIS               12
#define R128_BUS_CNTL_BUS_MASTER_INT_DIS               13
#define R128_BUS_CNTL_BUS_MASTER_EXT_REQ_DIS           14
#define R128_BUS_CNTL_BUS_MASTER_INT_REQ_DIS           15
#define R128_BUS_CNTL_BUS_MASTER_EXT_GNT_DIS           16
#define R128_BUS_CNTL_BUS_MASTER_INT_GNT_DIS           17
#define R128_BUS_CNTL_BUS_MASTER_EXT_ENB2              18
#define R128_BUS_CNTL_BUS_MASTER_EXT_REQ_ENB2          19
#define R128_BUS_CNTL_BUS_MASTER_EXT_GNT_ENB2          20
#define R128_BUS_CNTL_BUS_MASTER_INT_ENB2              21
#define R128_BUS_CNTL_BUS_MASTER_INT_REQ_ENQ2          22
#define R128_BUS_CNTL_BUS_MASTER_INT_GNT_ENB2          23
#define R128_BUS_CNTL_BUS_MASTER_EXT_DIS2              24
#define R128_BUS_CNTL_BUS_MASTER_INT_DIS2              25
#define R128_BUS_CNTL_BUS_MASTER_EXT_REQ_DIS2          26
#define R128_BUS_CNTL_BUS_MASTER_INT_REQ_DIS2          27
#define R128_BUS_CNTL_BUS_MASTER_EXT_GNT_DIS2          28
#define R128_BUS_CNTL_BUS_MASTER_INT_GNT_DIS2          29
#define R128_BUS_CNTL_BUS_MASTER_EXT_ENB3               30
#define R128_BUS_CNTL_BUS_MASTER_EXT_REQ_ENB3          31

//
// MEM_CNTL register bits
//
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_ENB          0
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_DIS          1
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_REQ_ENB      2
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_REQ_DIS      3
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_GNT_ENB      4
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_GNT_DIS      5
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_ENB      6
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_DIS      7
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_ENB      8
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_DIS      9
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_REQ_ENB   10
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_REQ_DIS  11
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_REQ_ENB   12
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_REQ_DIS  13
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_GNT_ENB   14
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_GNT_DIS  15
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_GNT_ENB   16
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_GNT_DIS  17
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_ENB2      18
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_DIS2     19
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_ENB2      20
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_DIS2     21
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_REQ_ENB2  22
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_REQ_DIS2 23
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_REQ_ENB2  24
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_REQ_DIS2 25
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_GNT_ENB2  26
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_GNT_DIS2 27
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_GNT_ENB2  28
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_INT_GNT_DIS2 29
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_ENB3      30
#define R128_MEM_CNTL_MEM_INIT_LAT_TIMER_EXT_DIS3     31

