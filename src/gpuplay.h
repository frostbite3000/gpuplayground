/* 
    GPUPlay
    Copyright © 2025 frostbite3000

    Raw GPU programming for Other GPUs
    Licensed under the MIT license (see license file)

    gpuplay.h: Central include that everything uses.
*/

#pragma once
#include <bios.h>
#include <ctype.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/nearptr.h>
#include <sys/farptr.h>

// #pragma once my beloved
#include <util/util.h>

/* Version information */

#define APP_NAME "GPUPlay"
#define APP_DESCRIPTION "Raw GPU programming for Other GPUs\n"
#define APP_VERSION "0.5.0.0 (pre-release)"
#define APP_COMPILE_TIME __DATE__ " " __TIME__

#define APP_VERSION_FULL APP_VERSION " (" APP_COMPILE_TIME ")\n" APP_DESCRIPTION

#define APP_SIGNON_STRING APP_NAME " Version " APP_VERSION_FULL

/* Core */

// The help string
extern const char* msg_help; 
void GPUPlay_ShowHelpAndExit();

// String
#define STRING_EMPTY ""

/* PCI */

/* PCI BIOS magic */
#define PCI_BIOS_MAGIC						0x20494350

/* PCI Type-0 header. We don't care about PCI to PCI bridges (type 1) or PCI to CardBus bridges (type 2) */
#define PCI_CFG_OFFSET_VENDOR_ID			0x00
#define PCI_CFG_OFFSET_DEVICE_ID			0x02
#define PCI_CFG_OFFSET_COMMAND				0x04

#define PCI_CFG_OFFSET_COMMAND_IO_ENABLED	0x01
#define PCI_CFG_OFFSET_COMMAND_MEM_ENABLED	0x02
#define PCI_CFG_OFFSET_COMMAND_BUS_MASTER	0x04	// Allow bus mastering

#define PCI_CFG_OFFSET_STATUS				0x06
#define PCI_CFG_OFFSET_REVISION				0x08
#define PCI_CFG_OFFSET_CLASS_CODE			0x09	
#define PCI_CFG_OFFSET_CLASS_CODE_HIGH 		0x09
#define PCI_CFG_OFFSET_CLASS_CODE_LOW		0x0A
#define PCI_CFG_OFFSET_CACHE_LINE_SIZE		0x0C
#define PCI_CFG_OFFSET_LATENCY_TIMER		0x0D
#define PCI_CFG_OFFSET_HEADER_TYPE			0x0E
#define PCI_CFG_OFFSET_BIST					0x0F

/* BARs. Only 0/1 matter for supported NV cards and 0/1/2 for any NV cards but for PCI dumping code we dump everything 

Well, I guess they would all be used for 64-bit mapping. But we only care about 32-bit
*/
#define PCI_CFG_OFFSET_BAR0					0x10	// Main GPU MMIO
#define PCI_CFG_OFFSET_BAR1					0x14	// VRAM + RAMIN (on nv3), otherwise just dumb framebuffer
#define PCI_CFG_OFFSET_BAR2					0x18	// NV20+
#define PCI_CFG_OFFSET_BAR3					0x1C	// For dumping purposes
#define PCI_CFG_OFFSET_BAR4					0x20	// For dumping purposes
#define PCI_CFG_OFFSET_BAR5					0x24	// For dumping purposes

#define PCI_CFG_OFFSET_CARDBUS_CIS_PTR		0x28
#define PCI_CFG_OFFSET_SUBSYSTEM_VENDOR_ID	0x2A
#define PCI_CFG_OFFSET_SUBSYSTEM_ID			0x2C
#define PCI_CFG_OFFSET_EXPANSION_ROM_BASE	0x30
#define PCI_CFG_OFFSET_CAPABILITIES_PTR 	0x34
#define PCI_CFG_OFFSET_INTERRUPT_LINE		0x39
#define PCI_CFG_OFFSET_INTERRUPT_PIN		0x3A
#define PCI_CFG_OFFSET_MINIMUM_GRANT		0x3B
#define PCI_CFG_OFFSET_MAXIMUM_LATENCY		0x3C

/* TODO: AGP SHIT! */

/* PCI Structures & Enums */
typedef enum 
{
	// Int 0x1A,AX=0xB1xx   = PCI
	PCI_FUNCTION_ID_BASE = 0xB1,

	// PCI BIOS SUBFUNCTIONS
	PCI_BIOS_PRESENT = 0x01,
	PCI_FIND_DEVICE = 0x02,
	PCI_FIND_CLASS_CODE = 0x03,
	PCI_GENERATE_SPECIAL_CYCLE = 0x06,
	PCI_READ_CONFIG_BYTE = 0x08,
	PCI_READ_CONFIG_WORD = 0x09,
	PCI_READ_CONFIG_DWORD = 0x0A,
	PCI_WRITE_CONFIG_BYTE = 0x0B,
	PCI_WRITE_CONFIG_WORD = 0x0C,
	PCI_WRITE_CONFIG_DWORD = 0x0D,
	PCI_GET_IRQ_ROUTING_OPTIONS = 0x0E,
	PCI_SET_PCI_IRQ = 0x0F,

	CFLAG_CARRY = 0x01,
} pci_functions_t;

typedef enum 
{
	PCI_ERROR_UNSUPPORTED_FUNCTION = 0x81,
	PCI_ERROR_BAD_VENDOR_ID = 0x83,
	PCI_ERROR_DEVICE_NOT_FOUND = 0x86,
	PCI_ERROR_BAD_PCI_REGISTER = 0x87,
} pci_errors_t; 

/* PCI Functions */
bool PCI_BiosIsPresent(void);
bool PCI_DevicePresent(uint32_t device_id, uint32_t vendor_id);

uint8_t PCI_ReadConfig8(uint32_t bus_number, uint32_t function_number, uint32_t offset);
uint16_t PCI_ReadConfig16(uint32_t bus_number, uint32_t function_number, uint32_t offset);
uint32_t PCI_ReadConfig32(uint32_t bus_number, uint32_t function_number, uint32_t offset);

bool PCI_WriteConfig8(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint8_t value);
bool PCI_WriteConfig16(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint16_t value);
bool PCI_WriteConfig32(uint32_t bus_number, uint32_t function_number, uint32_t offset, uint32_t value);

#define INT_VIDEO					0x10
#define INT_PCI_BIOS        		0x1A		// PCI BIOS interrupt 

/* Generic definition for all tests, used to prevent expanding the nv_test static initialisers */
#define PCI_VENDOR_GENERIC			0x0000
#define PCI_DEVICE_GENERIC			0x0000		

/* Device definitions */
#define PCI_VENDOR_ATI              0x1002      // ATI Technologies (Rage128, etc.)

// ATI Rage128
#define PCI_DEVICE_RAGE128_PRO_PF	0x5046		// Rage 128 Pro (385 BGA, AGP 4X, TMDS, PCI)	1999
#define PCI_DEVICE_RAGE128_PRO_PR	0x5052		// Rage 128 Pro (329 BGA, AGP 4x TMDS)		1999

// 3Dfx Interactive
#define PCI_VENDOR_3DFX             0x121A      // 3Dfx Interactive, Inc.

// 3Dfx Voodoo3/Banshee
#define PCI_DEVICE_VOODOO3          0x0005      // Voodoo3
#define PCI_DEVICE_BANSHEE          0x0003      // Voodoo Banshee


/* Other Device Definition */
typedef struct nv_device_info_s
{
	uint32_t device_id; 								// Device ID of the GPU
	uint32_t vendor_id;									// Vendor ID of the GPU
	const char* name; 									// Friendly name of the GPU
	bool (*init_function)();							// Function to call on entry point	
	void (*shutdown_function)();						// Function to call on shutdown
	bool (*gpus_section_applies)(uint32_t fourcc);		// Does this GPUS section apply for this GPU?
	bool (*gpus_section_parse)(uint32_t fourcc, FILE* stream);		// Parse a specific GPUS section
} nv_device_info_t; 

/* List of supported devices */
extern nv_device_info_t supported_devices[]; 

/* Full NV Device Struct (shared across all devices) */
typedef struct nv_device_s
{
	nv_device_info_t device_info;
	uint32_t bus_number;			// PCI bus number
	uint32_t function_number; 		// PCI function number
	void* bar0;						// PCI BAR0 mapping - gpu stuff
	void* bar1;						// PCI BAR1 mapping for DFB
	int32_t bar0_selector;			// MUST BE USED FOR ACCESS TO BAR0
	int32_t bar1_selector;			// MUST BE USED FOR ACCESS TO BAR1
	uint32_t bar1_dfb_start;		// DFB start address
	uint32_t ramin_start; 		// RAMIN start address

	uint32_t vram_amount;			// Amount of Video RAM

	/* Some registers shared between all gpus */
	uint32_t nv_pfb_boot_0;			// nv_pfb_boot_0 register read at boot
	uint32_t nv_pmc_boot_0;			// nv_pmc_boot_0 register read at boot
	uint32_t straps;				// Straps for oem-specific config
	double crystal_hz;				// Clock crystal base (TODO: fully refactor so this is not needed)

	uint32_t mpll;					// [NV1+] Core Clock [NV4+] Memory Clock
	uint32_t vpll;					// [NV1+] Video Clock
	uint32_t nvpll;					// [NV4+] Core Clock
} nv_device_t;

extern nv_device_t current_device;

// Detection functions
bool GPU_Detect(); 


//
// READ/WRITE functions for GPU memory areas
//

// only 8 and 32 bit are really needed
uint8_t mmio_read8(uint32_t offset); 
uint32_t mmio_read32(uint32_t offset); 
void mmio_write8(uint32_t offset, uint8_t val);
void mmio_write32(uint32_t offset, uint32_t val);

/* Requires some special dispensations if the bus size is 64-bit and there is only 2 MB of VRAM */
uint8_t nv_dfb_read8(uint32_t offset); 
uint16_t nv_dfb_read16(uint32_t offset); 
uint32_t nv_dfb_read32(uint32_t offset); 
void nv_dfb_write8(uint32_t offset, uint8_t val);
void nv_dfb_write16(uint32_t offset, uint16_t val);
void nv_dfb_write32(uint32_t offset, uint32_t val);


// Clock

#define NV_CLOCK_BASE_13500K				13500000.0
#define NV_CLOCK_BASE_14318180				14318180.0

double nv_clock_mnp_to_mhz(uint32_t clock_base, uint32_t mnp);

//
// VGA common stuff
// (Can be used on all gpus theoretically)
//

#define VGA_PORT_ATTRIBUTE_REGISTER				0x3C0		// This doesn't use index
#define VGA_PORT_ATTRIBUTE_DATA_WRITE			0x3C1
#define VGA_PORT_MISCOUT						0x3C2
#define VGA_PORT_SEQUENCER_INDEX				0x3C4
#define VGA_PORT_SEQUENCER						0x3C5
#define VGA_PORT_GRAPHICS_INDEX					0x3CE
#define VGA_PORT_GRAPHICS						0x3CF
#define VGA_PORT_MONO_CRTC_INDEX				0x3B4
#define VGA_PORT_MONO_CRTC						0x3B5
#define VGA_PORT_COLOR_CRTC_INDEX				0x3D4
#define VGA_PORT_COLOR_CRTC						0x3D5
#define VGA_PORT_INPUT0_MONO					0x3BA
#define VGA_PORT_INPUT0_COLOR					0x3DA

#define VGA_REALMODE_VBIOS_LOCATION				0xC0000

uint8_t vga_crtc_read(uint8_t index);
uint8_t vga_gdc_read(uint8_t index);
uint8_t vga_sequencer_read(uint8_t index);
uint8_t vga_attribute_read(uint8_t index);
void vga_crtc_write(uint8_t index, uint8_t value);
void vga_gdc_write(uint8_t index, uint8_t value);
void vga_sequencer_write(uint8_t index, uint8_t value);
void vga_attribute_write(uint8_t index, uint8_t value);

//
// SCRIPT PARSER
//

void Script_Run();
void Script_RunCommand(char* line_buf);

// This sucks. It's not a proper lexer/tokeniser, but we don't need one
typedef struct gpu_script_command_s
{
	const char* name_abbrev;
	const char* name_full;		// don't really need an alias system
	bool (*function)();
	uint32_t num_parameters;		// for parameter size checking
} gpu_script_command_t; 

extern gpu_script_command_t commands[];

/* Command utility stuff */
const char* Command_Argv(uint32_t argv);
uint32_t Command_Argc();

//
// SAVESTATES
//

#define GPUS_MAGIC				0x53555047	// 'GPUS'
#define GPUS_VERSION			1

#define GPUS_SECTIONS_MAX		32			// Sanity check heuristic; Maximum reasonable number of sections for a GPUS fine

typedef struct gpus_header_s
{
	uint32_t magic; 
	uint16_t version;			// Just in case...
	uint16_t num_sections;		// Number of GPUS file sections
	uint32_t device_id;
} gpus_header_t; 

typedef struct gpus_header_section_s
{
	uint32_t fourcc;
	uint32_t offset;
	uint32_t size; 
} gpus_header_section_t;

// Section names (little endian):
// CRTC registers				'CRTC'
// GDC registers				'VGAG'
// Sequencer registers			'VGAS'
// Attribute registers			'VGAA'
// MMIO							'MMIO'
// BAR1 (VRAM / RAMIN)			'BAR1'
// On-die Texture Cache			'CACH'
// EEPROM (nv1 only)			'NV1E'

typedef enum gpus_sections_e
{
	gpus_section_vga_crtc = 0x43545243,

	gpus_section_vga_gdc = 0x47414756,

	gpus_section_vga_sequencer = 0x53414756,

	gpus_section_vga_attribute = 0x41414756,

	gpus_section_mmio = 0x4F494D4D,

	gpus_section_bar1 = 0x31524142,

	gpus_section_cache = 0x48434143,

	gpus_section_nv1e = 0x44453136,
} gpus_sections; 

// GPUS functions
bool GPUS_Load();

/* REPL stuff */

void GPURepl_Run(); 