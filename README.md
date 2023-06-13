# EspMath

This library is intended to provide useful tools to perform math operations such arrays multiplication, convolution, and etc, on ESP32 devices. Most of the operations uses **DSP** (Digital Signal Processing) instructions to accelerate the procedure.

## Array

The array class allows us to provide multiple features to perform the essential operations for an array type. For more information, please, read its documentation alongside the code at [Array](src/esp_array.h).

## ANSI version

An ANSI version of some operations is provided as well, so you can compare their performances. It can be compiled on any machine and it doesn't use any acceleration method or tool.