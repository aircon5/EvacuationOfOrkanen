#ifndef NODE_H
#define NODE_H
#include <Arduino.h>
#include <vector>
#include "Map.h"

extern char direction;
extern int xcoord;
extern int ycoord;
extern String locationInBuiling;
extern bool active;
extern bool atStairs;
extern std::vector<String> freeToGo;

struct NearbyCheckpt {
    CheckpointType checkpt;
    String direction;
};

struct room {
    String roomName; 
    int amountPeople;
    bool empty;
    bool freeToGo;
};

extern room rooms[4];

String setupNode(int x, int y); //constructor
void updateNodeDirection();
void checkRooms();
void checkPeopleInRooms(String nodeStartedHere);

void setupCheckpt(int ID);
String getCheckptDirection(CheckpointType checkpt);


#endif