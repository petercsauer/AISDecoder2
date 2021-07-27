# AIS Decoder | V2
## Peter Sauer and Josh Mehlman | Marine Advanced Robotics | July 2021

## Setup
### Compile C Code
- cd templats
- make

### Run Decoder
- ./commsEx

## What it Does
- Continuously scans the serial port (/dev/ttyUSB0) and converts the data to an AISData object which is then read by the UDP
