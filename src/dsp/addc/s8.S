#include "dsps_mulc_platform.h"
#include "esp_opt.h"

#define in_addr   a2
#define out_addr  a3
#define len       a4
#define C         a5
#define shift     a6
#define addr_msk  a7
#define bit_msk   a11
#define cnst      a12

#define in_v      q0
#define C_v       q1
#define out_v     q2

  .text
  .align  ALIGNMENT
  .global dsps_addc_s8_esp
  .type   dsps_addc_s8_esp,@function

dsps_addc_s8_esp: 
// input    - a2
// output   - a3
// len      - a4
// C        - a5

  entry	a1, 16
  ee.vldbc.8 C_v, C          // C_v = C
#if (!FAST_MODE)
  l32i cnst, C, 0                // Load const
  loopgtz len, return_success
    l8ui a8, in_addr, 0          // Load next data
    add.n a9, a8, cnst           // Store the multiplication
    s8i a9, out_addr, 0          // Store result

    addi out_addr, out_addr, 1 // next output;
    addi in_addr, in_addr, 1   // next input;
#else
  movi shift, 0
  ssr    shift               // sar = frac
  movi   bit_msk, 0b1111
  and    bit_msk, bit_msk, len
  srli   len, len, 4
  beqz   bit_msk, loop_begin
  ee.vld.128.ip in_v, in_addr, 16           // load input
  ee.vadds.s8 out_v, in_v, C_v              // multiply, shift (sar), and store result
  ee.vst.128.ip out_v, out_addr, 16
loop_begin:
  loopgtz len, return_success
    ee.vld.128.ip in_v, in_addr, 16         // load input
    ee.vadds.s8 out_v, in_v, C_v            // multiply, shift (sar), and store result
    ee.vst.128.ip out_v, out_addr, 16
#endif
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK