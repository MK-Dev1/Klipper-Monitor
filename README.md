## Klipper-Monitor

Work in Progress  

Klipper-Monitor is a compact hardware display for monitoring 3D printers running Klipper.  

Built around an STM32 microcontroller and a 2.4" ILI9341 TFT display, it provides a simple way to keep track of your printer's status at a glance.  
&nbsp;  

**Right now it doesnt heve integration with klipper and only displays example hard coded data**

**Features:**  
- STM32-based hardware
- 2.4" ILI9341 TFT display
- Klipper printer status monitoring
- Currently supports Raspberry Pi-based Klipper setups
- Designed with toolchanger 3D printers in mind  
&nbsp;  

**Target Printer**  
Klipper-Monitor is being tested on E3NG printer.  

However, the project is not limited to E3NG. The firmware and interface can be adapted for any 3D printer that meets the hardware and software requirements.  
To sat the logo that is displayed while waiting for printer to start copy your logo to /img folder, run change_logo.py  
and select your logo.  
On default there is no logo  
&nbsp;  

**Compatibility**  
Klipper-Monitor is compatible with any raspberry pi based 3d printer running on klipper firmware  
It supports heated and not heated beds and multiple [up to 5] nozzles [6 if bed is not heated]  
&nbsp;  

**Project Status**  
This project is under active development. Hardware, firmware and features are still evolving, so things may change.  
More documentation and features will be added as development progresses.  
&nbsp;  

License  
GPL 3.0
