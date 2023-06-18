#include "dsps_addc_platform.h"
#include "esp_opt.h"

#define in_addr   a2
#define out_addr  a3
#define len       a4
#define C         f4
#define addr_msk  a6
#define bit_msk   a7

  .text
  .align  ALIGNMENT
  .global dsps_csub_f32_esp
  .type   dsps_csub_f32_esp,@function

dsps_csub_f32_esp:
// input    - a2
// output   - a3
// len      - a4
// C        - a5

    entry	a1, 16
    
    wfr		C, a5		// a5 - load to the f0
#if (!FAST_MODE)
  loopgtz len, return_success
    lsi f1, in_addr, 0
    sub.s f1, C, f1
    ssi f1, out_addr, 0

    addi in_addr, in_addr, 4
    addi out_addr, out_addr, 4

#else
  movi.n bit_msk, 0b11
  and    bit_msk, bit_msk, len
  srli   len, len, 2
  beqz   bit_msk, loop_begin
  ee.ldf.128.ip f3, f2, f1, f0, in_addr, 16          // load input
  sub.s f3, C, f3
  sub.s f2, C, f2
  sub.s f1, C, f1
  sub.s f0, C, f0
  ee.stf.128.ip f3, f2, f1, f0, out_addr, 16         // store result
loop_begin:
  loopgtz len, return_success
    ee.ldf.128.ip f3, f2, f1, f0, in_addr, 16          // load input
    sub.s f3, C, f3
    sub.s f2, C, f2
    sub.s f1, C, f1
    sub.s f0, C, f0
    ee.stf.128.ip f3, f2, f1, f0, out_addr, 16         // store result
#endif
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK