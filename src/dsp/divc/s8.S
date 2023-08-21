#include "dsps_addc_platform.h"
#include "esp_opt.h"

#define in_addr   a2
#define out_addr  a3
#define len       a4
#define C         a5
#define step_in   a6
#define step_out  a7
#define in_r      a8
#define out_r     a9

  .text
  .align  ALIGNMENT
  .global dsps_divc_s8_esp
  .type   dsps_divc_s8_esp,@function

dsps_divc_s8_esp: 
// input    - a2
// output   - a3
// len      - a4
// C        - a5
// step_in  - a6
// step_out - a7

  entry	a1, 16
  loopgtz len, return_success
    l8ui in_r, in_addr, 0            // Load next data
    sext in_r, in_r, 7               // sign extend data
    quos out_r, in_r, C              // Store the multiplication
    s8i  out_r, out_addr, 0          // Store result

    add out_addr, out_addr, step_out // next output;
    add in_addr, in_addr, step_in    // next input;
return_success:
  movi.n	in_addr, 0  //
  retw.n              // return status ESP_OK