#include "test_bench.h"

void setup(){
  vTaskDelay(pdMS_TO_TICKS(5000));
  debug.print("Free size[bytes]: " + String(heap_caps_get_free_size(Array<>::memCaps())));
  debug.print("Testing floating-point arrays arithmetic...");
  test_ari<float,10>();
  debug.print("Succeeded!");
  debug.print("Testing integer 32 bits arrays arithmetic...");
  test_ari<int32_t,10>();
  debug.print("Succeeded!");
  debug.print("Testing integer 16 bits arrays arithmetic...");
  test_ari<int16_t,10>();
  debug.print("Succeeded!");
  debug.print("Testing integer 8 bits arrays arithmetic...");
  test_ari<int8_t,10>();
  debug.print("Succeeded!");
  debug.print("Free size[bytes]: " + String(heap_caps_get_free_size(Array<>::memCaps())));
}

void loop(){}
