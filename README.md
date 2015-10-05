AVR Block Shover
================

A program for the ATmega328p for reading color data over multiple i2c color sensors and using that data to shove blocks.

Details
-------

The PCA9547 is an eight-channel i2c multiplexer, used to switch between i2c devices with the same hard-coded address. More info here: [PCA9547 datasheet](http://www.nxp.com/documents/data_sheet/PCA9547.pdf)

The TCS34725 is an i2c CRGB color sensor. The address is fixed, hence the multiplexer. More info here: [TCS34725 datasheet](http://www.adafruit.com/datasheets/TCS34725.pdf)

The ATmega328p is an 8-bit microcontroller made by Atmel. You probably know it as the heart of the Arduino UNO but it is *so much more*. They're really cheap and I have a ton so ask me and I'll give you one or three.

Description
-----------

The block shover is controlled via USART commands. Upon booting, the shover steps through each sensor (the number of which is defined by the *SENSOR_NUMBER* constant in blockshover.h) and initializes them. It then idles until it receives a command over USART.

Commands correspond to colors and are as follows:

1 - Shove Red

2 - Shove Green

3 - Shove Yellow

4 - Shove Blue

A shove command wakes the microcontroller up, reads color data from each sensor, and shoves the blocks that correspond to the selected color. It continues doing this until all sensors report no matching blocks. Following the shove action, the microcontroller goes back to sleep.

What's done
-----------

- communication
- multiple sensor reading
- control via serial
- detecting color values with sensor data

What needs to be done
---------------------

- integrate with hardware
- implement shoving action
- button controls for shoving
- make arms work
- insert to-do thing here

How do I do this myself?
------------------------

If you would like to assist with this, please let me (Will) know. As of now, you will need the following to replicate what is currently available:

- an AVR programming interface (I'm using the [USBtinyISP from Adafruit](http://www.adafruit.com/products/46) with [this kit](http://shop.evilmadscientist.com/tinykitlist/230) but I hear the [AVR Dragon](http://www.atmel.com/tools/AVRDRAGON.aspx) is good as well in addition to being official)
- an ATmega328p (hit me up, I have approx. a billion of them)
- a PCA9547 with header pins (they're surface mount, so get at me and I'll give you one that's breadboard ready)
- one or more TCS34725 (using this neat protoboard version from adafruit [link](http://www.adafruit.com/products/1334))
- a programming environment. I have a book I can share that will walk you through the basics. All platforms (Mac, Linux, Windows) are supported.

If you have no clue what any of that means, it's no problem. Feel free to @ me on slack or hit me up when I'm in the building. I'll help as best I can and provide resources so you can join me in this infinite k-hole that is microcontrollers.

Acknowledgements
----------------

Uses the [USART library](https://github.com/hexagon5un/AVR-Programming) from *Make: AVR Programming* by Elliot Williams and the [I2C-master-lib](https://github.com/devthrash/I2C-master-lib) library by github user devthrash.
