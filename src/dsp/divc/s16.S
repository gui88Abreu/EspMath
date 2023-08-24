#include "dsps_addc_platform.h"
#include "esp_opt.h"

#define in_addr   a2
#define out_addr  a3
#define len       a4
#define C         a5
#define step_in   a6
#define step_out  a7
#define frac      a8 // from stack

#define sel_p     0

  .text
  .align  ALIGNMENT
  .global dsps_divc_s16_esp
  .type   dsps_divc_s16_esp,@function

dsps_divc_s16_esp: 
// input    - a2
// output   - a3
// len      - a4
// C        - a5
// step_in  - a6
// step_out - a7
// frac     - a8 (from stack)

  entry	sp, 16
  l32i frac, a1, 16
  ssl     frac                 // sar = frac

  slli step_in, step_in, 1
  slli step_out, step_out, 1
  loopgtz len, return_success
    l16si a8, in_addr, 0             // load next data
    sll  a8, a8                      // Shift left (sar), and store in a8
    quos a8, a8, C                   // Store the multiplication at a8
    s16i a8, out_addr, 0             // Store result in the output memory
    add out_addr, out_addr, step_out // next output;
    add in_addr, in_addr, step_in    // next input;
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK