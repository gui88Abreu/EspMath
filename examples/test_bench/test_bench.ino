#include "test_bench.h"

constexpr size_t array_length = (uint16_t)(1 + (__TIME__[7] - 48)*28 + (__TIME__[4] - 48));
void setup(){
  vTaskDelay(pdMS_TO_TICKS(5000));
  debug.print("Array Size: " + String(array_length));
  debug.print("Free size[bytes]: " + String(xPortGetFreeHeapSize()));
  debug.print("Testing floating-point arrays arithmetic...");
  test_ari<float,array_length>();
  debug.print("Succeeded!");
  debug.print("Testing integer 32 bits arrays arithmetic...");
  test_ari<int32_t,array_length>();
  debug.print("Succeeded!");
  debug.print("Testing integer 16 bits arrays arithmetic...");
  test_ari<int16_t,array_length>();
  debug.print("Succeeded!");
  debug.print("Testing integer 8 bits arrays arithmetic...");
  test_ari<uint8_t,array_length>();
  debug.print("Completed!");
  debug.print("Free size[bytes]: " + String(xPortGetFreeHeapSize()));
}

void loop(){}
