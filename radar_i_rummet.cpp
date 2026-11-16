#include "radar_i_rummet.h"

// Konstruktor
RadarSensor::RadarSensor(HardwareSerial& serial)
    : radarSerial(serial) {
}

// Starta kommunikationen med radarn
void RadarSensor::begin() {
    radarSerial.begin(115200, SERIAL_8N1, 16, 17);
}

// Läser inkommande data från radarn
void RadarSensor::update() {
    while (radarSerial.available() > 0) {
        char c = radarSerial.read();
            if (c == '\n') {
              receivedMessage.trim();
              if (!messageTooLong && receivedMessage == "ON") {

                personDetected = true;

                dataValid = true;

                lastUpdate = millis();

            }else if (!messageTooLong && receivedMessage == "OFF") {
              personDetected = false;
              dataValid = true;
              lastUpdate = millis();
             }
             receivedMessage = "";
             messageTooLong = false;

        } else {

         if (receivedMessage.length() < 32) {
          receivedMessage += c;

         } else {
          messageTooLong = true; 
         }

        }

    }
    
}

// Returnerar om radarn detekterar närvaro
bool RadarSensor::isPersonDetected() const {
    return personDetected;
}

// Returnerar om radardatan är giltig
bool RadarSensor::isDataValid() const {
    return dataValid && (millis() - lastUpdate < DATA_TIMEOUT);
}

// Returnerar tiden för senaste giltiga radarmeddelandet
unsigned long RadarSensor::getLastUpdate() const {
    return lastUpdate;
}