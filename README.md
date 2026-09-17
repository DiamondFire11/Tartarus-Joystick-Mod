# Tartarus Analog Joystick Mod | SAMD21 Custom Controller PCB
[![Version: v1.0](https://img.shields.io/badge/Version-v1.0-blueviolet.svg)](https://github.com/DiamondFire11/Tartarus-Joystick-Mod/releases/tag/v1.0-ES)
[![License: GPLv3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)
[![MCU: SAMD21E18](https://img.shields.io/badge/MCU-Atmel%20SAMD21E18-orange.svg)](https://www.microchip.com/en-us/product/ATSAMD21E18)
[![CAD: Altium Designer](https://img.shields.io/badge/CAD-Altium%20Designer-brightgreen.svg)](https://www.altium.com/)
[![Framework: PlatformIO](https://img.shields.io/badge/Framework-PlatformIO-orange.svg)](https://platformio.org/)
[![Language: C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C.svg)](https://en.cppreference.com/w/cpp/17)
[![Interface: USB--HID](https://img.shields.io/badge/Interface-USB--HID-informational.svg)](https://www.usb.org/)

An electro-mechanical replacement PCB for Razer Tartarus V2/Pro keypads, replacing digital directional pads with an analog joystick module. Features a custom 4-layer stackup, hardware signal conditioning, non-blocking C++ state machines, and integrated power regulation.

---

## 📸 PCB CAD Render
![PCB Render](https://github.com/DiamondFire11/Tartarus-Joystick-Mod/blob/main/docs/tartarus-board-view.png)

## 🛠 Engineering Highlights & System Architecture

* **4-Layer Controlled Stackup:** Designed in Altium Designer utilizing a compact 22mm x 30mm board footprint. Stackup topology: `Top (Signal/Parts)` $\rightarrow$ `GND Plane (Solid Reference)` $\rightarrow$ `Power Plane (3.3V LDO)` $\rightarrow$ `Bottom (Signal/Traces)` to control return path loop inductance, improve thermal dissipation, and reduce EMI.
* **Hardware-Level Signal Conditioning:** Integrates a two-stage inverting hardware Schmitt-trigger debouncing circuit paired with a fast-acting Schottky diode pull-down network. Bypasses the resistor on discharge to provide near-zero press latency while maintaining a deterministic ~10 ms RC debouncing filter on switch release.
* **32-Bit Microcontroller Architecture:** Powered by an ATSAMD21E18 32-bit ARM Cortex-M0+ MCU. Utilizes non-CPU-blocking polling, efficient bit-wise masking, and custom C++ modular firmware running on PlatformIO for low-latency execution.
* **Signal & Power Integrity:** Native USB 2.0 Type-C interface with dedicated ESD suppression chokes, controlled impedance differential pair routing, onboard LDO regulation, and top-layer component placement.
---

## 📐 System Technical Specifications

| Parameter | Specification Details                                             |
| :--- |:------------------------------------------------------------------|
| **Board Dimensions** | 22mm x 30mm (Fits native Razer keypad cavity)|
| **Microcontroller** | Microchip/Atmel SAMD21E18 (32-bit ARM Cortex-M0+, 48MHz)|
| **PCB Stackup** | 4-Layer FR-4 (1.6mm thickness, 1oz Cu outer, solid inner planes)|
| **Hardware Debounce** | Asymmetric ~10 ms release delay (Schmitt Trigger + Schottky diode)|
| **Bus Protocols** | USB 2.0 HID Native, SWD Programming Interface, UART Debugging|
| **Toolchain** | Altium Designer, C++17, PlatformIO / GCC ARM, Atmel-ICE|
---

## 💻 Firmware Architecture & Flashing

The C++ firmware uses modular drivers to maintain low execution time and consistent USB polling rates:

```bash
# Clone repository with PlatformIO configuration
git clone https://github.com/DiamondFire11/Tartarus-Joystick-Mod.git
cd Tartarus-Joystick-Mod

# Build firmware target for SAMD21E18
pio run -e samd21e18_tartarus
```
---

## ⚡ Flashing via SWD & Hardware Debugging
1. Connect an Atmel-ICE or SWD-compatible programmer to the target SWD header pads.
2. Flash the board configuration using PlatformIO or Atmel Studio.
3. **Recovery Mode**: If USB SERCOM arbitration triggers a bus fault during firmware execution, double-tap the physical reset pad to force hardware bootloader mode.
--- 

## 🚀 Roadmap & Design Revisions
* [ ] **EEPROM / Flash Emulation**: Non-volatile storage of analog joystick deadband and runtime calibration offsets across power cycles.
* [ ] **Joy-Con ZIF Interface**: Next board iteration replacing the analog pot with a 0.5mm pitch ZIF flex-connector for Nintendo Switch Joy-Con stick modules.
---

## 📄 License & Hardware Manufacturing Files
* **Production Files**: Production Gerber (.gbr), N.C. Drill (.drl), Solder Paste Stencils (.gtp/.gbp), and Altium Draftsman assembly drawings are located in the /hardware directory
* **License**: Distributed under [GPLv3 License](https://github.com/DiamondFire11/Tartarus-Joystick-Mod/blob/main/LICENSE). 
