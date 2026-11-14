#ifndef NODE_H
#define NODE_H
#include <Arduino.h>
#include <vector>


extern char direction;
extern int xcoord;
extern int ycoord;
extern String locationInBuiling;
extern bool active;
extern bool atStairs;
extern std::vector<String> freeToGo;

struct room {
    String roomName; 
    int amountPeople;
    bool empty;
    bool freeToGo;
};

extern room rooms[4];

String setupNode(int x, int y); //constructor
void updateNodeDirection();
void checkPeopleInRooms(String nodeStartedHere);
void checkRooms();

#endif