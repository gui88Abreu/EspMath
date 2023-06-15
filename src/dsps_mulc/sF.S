#include "dsps_mulc_platform.h"
#include "esp_opt.h"

#define in_addr   a2
#define out_addr  a3
#define len       a4
#define C         f4
#define addr_msk  a6
#define bit_msk   a7

  .text
  .align  ALIGNMENT
  .global dsps_mulc_f32_esp
  .type   dsps_mulc_f32_esp,@function

dsps_mulc_f32_esp:
// input    - a2
// output   - a3
// len      - a4
// C        - a5

    entry	a1, 16
    
    wfr		C, a5		// a5 - load to the f0
#if (!FAST_MODE)

  movi  bit_msk,  0xFF
  movi  addr_msk, 0xF
decider:
  blti len, 1, return_success
  and   a8, in_addr, addr_msk
  beqz  a8, mulc_128_bits
  beqi  a8, 0b1000, mulc_64_bits

mulc_32_bits:
  lsi f1, in_addr, 0
  mul.s f1, f1, C
  ssi f1, out_addr, 0

  addi in_addr, in_addr, 4
  addi out_addr, out_addr, 4
  addi len, len, -1

  j decider

mulc_64_bits:
  blti len, 2, mulc_32_bits
  ee.ldf.64.ip f1, f0, in_addr, 8          // load input
  mul.s f1, f1, C
  mul.s f0, f0, C
  ee.stf.64.ip f1, f0, out_addr, 8         // store result
  addi len, len, -2

  j decider

mulc_128_bits:
  blti len, 4, mulc_64_bits
  ee.ldf.128.ip f3, f2, f1, f0, in_addr, 16          // load input
  mul.s f3, f3, C
  mul.s f2, f2, C
  mul.s f1, f1, C
  mul.s f0, f0, C
  ee.stf.128.ip f3, f2, f1, f0, out_addr, 16         // store result
  addi len, len, -4

  j decider

#else
  movi.n bit_msk, 0b11
  and    bit_msk, bit_msk, len
  srli   len, len, 2
  beqz   bit_msk, loop_begin
  ee.ldf.128.ip f3, f2, f1, f0, in_addr, 16          // load input
  mul.s f3, f3, C
  mul.s f2, f2, C
  mul.s f1, f1, C
  mul.s f0, f0, C
  ee.stf.128.ip f3, f2, f1, f0, out_addr, 16         // store result
loop_begin:
  loopgtz len, return_success
    ee.ldf.128.ip f3, f2, f1, f0, in_addr, 16          // load input
    mul.s f3, f3, C
    mul.s f2, f2, C
    mul.s f1, f1, C
    mul.s f0, f0, C
    ee.stf.128.ip f3, f2, f1, f0, out_addr, 16         // store result
#endif
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK