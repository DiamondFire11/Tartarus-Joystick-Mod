# Introducing the Tartarus Joystick Mod!

Looking to enhance your control and precision on Razer Tartarus V2/Pro left-handed keyboards? This DIY project offers a custom PCB that replaces the D-pad with a responsive joystick, built around the powerful ARM Cortex-M0 (Atmel SAMD) microcontroller.

## Getting Started

This mod requires some hands-on assembly. We'll provide the necessary build files, but you'll need to order and assemble the PCB yourself using the included GERBER, schematics, and Draftsman files.

The project utilizes an Atmel SAMD21E18 microcontroller (MCU) to connect the joystick to your computer. To program the MCU with the bootloader, you'll need an SWD programmer. The Adafruit Trinket M0 bootloader is recommended for this purpose, allowing you to easily configure the PCB as a custom Arduino board.

**We strongly recommend utilizing solder paste stencils (available as .GTP and .GBP GERBER files) for this project.** Soldering the small SMD components can be challenging for beginners. Recommended stencil manufacturers can be found in the [Resources](## Resources) section.

## Planned Changes

- **EEPROM Integration:** To enhance user experience, we plan to incorporate EEPROM memory for non-volatile storage of calibration settings.
- **ZIF Connector for Joy-Con Compatibility:** We're exploring the possibility of replacing the included joystick with a ZIF connector, enabling seamless integration with Nintendo Joy-Con sticks.

## Uploading Custom Firmware

For uploading custom code, we recommend using an IDE with PlatformIO installed. The standard Arduino IDE won't work due to the PCB's unique pin layout. Luckily, PlatformIO board and variant files are included in the project repository for your convenience.

## A Note on Uploading

In some instances, the SAM-BA programming software might encounter issues writing all firmware blocks to the MCU. This can happen if both the USB data pins (used for serial communication) and the programming interface try to access the device simultaneously, causing the MCU to crash. If this occurs, simply double-press the reset button to force a reboot and enter bootloader mode. Then, retry uploading the code.

## Important Reminders

- Please refrain from modifying the PlatformIO variant files, as this can prevent the code from compiling correctly.
- This project doesn't support the standard Arduino IDE. If you seek support, we'll kindly recommend switching to PlatformIO.

## Join the Community!

We welcome contributions to this project! If you have significant hardware modifications in mind, please create an issue first to discuss the proposed changes. Additionally, remember to update any relevant software tests along with your contributions.

## Resources
#### Hardware
[Board Manufacturing - PCBWay](https://www.pcbway.com)

[Components - Mouser](https://www.mouser.com)

[OSH Stencils](https://www.oshstencils.com)

#### Software and Programming
[SWD Programmer - Atmel ICE](https://www.mouser.com/ProductDetail/Microchip-Technology/ATATMEL-ICE?qs=KLFHFgXTQiDAUrt43H15kQ%3D%3D)

[PlatformIO](https://platformio.org)

## License

This project is licensed under the GPLv3 ([GPL](https://choosealicense.com/licenses/gpl-3.0/)).
