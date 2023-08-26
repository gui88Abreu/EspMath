#include "esp_opt.h"

#define x1_addr   a2
#define x2_addr   a3
#define y_addr    a4
#define len       a5
#define step_x1   a6
#define step_x2   a7
#define frac      a8
#define acc       a9

#define x1_r      a10
#define x2_r      a11
#define y_r       a12

#define unligned  a6
#define bit_msk   a7
#define x1_v      q0
#define x2_v      q1
#define y_v       q2
#define acc_v     q3


  .text
  .align  ALIGNMENT
  .global dsps_dotp_s16_esp
  .type   dsps_dotp_s16_esp,@function

dsps_dotp_s16_esp: 
// x1       - a2
// x2       - a3
// y        - a4
// len      - a5
// step_x1  - a6
// step_x2  - a7
// frac     - a8 (stack)

  entry	 sp, 16
  l32i   frac, a1, 16
  ssr  frac                     // sar = frac
  movi.n acc, 0
  beqi   step_x1, 1, .L0
.L1:
  slli step_x1, step_x1, 1
  slli step_x2, step_x2, 1
  loopgtz len, .R0
    l16si     x1_r, x1_addr, 0      // load next data
    l16si     x2_r, x2_addr, 0      // load next data
    mul.aa.ll x1_r, x2_r            // Store the multiplication at the acc
    rsr       y_r, acclo            // Read the 32 low bits from the acc
    srl       y_r, y_r              // Shift right (sar), and store in x1_r
    add.n     acc, acc, y_r         //

    add x1_addr, x1_addr, step_x1   // next input;
    add x2_addr, x2_addr, step_x2   // next input;
.L0:
  bgei   step_x2, 2, .L1
  blti   len, 8, .L1
  movi.n bit_msk, 0b111                // select 3 least significant bits
  and    unligned, len, bit_msk
  srli   len, len, 3
  
  ee.zero.q acc_v
  loopgtz len, .L2
    ee.vld.128.ip x1_v, x1_addr, 16    // load input
    ee.vld.128.ip x2_v, x2_addr, 16    // load input
    ee.vmul.s16   y_v, x1_v, x2_v      // multiply the input by C
    ee.vadds.s16  acc_v, acc_v, y_v    
.L2:
  ee.movi.32.a  acc_v, y_r, 0
  extui         x1_r, y_r, 16, 16
  sext          x1_r, x1_r, 15
  extui         x2_r, y_r,  0, 16
  sext          x2_r, x2_r, 15
  add.n         acc, acc, x1_r        //
  add.n         acc, acc, x2_r        //

  ee.movi.32.a  acc_v, y_r, 1
  extui         x1_r, y_r, 16, 16
  sext          x1_r, x1_r, 15
  extui         x2_r, y_r,  0, 16
  sext          x2_r, x2_r, 15
  add.n         acc, acc, x1_r        //
  add.n         acc, acc, x2_r        //

  ee.movi.32.a  acc_v, y_r, 2
  ee.src.q      acc_v, acc_v, y_v
  extui         x1_r, y_r, 16, 16
  sext          x1_r, x1_r, 15
  extui         x2_r, y_r,  0, 16
  sext          x2_r, x2_r, 15
  add.n         acc, acc, x1_r        //
  add.n         acc, acc, x2_r        //

  ee.movi.32.a  acc_v, y_r, 3
  ee.src.q      acc_v, acc_v, y_v
  extui         x1_r, y_r, 16, 16
  sext          x1_r, x1_r, 15
  extui         x2_r, y_r,  0, 16
  sext          x2_r, x2_r, 15
  add.n         acc, acc, x1_r        //
  add.n         acc, acc, x2_r        //
  
  loopgtz unligned, .R0
    l16si x1_r, x1_addr, 0      // load next data
    l16si x2_r, x2_addr, 0      // load next data
    mul.aa.ll x1_r, x2_r        // Store the multiplication at the acc
    rsr y_r, acclo              // Read the 32 low bits from the acc
    srl y_r, y_r                // Shift right (sar), and store in x1_r
    add.n acc, acc, y_r         //

    addi x1_addr, x1_addr, 2    // next input;
    addi x2_addr, x2_addr, 2    // next input;
.R0:
  s16i acc, y_addr, 0            // Store result in the output memory
  movi.n	  a2, 0                //
  retw.n                         // return status ESP_OK