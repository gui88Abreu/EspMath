#include "esp_opt.h"

#define x1_addr   a2
#define x2_addr   a3
#define y_addr    a4
#define len       a5
#define step_x1   a6
#define step_x2   a7
#define aux       a8

#define x1_r      f0
#define x2_r      f1
#define acc0      f0
#define acc1      f1
#define acc2      f2
#define acc3      f3

  .text
  .align  ALIGNMENT
  .global dsps_dotp_f32_esp
  .type   dsps_dotp_f32_esp,@function

dsps_dotp_f32_esp: 
// x1       - a2
// x2       - a3
// y        - a4
// len      - a5
// step_x1  - a6
// step_x2  - a7

  entry	 sp, 16
  
  movi.n aux, 0
  wfr   acc0, aux        // acc0 = 0;
  
  bgei   step_x1, 2, .L1 // step_x >= 2
  bgei   step_x2, 2, .L1 // step_x >= 2
  
	wfr	    acc1, aux        // = 0;
	wfr	    acc2, aux        // = 0;
	wfr	    acc3, aux        // = 0;
  srli   aux, len, 2       // aux = len / 4
  loopgtz aux, .L0
    ee.ldf.128.ip f7, f6, f5, f4, x1_addr, 16   // load input
    ee.ldf.128.ip f11, f10, f9, f8, x2_addr, 16 // load input
    
    madd.s  acc0, f4, f8
    madd.s  acc1, f5, f9
    madd.s  acc2, f6, f10
    madd.s  acc3, f7, f11
.L0:
  extui  len,   len,    0, 2                  // len = len % 4
  add.s acc1,  acc0, acc1
	add.s acc3,  acc2, acc3
	add.s acc0,  acc1, acc3
.L1:
  slli step_x1, step_x1, 2
  slli step_x2, step_x2, 2
  loopgtz len, .R0
    lsi    x1_r, x1_addr, 0           // load next data
    add.n x1_addr, x1_addr, step_x1   // next input;

    lsi    x2_r, x2_addr, 0           // load next data
    add.n x2_addr, x2_addr, step_x2   // next input;
    
    madd.s acc0, x1_r, x2_r           // add to acc
.R0:
  ssi    acc0, y_addr, 0             // Store result in the output memory
  movi.n	 a2, 0                     //
  retw.n                             // return status ESP_OK