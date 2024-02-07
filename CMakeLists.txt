set(COMPONENT_SRCS
src/esp_array.cpp
src/esp_fixed_point.cpp
src/dsp/add/s8.S
src/dsp/add/s16.S
src/dsp/add/s32.S
src/dsp/add/sF.S
src/dsp/fixed/float2fixed.S
src/dsp/fixed/fixed2float.S
src/dsp/subc/s8.S
src/dsp/subc/s16.S
src/dsp/subc/s32.S
src/dsp/subc/sF.S
src/dsp/mul/s8.S
src/dsp/mul/s16.S
src/dsp/mul/s32.S
src/dsp/mul/sF.S
src/dsp/div/s8.S
src/dsp/div/s16.S
src/dsp/div/s32.S
src/dsp/div/sF.S
src/dsp/sub/s8.S
src/dsp/sub/s16.S
src/dsp/sub/s32.S
src/dsp/sub/sF.S
src/dsp/divc/s32I.S
src/dsp/divc/s8.S
src/dsp/divc/s8I.S
src/dsp/divc/s16.S
src/dsp/divc/s32.S
src/dsp/divc/s16I.S
src/dsp/divc/sF.S
src/dsp/divc/sFI.S
src/dsp/mulc/s8.S
src/dsp/mulc/s16.S
src/dsp/mulc/s32.S
src/dsp/mulc/sF.S
src/dsp/dopP/s16.S
src/dsp/dopP/sF.S
src/dsp/addc/s8.S
src/dsp/addc/s16.S
src/dsp/addc/s32.S
src/dsp/addc/sF.S
src/dsp/sum/s8.S
src/dsp/sum/s16.S
src/dsp/sum/s32.S
)

set(COMPONENT_LIBRARIES
src/
)

idf_component_register(
  SRCS ${COMPONENT_SRCS}
  INCLUDE_DIRS ${COMPONENT_LIBRARIES}
  REQUIRES esp-dsp arduino-esp32
)