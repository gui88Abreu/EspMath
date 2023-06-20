# EspMath

This library is intended to provide valuable tools to perform math operations such as arrays multiplication, convolution, etc, on ESP32 devices. Most of the operations uses **DSP** (Digital Signal Processing) instructions to accelerate the procedure.

Take a look at [Examples](examples/test_bench/) to learn how to use it.

## DSP acceleration

At [DSP](src/dsp/), you can find all the dsp accelerated operations. All implementations provide two versions of them. The first one implements a soft acceleration that can work with all contiguous memories. The second one is a faster implementation that makes use of the Processor Instruction Extensions (PIE) to accelerate operations and works only with 16 bytes aligned memory. The second implementation is enabled by default and can be disabled using the macro MEMORY_ALIGN at [configuration](src/esp_opt.h).

Until now, these libraries are guaranteed to work only on ESP32-S3 devices. To use them, just include [esp_dsp](src/esp_dsp.h).

## Array Class

The array class allows us to provide multiple features to perform the essential operations for an array type. Please read its documentation alongside the code at [Array](src/esp_array.h) for more information.

## ANSI C version

An ANSI C version of some operations is also provided, so you can compare their performances. It can be compiled on any machine and it doesn't use any acceleration method or tool. They can be found at [ANSI](src/ansi.h).

## Random Number Generation

At [EspRand](src/esp_rand.h), you can find a simple implementation to ease the generation of random numbers on esp32 devices. The implementation makes use of the random number generator implemented by Espressif, which uses the RF module to generate true random numbers.
