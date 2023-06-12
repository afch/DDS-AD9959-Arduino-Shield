# DDS-AD9959-Arduino-Shield
4-Channels DDS (Direct Digital Synthesis) Analog Devices AD9959 Arduino Shield by GRA &amp; AFCH

Web-site: https://gra-afch.com
Direct link to category:  https://gra-afch.com/product-category/rf-units/

# GA_Flasher or xLoader can be used to upload compiled firmware from "Firmware Compiled (.HEX File)" folder: https://github.com/afch/GA_Flasher

"Firmware Compiled (.HEX File)" - folder contains pre-Compiled firmware (.HEX File), that can be uploaded via "AvrDude", GA_Flasher or any other software for flashing Atmega 2560

"Firmware Source (.INO File)" - folder contains source of firmware that can be compiled (modified if necessary) and uploaded via Arduino IDE: https://www.arduino.cc/en/Main/Software

"Libraries" - contains libraries that are only needed for compilation: place them to "C:\Users\[USER]\Documents\Arduino\libraries", where [USER] - your windows username.

This AD9959 Shield can be easily connected to Arduino Mega without additional wires and converters. All functions of the DDS AD9959 are brought to the contacts of the Arduino Mega thanks to this you can fully reveal all the capabilities of the DDS AD9959.

Key Benefits:

* Low harmonics no more than -60dB. An output RF transformer is used for the correct operation of the current mirror.
* balancing transformer for TCXO and REF CLK IN options
* Small spur
* 4 layer board. Signal lines TOP and Bottom, inner layers Ground and Power.
* Low Noise LDO Stabilizers
* Separate power supply for all analog and digital lines (1.8 and 3.3V), 5pcs IC voltage stabilizers are used. Additionally, there is an RF Ferrite bead interchange.
* High-speed decoupling Level converter and TTL 5V matching
* one of these types of clock source can be used/installed:

1. XO - Crystal 25 Mhz 20ppm internal oscillator with PLL,
2. TCXO - 5 - 50 Mhz 1ppm external oscillator PLLz,
3. REF CLK IN - external generator 400 MHz to 600 MHz


* Easy to connect OLED display.
* Convenient and fast control with an encoder
* The synthesizer is capable to generate sine wave.
* The software allows you to select and configure the frequency of the clock generator through the user menu (without the need to recompile the program).
* Any settings can be stored in non-volatile EEPROM memory (located at Arduino Mega).
* Basic settings are applied and saved automatically.
* This shield support overclocking the AD9959 core up to 600 MHz.
* DDS AD9959 Shield has ability to generate a signal up to 225 MHz (to suppress harmonics, it is recommended to overclock the AD9959 for frequencies above 200 MHz).

# Switching a clock source is made by next components:
<pre>
|-------------------------------|-----------------------------------|-----------------|
|      Clock source             |            Capacitors             |    Resistors    |
|   (only one at a time)        | C20  |  C22  |  C18,C19 | C14,C17 |  XTAL | REF_CLK |
|-------------------------------------------------------------------------------------|
| XO - Crystal Oscillator (Z1)  |  -   |   -   |    V     |    X    |   V   |    X    |
| TCXO - Oscillator 1ppm (Z2)   |  V   |   X   |    X     |    V    |   X   |    V    |
| REF CLK - External Generator  |  X   |   X   |    X     |    V    |   X   |    V    |
|-------------------------------------------------------------------------------------|
</pre>

Where V means that the component must be installed (soldered), and X - means that the component must be removed

# List of Serial Port Commands:
Starting with version 1.02, the ability to control via the serial port has been added.

  C — Set the current output Channel: (0 — 3)  
  F — Sets Frequency in Hz (100000 — 225000000)  
  A — Sets the power (Amplitude) level of the selected channel in dBm (-60 — -7)  
  P — Sets the Phase of the selected channel in dBm (0 — 360)  
  M — Gets Model  
  V — Gets Firmware Version  
  h — This Help  
  ; — Commands Separator  
  
  Example:  
  C0;F100000;A-10  
  Sets the Frequency to 100 kHz, and Output Power (Amplitude) to -10 dBm on Channel 0 (RF OUT0).  
  Any number of commands in any order is allowed, but the very first command must be "C"  

# Serial Port Settings:

  Speed - 115200 Bouds  
  Data Bits - 8  
  Stop Bits - 1  
  Parity - No  
  DTR - OFF  
# Windows:

An example of setting up a serial port in the Windows console:
  <pre>
  mode COM3 baud=115200 DTR=OFF Data=8
  </pre>
  
Usage example:
  <pre>
  echo F100000000 > COM3
  </pre>
# Ubuntu 22.04:

An example of setting up a serial port in the Ubuntu:
  <pre>
  sudo usermod -aG dialout $USER_NAME$
  sudo chmod a+rw /dev/ttyUSB0
  sudo stty -F /dev/ttyUSB0 115200 cs8 ixoff -hupcl -echo
  </pre>
  
Usage example:
  <pre>
  echo "F100000000" > /dev/ttyUSB0
  </pre>
