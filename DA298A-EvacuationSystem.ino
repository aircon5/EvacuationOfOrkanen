
#include "radar_i_rummet.h"

HardwareSerial radarSerial(2);
RadarSensor radar(radarSerial);

void setup() {
  Serial.begin(115200);
  radar.begin();

}

void loop() {
  radar.update();
  if (radar.isDataValid()) {
    if (radar.isPersonDetected()) {
        Serial.println("Person detected");

       } else {

        Serial.println("No person detected");

        }

      } else {

      Serial.println("Radar data unknown");

}

}