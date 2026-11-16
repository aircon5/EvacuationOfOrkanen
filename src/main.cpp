#include "node.h"
#include "Map.h"
#include "Algoritm.h"
#include <Arduino.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "stdint.h"

String test;
exitChoice exitA;
std::vector<int> way;
std::vector<CheckpointType> nodCheckpoints;

void setup() {
    Serial.begin(115200);

    delay(1000);

    setupMap();
    test = setupNode(128, 70);
    Astar(xcoord, ycoord, exitA, way, nodCheckpoints);
}


void loop() {

    Serial.println("--------------------");
    //Serial.println("testar om det skapar en nod med rätt plats" + test);
    /* Test Purpose
    checkPeopleInRooms("A331");
    
    for (String roomName : freeToGo) {

        for (int i = 0; i < 4; i++) {

            if (rooms[i].roomName == roomName) {
                Serial.print(rooms[i].roomName);
                Serial.print(": ");
                Serial.println(rooms[i].amountPeople);
            }
        }
    }
    rooms[0].amountPeople--;
    rooms[3].amountPeople--;

    
    updateNodeDirection();
    */
    Serial.println("--------------------");

    delay(1000);
}



