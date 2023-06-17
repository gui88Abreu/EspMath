#include "dsps_mulc_platform.h"
#include "esp_opt.h"

#define in_addr   a2
#define out_addr  a3
#define len       a4
#define C         a5
#define addr_msk  a7
#define bit_msk   a11

#define in_v      q1
#define out_v     q2

    .text
    .align  ALIGNMENT
    .global dsps_mulc_s32_esp
    .type   dsps_mulc_s32_esp,@function

dsps_mulc_s32_esp: 
// input    - a2
// output   - a3
// len      - a4
// C        - a5

  entry	a1, 16

#if (!FAST_MODE)
  loopgtz len, return_success
    l32i a8, in_addr, 0          // Load next data
    mull a9, a8, C               // Store the multiplication in the a9
    s32i a9, out_addr, 0         // Store result in the output memory

    addi out_addr, out_addr, 4   // next output;
    addi in_addr, in_addr, 4     // next input;

#else
  movi.n bit_msk, 0b11
  and    bit_msk, bit_msk, len
  srli   len, len, 2
  beqz   bit_msk, loop_begin
  ee.vld.128.ip in_v, in_addr, 16      // load input
  ee.movi.32.a in_v, a8, 0             // Move first operand
  ee.movi.32.a in_v, a9, 1             // Move second operand
  ee.movi.32.a in_v, a10, 2            // Move third operand
  ee.movi.32.a in_v, a11, 3            // Move fourth operand
  mull a8, a8, C                       // Store the multiplication in the a8
  mull a9, a9, C                       // Store the multiplication in the a9
  mull a10, a10, C                     // Store the multiplication in the a10
  mull a11, a11, C                     // Store the multiplication in the a11
  ee.movi.32.q out_v, a8, 0            // Move result of the first multiplication
  ee.movi.32.q out_v, a9, 1            // Move result of the second multiplication
  ee.movi.32.q out_v, a10, 2           // Move result of the third multiplication
  ee.movi.32.q out_v, a11, 3           // Move result of the fourth multiplication
  ee.vst.128.ip out_v, out_addr, 16    // store results
loop_begin:
  loopgtz len, return_success
    ee.vld.128.ip in_v, in_addr, 16      // load input
    ee.movi.32.a in_v, a8, 0             // Move first operand
    ee.movi.32.a in_v, a9, 1             // Move second operand
    ee.movi.32.a in_v, a10, 2            // Move third operand
    ee.movi.32.a in_v, a11, 3            // Move fourth operand
    mull a8, a8, C                       // Store the multiplication in the a8
    mull a9, a9, C                       // Store the multiplication in the a9
    mull a10, a10, C                     // Store the multiplication in the a10
    mull a11, a11, C                     // Store the multiplication in the a11
    ee.movi.32.q out_v, a8, 0            // Move result of the first multiplication
    ee.movi.32.q out_v, a9, 1            // Move result of the second multiplication
    ee.movi.32.q out_v, a10, 2           // Move result of the third multiplication
    ee.movi.32.q out_v, a11, 3           // Move result of the fourth multiplication
    ee.vst.128.ip out_v, out_addr, 16    // store results
#endif
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK