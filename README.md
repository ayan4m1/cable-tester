# USB/TRRS Cable Tester

This project is a cable tester designed for keyboard cable makers.

## Features

The board features the following connections on both the device and host side:

* USB-A
* USB-B
* USB-B Micro
* USB-B Mini
* USB-C
* TRRS
* 4-pin DuPont header for LEMO/GX16 connector

It can detect the following fault conditions:

* Open circuit (no continuity)
* Short circuit

## Usage

1. Power the device by connecting a USB cable to the connector at the top of the board. The indicator LEDs should appear blue.
2. Connect a cable to both the host and device sides of the board.
3. Press the test button. The indicator LEDs will appear yellow during the test.
4. Use the table below to interpret the test results.

## Results

**Green** means there is continuity and there were no shorts detected.

**Orange** means no continuity was detected.

**Red** means a short was detected.
