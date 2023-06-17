#include "dsps_mul_platform.h"
#include "esp_opt.h"

#define x1_addr   a2
#define x2_addr   a3
#define y_addr    a4
#define len       a5
#define addr_msk  a6

#define x1_r      f0
#define x2_r      f1
#define y_r       f2

#define bit_msk   a11

  .text
  .align  ALIGNMENT
  .global dsps_mul_f32_esp
  .type   dsps_mul_f32_esp,@function

dsps_mul_f32_esp:
// x1       - a2
// x2       - a3
// output   - a4
// len      - a5

    entry	a1, 16
#if (!FAST_MODE)
  loopgtz len, return_success
    lsi x1_r, x1_addr, 0
    lsi x2_r, x2_addr, 0
    mul.s y_r, x1_r, x2_r
    ssi y_r, y_addr, 0

    addi x1_addr, x1_addr, 4
    addi x2_addr, x2_addr, 4
    addi y_addr, y_addr, 4

#else
  movi.n bit_msk, 0b11
  and    bit_msk, bit_msk, len
  srli   len, len, 2
  beqz   bit_msk, loop_begin
  ee.ldf.128.ip f3, f2, f1, f0, x1_addr, 16          // load input
  ee.ldf.128.ip f7, f6, f5, f4, x2_addr, 16          // load input
  mul.s f3, f3, f7
  mul.s f2, f2, f6
  mul.s f1, f1, f5
  mul.s f0, f0, f4
  ee.stf.128.ip f3, f2, f1, f0, y_addr, 16         // store result
loop_begin:
  loopgtz len, return_success
    ee.ldf.128.ip f3, f2, f1, f0, x1_addr, 16          // load input
    ee.ldf.128.ip f7, f6, f5, f4, x2_addr, 16          // load input
    mul.s f3, f3, f7
    mul.s f2, f2, f6
    mul.s f1, f1, f5
    mul.s f0, f0, f4
    ee.stf.128.ip f3, f2, f1, f0, y_addr, 16         // store result
#endif
return_success:
  movi.n	x1_addr, 0  //
  retw.n              // return status ESP_OK