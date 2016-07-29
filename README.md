# ARM Global Intern Innovation Challenge 2016
## Team: Gotta Cache 'Em All
Members: Dale Whinham, Gaetan Bahl, Alex Gilday

Our team uses the theme of Pokemon to make a lesson on networking and security be both educational and fun for children. Our lesson begins with simple introduction to various methods of communication, serial and wireless radio, then expands further into security, introducing encryption with checksums as a method of countering potential man-in-the-middle and similar packet-sniffing attack vectors.

## The Source Code

Our source is split into two parts: firmware for the BBC Micro:bit and an Android app. The firmware for the BBC Micro:bit includes serial communication via the USB port, radio communication using the Bluetooth Low Energy adapter, and encryption keys generated by gestures using the accelerometer. It also has a "packet-sniffing" mode for demonstrating the capture of the radio "packets."

The Android app is a custom app built to provide easier access to the serial communication on an Android phone without having to setup a serial terminal connection manually. It automatically uses the correct baud rate, parity, and other options, and sends the serial data with the correct formatting (single carriage-return terminator, '!' prefix to instruct the Micro:bit to encrypt the message). An apk of the app in including in the Releases page.

## Progress Blog

We made a blog that follows our progress during development of our demonstration and other resources: http://alexgilday.co.uk/armiic/

## Useful links 

- Python docs for micro bit http://microbit-micropython.readthedocs.io/en/latest/
- Some kind of microbit IDE https://github.com/ntoll/mu
- Flashing tool http://uflash.readthedocs.io/en/latest/
