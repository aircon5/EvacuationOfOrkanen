#ifndef RADAR_I_RUMMET_H
#define RADAR_I_RUMMET_H

#include <Arduino.h>
#include <HardwareSerial.h>

class RadarSensor {
public:
    RadarSensor(HardwareSerial& serial);

    void begin();
    void update();

    bool isPersonDetected() const;
    bool isDataValid() const;
    unsigned long getLastUpdate() const;
    

private:
    HardwareSerial& radarSerial;

    bool personDetected = false;
    bool dataValid = false;
    unsigned long lastUpdate = 0;
    const unsigned long DATA_TIMEOUT = 5000;
    String receivedMessage = "";
    bool messageTooLong = false;
};

#endif