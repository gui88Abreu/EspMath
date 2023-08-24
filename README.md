# EspMath

This library is intended to provide valuable tools to perform math operations such as arrays multiplication, convolution, etc, on ESP32 devices. ESP32-S3 chips provide **DSP** (Digital Signal Processing) instructions to accelerate the procedure.

Take a look at [Examples](examples/test_bench/) to learn how to use it.

## DSP acceleration

At [DSP](src/dsp/), you can find all the dsp accelerated operations. These functions make use of the Processor Instruction Extensions (PIE) to accelerate operations and work only with 16 bytes aligned memory. The class [Array](src/esp_array.h) already uses them to perform operations.

DSP acceleration targets ESP32-S3 devices and is not available for other chips. To use them, just include [esp_dsp](src/esp_dsp.h).

## Array Class

The array class provides multiple features to perform essential operations for an array type. Please read its documentation alongside the code at [Array](src/esp_array.h) for more information.

## Fixed Point Computation

In this project, you will find many tools to accelerate the computation of fixed-point (16 bits) data such as [fixed](src/esp_fixed_point.h), which was designed to ease fixed-point manipulation. For fixed-point arrays, [Array](src/esp_array.h) and [DSP](src/dsp/) provide multiple features to ease and accelerate the computation as well.

## ANSI C version

An ANSI C version of some operations is also provided, so you can compare their performances. It can be compiled on any machine and it doesn't use any acceleration method or tool. They can be found at [ANSI](src/ansi.h).

## Random Number Generation

At [EspRNG](src/esp_rng.h), you can find a simple implementation to ease the generation of random numbers on esp32 devices. The implementation makes use of the random number generator implemented by Espressif, which uses the RF module to generate true random numbers.
