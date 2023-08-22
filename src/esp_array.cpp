#include "esp_array.h"

#if BENCHMARK_TEST
#include "esp_debug.h" // https://github.com/guilhAbreu/EspDebug
#endif

#if BENCHMARK_TEST
#define REPORT_BENCHMARK(title, func1, ...)\
{\
func1(__VA_ARGS__); /* warm up the cache */ \
unsigned intlevel = dsp_ENTER_CRITICAL(); \
uint32_t func1_start = xthal_get_ccount(); \
func1(__VA_ARGS__); \
uint32_t func1_end = xthal_get_ccount(); \
dsp_EXIT_CRITICAL(intlevel); \
debug.print(title + String(func1_end - func1_start)); \
}
#endif

namespace espmath{
#ifdef CONFIG_IDF_TARGET_ESP32S3
#if CONFIG_IDF_TARGET_ESP32S3

  template<>
  Array<float> Array<float>::conv(Array<float>& kernel)
  {
    shape2D outputShape = shape2D(1, _shape.columns + kernel.shape.columns -1);
    Array<float> convOutput(outputShape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_conv_f32_ae32, _array, _shape.columns, kernel, kernel.shape.columns, convOutput);
  #else
    exec_dsp(dsps_conv_f32_ae32, _array, _shape.columns, kernel, kernel.shape.columns, convOutput);
  #endif
    return convOutput;
  }

  template<>
  Array<float> Array<float>::correlation(Array<float>& pattern)
  {
    Array<float> corr(_shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_corr_f32_ae32, _array, _shape.columns, pattern, pattern.shape.columns, corr);
  #else
    exec_dsp(dsps_corr_f32_ae32, _array, _shape.columns, pattern, pattern.shape.columns, corr);
  #endif
    return corr;
  }

  Array<float> operator+(Array<float>& onearray, Array<float> another)
  {
    Array<float> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_add_f32_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_add_f32_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }

  Array<int32_t> operator+(Array<int32_t>& onearray, Array<int32_t> another)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_add_s32_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_add_s32_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }

  Array<uint32_t> operator+(Array<uint32_t>& onearray, Array<uint32_t> another)
  {
    Array<uint32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ",\
                    dsps_add_s32_esp,\
                    (int32_t*)onearray,\
                    (int32_t*)another,\
                    (int32_t*)newArray,\
                    onearray.shape.columns);
  #else
    exec_dsp(dsps_add_s32_esp,\
            (int32_t*)onearray,\
            (int32_t*)another,\
            (int32_t*)newArray,\
            onearray.shape.columns);
  #endif
    return newArray;
  }

  Array<int16_t> operator+(Array<int16_t>& onearray, Array<int16_t> another)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_add_s16_esp, onearray, another, newArray, onearray.shape.columns, 1, 1, 1, 0);
  #else
    exec_dsp(dsps_add_s16_esp, onearray, another, newArray, onearray.shape.columns, 1, 1, 1, 0);
  #endif
    return newArray;
  }

  Array<int8_t> operator+(Array<int8_t>& onearray, Array<int8_t> another)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_add_s8_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_add_s8_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }

  Array<float> operator+(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_addc_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<int32_t> operator+(Array<int32_t>& onearray, const int32_t value)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_addc_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<uint32_t> operator+(Array<uint32_t>& onearray, const uint32_t value)
  {
    Array<uint32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_addc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<int16_t> operator+(Array<int16_t>& onearray, const int16_t value)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s16_esp, onearray, newArray, newArray.shape.columns, &value, 1, 1, 0);
  #else
    exec_dsp(dsps_addc_s16_esp, onearray, newArray, newArray.shape.columns, &value, 1, 1, 0);
  #endif
    return newArray;
  }

  Array<int8_t> operator+(Array<int8_t>& onearray, const int8_t value)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #else
    exec_dsp(dsps_addc_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #endif
    return newArray;
  }

  Array<float> operator+(const float value, Array<float> onearray)
  {
    Array<float> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_addc_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<int32_t> operator+(const int32_t value, Array<int32_t> onearray)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_addc_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<uint32_t> operator+(const uint32_t value, Array<uint32_t> onearray)
  {
    Array<uint32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_addc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<int16_t> operator+(const int16_t value, Array<int16_t> onearray)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s16_esp, onearray, newArray, newArray.shape.columns, &value, 1, 1, 0);
  #else
    exec_dsp(dsps_addc_s16_esp, onearray, newArray, newArray.shape.columns, &value, 1, 1, 0);
  #endif
    return newArray;
  }

  Array<int8_t> operator+(const int8_t value, Array<int8_t> onearray)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_addc_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #else
    exec_dsp(dsps_addc_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #endif
    return newArray;
  }

  Array<float> operator-(Array<float>& onearray, Array<float> another)
  {
    Array<float> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_sub_f32_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_sub_f32_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }

  Array<int32_t> operator-(Array<int32_t>& onearray, Array<int32_t> another)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_sub_s32_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_sub_s32_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }

  Array<uint32_t> operator-(Array<uint32_t>& onearray, Array<uint32_t> another)
  {
    Array<uint32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ",\
                    dsps_sub_s32_esp,\
                    (int32_t*)onearray,\
                    (int32_t*)another,\
                    (int32_t*)newArray,\
                    onearray.shape.columns);
  #else
    exec_dsp(dsps_sub_s32_esp,\
            (int32_t*)onearray,\
            (int32_t*)another,\
            (int32_t*)newArray,\
            onearray.shape.columns);
  #endif
    return newArray;
  }

  Array<int16_t> operator-(Array<int16_t>& onearray, Array<int16_t> another)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_sub_s16_esp, onearray, another, newArray, onearray.shape.columns, 1, 1, 1, 0);
  #else
    exec_dsp(dsps_sub_s16_esp, onearray, another, newArray, onearray.shape.columns, 1, 1, 1, 0);
  #endif
    return newArray;
  }

  Array<int8_t> operator-(Array<int8_t>& onearray, Array<int8_t> another)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_sub_s8_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_sub_s8_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }

  Array<float> operator-(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("cycles to complete: ", dsps_subc_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_subc_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<int32_t> operator-(Array<int32_t>& onearray, const int32_t value)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("cycles to complete: ", dsps_subc_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_subc_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }
  
  Array<uint32_t> operator-(Array<uint32_t>& onearray, const uint32_t value)
  {
    Array<uint32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("cycles to complete: ", dsps_subc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_subc_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }
  
  Array<int16_t> operator-(Array<int16_t>& onearray, const int16_t value)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("cycles to complete: ", dsps_subc_s16_esp, onearray, newArray, newArray.shape.columns, &value, 1, 1, 0);
  #else
    exec_dsp(dsps_subc_s16_esp, onearray, newArray, newArray.shape.columns, &value, 1 , 1, 0);
  #endif
    return newArray;
  }

  Array<int8_t> operator-(Array<int8_t>& onearray, const int8_t value)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("cycles to complete: ", dsps_subc_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #else
    exec_dsp(dsps_subc_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #endif
    return newArray;
  }

  Array<float> operator-(const float value, Array<float> onearray)
  {
    Array<float> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_csub_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_csub_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<int32_t> operator-(const int32_t value, Array<int32_t> onearray)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_csub_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_csub_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }
  
  Array<uint32_t> operator-(const uint32_t value, Array<uint32_t> onearray)
  {
    Array<uint32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_csub_s32_esp, (int32_t*)onearray, (int32_t*)newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_csub_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<int16_t> operator-(const int16_t value, Array<int16_t> onearray)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_csub_s16_esp, onearray, newArray, newArray.shape.columns, &value, 1, 1, 0);
  #else
    exec_dsp(dsps_csub_s16_esp, onearray, newArray, newArray.shape.columns, &value, 1, 1, 0);
  #endif
    return newArray;
  }
  
  Array<int8_t> operator-(const int8_t value, Array<int8_t> onearray)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_csub_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #else
    exec_dsp(dsps_csub_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #endif
    return newArray;
  }
  
  Array<float> operator*(Array<float>& onearray, Array<float> another)
  {
    Array<float> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mul_f32_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_mul_f32_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }
  
  Array<int32_t> operator*(Array<int32_t>& onearray, Array<int32_t> another)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mul_s32_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_mul_s32_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }
  
  Array<uint32_t> operator*(Array<uint32_t>& onearray, Array<uint32_t> another)
  {
    Array<uint32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ",\
                      dsps_mul_s32_esp,\
                      (int32_t*)onearray,\
                      (int32_t*)another,\
                      (int32_t*)newArray,\
                      onearray.shape.columns);
  #else
    exec_dsp(dsps_mul_s32_esp,\
            (int32_t*)onearray,\
            (int32_t*)another,\
            (int32_t*)newArray,\
            onearray.shape.columns);
  #endif
    return newArray;
  }
  
  Array<int16_t> operator*(Array<int16_t>& onearray, Array<int16_t> another)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mul_s16_esp, onearray, another, newArray, onearray.shape.columns, 1, 1, 1, onearray.frac);
  #else
    exec_dsp(dsps_mul_s16_esp, onearray, another, newArray, onearray.shape.columns, 1, 1, 1, onearray.frac);
  #endif
    return newArray;
  }
  
  Array<int8_t> operator*(Array<int8_t>& onearray, Array<int8_t> another)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mul_s8_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_mul_s8_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }
  
  Array<float> operator*(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_mulc_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }
  
  Array<int32_t> operator*(Array<int32_t>& onearray, const int32_t value)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_mulc_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }
  
  Array<uint32_t> operator*(Array<uint32_t>& onearray, const uint32_t value)
  {
    Array<uint32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ",\
                    dsps_mulc_s32_esp,(int32_t*)onearray,\
                    (int32_t*)newArray,\
                    newArray.shape.columns,\
                    value);
  #else
    exec_dsp(dsps_mulc_s32_esp,(int32_t*)onearray,\
                (int32_t*)newArray,\
                newArray.shape.columns,\
                value);
  #endif
    return newArray;
  }
  
  Array<int16_t> operator*(Array<int16_t>& onearray, const int16_t value)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s16_esp, onearray, newArray, newArray.shape.columns, value, 1, 1, onearray.frac);
  #else
    exec_dsp(dsps_mulc_s16_esp, onearray, newArray, newArray.shape.columns, value, 1, 1, onearray.frac);
  #endif
    return newArray;
  }
  
  Array<int8_t> operator*(Array<int8_t>& onearray, const int8_t value)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #else
    exec_dsp(dsps_mulc_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #endif
    return newArray;
  }
  
  Array<float> operator*(const float value, Array<float> onearray)
  {
    Array<float> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_mulc_f32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }
  
  Array<int32_t> operator*(const int32_t value, Array<int32_t> onearray)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_mulc_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }
  
  Array<uint32_t> operator*(const uint32_t value, Array<uint32_t> onearray)
  {
    Array<uint32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ",\
                    dsps_mulc_s32_esp,(int32_t*)onearray,\
                    (int32_t*)newArray,\
                    newArray.shape.columns,\
                    value);
  #else
    exec_dsp(dsps_mulc_s32_esp,(int32_t*)onearray,\
                (int32_t*)newArray,\
                newArray.shape.columns,\
                value);
  #endif
    return newArray;
  }

  Array<int16_t> operator*(const int16_t value, Array<int16_t> onearray)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s16_esp, onearray, newArray, newArray.shape.columns, value, 1, 1, onearray.frac);
  #else
    exec_dsp(dsps_mulc_s16_esp, onearray, newArray, newArray.shape.columns, value, 1, 1, onearray.frac);
  #endif
    return newArray;
  }

  Array<int8_t> operator*(const int8_t value, Array<int8_t> onearray)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_mulc_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #else
    exec_dsp(dsps_mulc_s8_esp, onearray, newArray, newArray.shape.columns, &value);
  #endif
    return newArray;
  }

  Array<float> operator/(Array<float>& onearray, const float value)
  {
    Array<float> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_divc_f32_esp, onearray, newArray, onearray.shape.columns, value);
  #else
    exec_dsp(dsps_divc_f32_esp, onearray, newArray, onearray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<int32_t> operator/(Array<int32_t>& onearray, const int32_t value)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_divc_s32_esp, onearray, newArray, onearray.shape.columns, value);
  #else
    exec_dsp(dsps_divc_s32_esp, onearray, newArray, onearray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<int16_t> operator/(Array<int16_t>& onearray, const int16_t value)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_divc_s16_esp, onearray, newArray, onearray.shape.columns, value, 1, 1, onearray.frac);
  #else
    exec_dsp(dsps_divc_s16_esp, onearray, newArray, onearray.shape.columns, value, 1, 1, onearray.frac);
  #endif
    return newArray;
  }
  
  Array<int8_t> operator/(Array<int8_t>& onearray, const int8_t value)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_divc_s8_esp, onearray, newArray, onearray.shape.columns, value);
  #else
    exec_dsp(dsps_divc_s8_esp, onearray, newArray, onearray.shape.columns, value);
  #endif
    return newArray;
  }
  
  Array<float> operator/(const float value, Array<float> another)
  {
    Array<float> newArray(another.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_cdiv_f32_esp, another, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_cdiv_f32_esp, another, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }
  
  Array<int32_t> operator/(const int32_t value, Array<int32_t> onearray)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_cdiv_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_cdiv_s32_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<int16_t> operator/(const int16_t value, Array<int16_t> onearray)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_cdiv_s16_esp, onearray, newArray, newArray.shape.columns, value, 1, 1, onearray.frac);
  #else
    exec_dsp(dsps_cdiv_s16_esp, onearray, newArray, newArray.shape.columns, value, 1, 1, onearray.frac);
  #endif
    return newArray;
  }

  Array<int8_t> operator/(const int8_t value, Array<int8_t> onearray)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_cdiv_s8_esp, onearray, newArray, newArray.shape.columns, value);
  #else
    exec_dsp(dsps_cdiv_s8_esp, onearray, newArray, newArray.shape.columns, value);
  #endif
    return newArray;
  }

  Array<float> operator/(Array<float>& onearray, Array<float> another)
  {
    Array<float> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_div_f32_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_div_f32_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }
  
  Array<int32_t> operator/(Array<int32_t>& onearray, Array<int32_t> another)
  {
    Array<int32_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_div_s32_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_div_s32_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }
  
  Array<int16_t> operator/(Array<int16_t>& onearray, Array<int16_t> another)
  {
    Array<int16_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_div_s16_esp, onearray, another, newArray, onearray.shape.columns, 1, 1, 1, onearray.frac);
  #else
    exec_dsp(dsps_div_s16_esp, onearray, another, newArray, onearray.shape.columns, 1, 1, 1, onearray.frac);
  #endif
    return newArray;
  }
  
  Array<int8_t> operator/(Array<int8_t>& onearray, Array<int8_t> another)
  {
    Array<int8_t> newArray(onearray.shape);
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_div_s8_esp, onearray, another, newArray, onearray.shape.columns);
  #else
    exec_dsp(dsps_div_s8_esp, onearray, another, newArray, onearray.shape.columns);
  #endif
    return newArray;
  }
  
  float operator^(Array<float>& onearray, Array<float> another)
  {
    float result;
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_dotprod_f32_ae32, onearray, another, &result, onearray.shape.columns);
  #else
    exec_dsp(dsps_dotprod_f32_ae32, onearray, another, &result, onearray.shape.columns);
  #endif
    return result;
  }

  int32_t operator^(Array<int32_t>& onearray, Array<int32_t> another)
  {
    int32_t result;
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_dotp_s32_esp, onearray, another, &result, onearray.shape.columns, 1, 1);
  #else
    exec_dsp(dsps_dotp_s32_esp, onearray, another, &result, onearray.shape.columns, 1, 1);
  #endif
    return result;
  }
  
  int16_t operator^(Array<int16_t>& onearray, Array<int16_t> another)
  {
    int16_t result;
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_dotp_s16_esp, onearray, another, &result, onearray.shape.columns, 1, 1, onearray.frac);
  #else
    exec_dsp(dsps_dotp_s16_esp, onearray, another, &result, onearray.shape.columns, 1, 1, onearray.frac);
  #endif
    return result;
  }

  int8_t operator^(Array<int8_t>& onearray, Array<int8_t> another)
  {
    int8_t result;
  #if BENCHMARK_TEST
    REPORT_BENCHMARK("Cycles to complete: ", dsps_dotp_s8_esp, onearray, another, &result, onearray.shape.columns, 1, 1);
  #else
    exec_dsp(dsps_dotp_s8_esp, onearray, another, &result, onearray.shape.columns, 1, 1);
  #endif
    return result;
  }

#endif
#endif
}