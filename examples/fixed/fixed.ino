#include "esp_fixed_point.h"
#include "esp_debug.h"
#include "esp_array.h"

#define INPUT_SIZE 8

using namespace espmath;
  
typedef struct Parameters{
    fixed target = 8; /* Target Temperature */                  
    fixed observed = 12; /* Observed  Temperature */
    fixed ambient = 22; /* Ambient Temperature */
    fixed heatsink = 28; /* Heatsink Temperature */
    fixed cooling = 0; /* Cooling Temperature */
    fixed currentV = 8; /* Current Voltage Output */
    fixed currentC = 1.5; /* Current Current Output */
    int8_t energy = 80; /* Current Battery Level */
    fixed minTarget = 8; /* Minimum Target Temperature */
    fixed maxTarget = 18; /* Maximum Target Temperature */
    fixed maxV = 8; /* Maximum Voltage Output */
    fixed maxC = 3; /* Maximum Current Output */
    const uint32_t expectTime = 1800; /* Expected Time to Achieve The Target Temperature */
    const int8_t energyTh = 70; /* Battery Level Threshold That This Values Takes Effect (higher than this is ignored) */
    const fixed heatsinkTh = 32; /* Maximum Temperature For The Heatsink */
}param;

const fixed C[INPUT_SIZE] = {
   2.5,  /*Temperature Difference Coefficient*/
    .1,  /*Spent Time Coefficient*/
    .8,  /*Ambient Temperature Resistance Coefficient*/
  -1.3,  /*Heatsink Temperature Coefficient*/
  -1.0,  /*Battery Level Coefficient*/
   1.1,  /*Cooling-Target Coefficient*/
   0.7,  /*Current Voltage Coefficient*/
   0.3   /*Current Current Coefficient*/
};

void regulator(Parameters* p)
{
  fixed X[INPUT_SIZE];
  
  X[0] = (p->observed - p->target) / (abs(p->observed) + abs(p->target));
  X[1] = 1 - ((float)((p->expectTime - 360) / p->expectTime));
  X[2] = (p->ambient - p->target) / (abs(p->ambient) + abs(p->target));
  X[3] = (p->heatsink - p->ambient) / (p->heatsinkTh - p->ambient);
  X[4] = p->energy > p->energyTh ? 0 : ((short)p->energy*p->energy) / 10000.0f;
  X[5] = (p->cooling - p->target) / (p->maxTarget - p->minTarget);
  X[6] = 1 - p->currentV / p->maxV;
  X[7] = 1 - p->currentC / p->maxC;

  Array<int16_t> input(X, shape2D(1,INPUT_SIZE));
  Array<int16_t> coeff(C, shape2D(1,INPUT_SIZE));
  
  debug.print("X: ");
  debug.print(X, (uint32_t)INPUT_SIZE, (uint32_t)4);
  debug.print("C: ");
  debug.print(C, (uint32_t)INPUT_SIZE, (uint32_t)4);
  
  
  if ((!X[0] && !X[2]) || X[0] < 0.f || X[3] >= 1.f || X[5] > 0)
  {
    debug.print("Shutdown!");
    return;
  }

  fixed y;
  y.data = input ^ coeff; // Dot Product
  fixed activF = tanh(y);
  
  debug.print("X * C: " + String(y));
  debug.print("tanh(X * C): " + String(activF));
  debug.print("Output Voltage: " + String(((activF + 1.) / 2.) * (p->maxV)) + "V");
}

void setup()
{
  param p;
  vTaskDelay(pdMS_TO_TICKS(5000));
  regulator(&p);
}

void loop(){}