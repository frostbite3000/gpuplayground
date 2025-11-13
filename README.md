# GPUPlay

## A hardware test suite for other GPUs (based on nvplayground)

This is a test suite intended to allow for hardware testing and emulation of Other graphics processing units so that they can be emulated as part of the [Bochs](https://github.com/frostbite3000/) project. It runs under [DJGPP](https://www.delorie.com/djgpp/) in order to obtain exclusive use of the graphics processor's resources without
a Windows or Linux driver interfering while still benefiting from a 32-bit protected mode environment.

The program is currently capable of performing several modes of operation:

* Running a user-defined "main" function (not actually implemented in practice)
* Running any of the tests defined in the `gpuplay.ini` file
* Running a ".nvp" script (GPUScript will be documented when a real interpreter exists for it)

All of this functionality is fairly and is subject to change at any time. Feel free to submit a pull request to contribute to the project or report issues.

## Supported hardware 

### Well-supported

**ATI Rage128**: This has been extensively tested across several units of R128 devices. If you have one, please run this program under it and contribute to the testing effort by submitting your gpuplay.log to me (currently, just contact me on email at frostbite@frostbite3000.net, but a place to submit runs will be worked out eventually).

### Unknown

**Other PCI devices**: This project could hypothetically be extended to any PCI/AGP/PCIe device of interest, not just graphics hardware. Most likely, supporrt for these devices
will only be added if someone wishes to add it, merged into main at my, as the maintainer's discretion. 

## Supported software environments

### Well-supported

**MS-DOS under DJGPP**: This is the intended and gold standard environment for running GPUPlay.

### Some support

**Windows 9x DOS box**: Should work (and may be required for NV1 until the correct initialisation procedure is determined) but be wary of running tests, especially graphics tests, while the drivers are running. Other's drivers, especially their old ones, are infamously temperamental and may not like being interfered with.

### Not supported

**Windows NT (NTVDM)**: The PCI BIOS is not emulated, so until if or when the PCI bus is manually enumerated, NTVDM will not work.