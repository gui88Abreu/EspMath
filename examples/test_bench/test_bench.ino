#include "test_bench.h"

//constexpr size_t array_length = ((uint16_t)((__TIME__[7] + __TIME__[6]/10.0f)*10) % 50);
constexpr size_t array_length = 53;
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
  test_ari<int8_t,array_length>();
  debug.print("Succeeded!");
  debug.print("Free size[bytes]: " + String(xPortGetFreeHeapSize()));
}

void loop(){}
