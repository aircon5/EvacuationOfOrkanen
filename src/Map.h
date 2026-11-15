#ifndef MAP_H
#define MAP_H
#include <Arduino.h>

const int mapWidth = 220;
const int mapHeight = 120;

//Tile typer
enum TileType : uint8_t {
    FLOOR,
    WALL,
    DOOR,
    EXIT,
    STAIRS,
    ROOM,
    CORRIDOR
};

//Checkpoint typer
enum CheckpointType : uint8_t {
    NO_CHECKPOINT, // Tror inte denna behövs
    GEM_LABB_DOOR,
    LABB_A330,
    LABB_A331,
    SAL_A332,
    SAL_A333,
    A330_OUTSIDE_DOWN,
    A330_OUTSIDE_RIGHT,
    A333_DOWN,
    A331_OUTISDE_LEFT,
    LEFT_STAIRS,
    RIGHT_STAIRS,
    LEFT_CORRIDOR_DOWN
};

//Doorflag, dörr status eller bara inte en dörr
enum DoorFlag : uint8_t {
    DOOR_CLOSED,
    DOOR_OPEN,
    NO_DOOR
};

struct Patch {
    uint8_t tileType;
    uint8_t checkpoint;
    uint8_t doorFlag;
}; 

extern Patch mapOfOrkanen[mapHeight][mapWidth]; // mapHeight är y koordinaten och är mindre än x vilket betyder mindre minne används.

//Skapar hela världen med default värden för varje tile/patch
void setupWorld();

//Resterande här skapar de olika delarna av kartan, rum, korridorer, dörrar, utgångar och väggar.
void setupRoomsAndCorridors();
void setupDoors();
void setupExits();
void setupStairs();
void setupWalls();

//Denna funktionen sätter upp kartan genom att kalla på dem andra funktioerna ovan
void setupMap();

//Dessa funktioner byter statusen på dörrarna
void blockDoor(CheckpointType checkpt);
void openDoor(CheckpointType checkpt);

//Checkar om koordinaterna är innanför kartan, annars returnar false
bool validCoord(int x, int y);

//Ställer in en tile på kartan med en viss typ
void setTile(int x, int y, TileType type);

//Ställer in en checkpoint på kartan med en viss typ
void setCheckpoint(int x, int y, CheckpointType checkpoint);

//Checkar om en tile är blockerad (Vägg, stängd dörr eller utanför) och returnar true, om inte returnar den false
bool isBlocked(int x, int y);
#endif

