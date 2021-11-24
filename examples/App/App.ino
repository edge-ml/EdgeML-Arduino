/* 
 * Use this sketch if you want to control nicla over BLE in combination with edge-ml.
*/

#include "Arduino.h"
#include "Edge_ML.h"

// Set DEBUG to true in order to enable debug print
#define DEBUG false

void setup()
{
#if DEBUG
  Serial.begin(115200);
  edge_ml.debug(Serial);
#endif

  edge_ml.begin();
}

void loop()
{
  // Update and then sleep
  edge_ml.update();
}