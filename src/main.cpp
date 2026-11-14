#include "Map.h"
#include "Algoritm.h"
#include <Arduino.h>
#include <unordered_map>
#include <string>

using namespace std;

int directions = 0;
int nextNode = 0;

int routeCheckpoints[3] = {0, 0, 0};

int nearbyCheckpoint[3][2] = {
    {5, 10},
    {8, 20},
    {2, 30}
};

bool priority = false;

int xcor = 0;
int ycor = 0;

String position = "";
int heading = 0;

unordered_map<string, int> people = {
    {"A", 20},
    {"B", 10},
    {"C", 0},
    {"D", 5}
};

int getAmountOfPeople();
void updateNodeDirection();
void updateDirectionOnNode(int coorx, int coory, int newDir);



void setup() {
    Serial.begin(115200);

    delay(1000);

    setupMap();
}


void loop() {

    updateNodeDirection();

    Serial.println("--------------------");

    Serial.print("Heading: ");
    Serial.println(heading);

    Serial.print("Direction: ");
    Serial.println(directions);

    Serial.println("--------------------");

    delay(1000);
}


int getAmountOfPeople() {
    return 10;
}


void updateNodeDirection() {

    bool found = false;
    int selectedCheckpoint = -1;

    // Loop through route checkpoints
    for (int i = 0; i < 3; i++) {

        int c = routeCheckpoints[i];

        Serial.print("Checking route checkpoint: ");
        Serial.println(c);

        // Loop through nearby checkpoints
        for (int j = 0; j < 3; j++) {

            if (nearbyCheckpoint[j][0] == c) {

                selectedCheckpoint = nearbyCheckpoint[j][1];

                found = true;

                Serial.print("Match found! Direction: ");
                Serial.println(selectedCheckpoint);

                break;
            }
        }

        if (found) {
            break;
        }
    }

    // Update direction if a match was found
    if (found) {

        directions = selectedCheckpoint;
        heading = selectedCheckpoint;

    } else {

        Serial.println("No matching checkpoint found!");

    }
}


void updateDirectionOnNode(int coorx, int coory, int newDir) {

    if (xcor == coorx && ycor == coory) {

        directions = newDir;
        heading = newDir;
        priority = true;

        Serial.println("Node direction updated!");

    }
}
