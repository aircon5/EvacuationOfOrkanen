#include "Map.h"
#include <Arduino.h>    

struct Rec {
    uint8_t x1;
    uint8_t x2;
    uint8_t y1;
    uint8_t y2;
    uint8_t val;
};

static const Rec tileMap[] PROGMEM {
    //RUMMEN
    {40, 72, 82, 110, ROOM}, //RUM A
    {76, 105, 82, 110, ROOM}, //RUM B
    {107, 140, 78, 110, ROOM}, //RUM C
    {142, 166, 78, 110, ROOM}, //RUM D

    //TRAPPOR
    {38, 49, 42, 60, STAIRS}, //Vänster trappa
    {165, 180, 49, 60, STAIRS}, //Höger trappa

    //EXITS
    {40, 40, 58, 58, EXIT}, //Vänstra utgången
    {178, 178, 53, 53, EXIT}, //Högra utgången

    //VÄGGAR
      {39, 167, 111, 112, WALL},
    {167, 167, 78, 110, WALL},
    {141, 141, 78, 110, WALL},
    {106, 106, 78, 110, WALL},
    {106, 167, 77, 77, WALL},
    {104, 105, 78, 78, WALL},
    {94, 96, 78, 78, WALL},
    {86, 95, 79, 79, WALL},
    {79, 88, 80, 80, WALL},
    {76, 81, 81, 81, WALL},
    {74, 75, 82, 110, WALL},
    {47, 75, 81, 81, WALL},
    {39, 39, 81, 81, WALL},
    {39, 39, 65, 110, WALL},
    {39, 45, 62, 64, WALL},
    {60, 60, 25, 74, WALL},
    {50, 50, 42, 60, WALL},
    {44, 44, 42, 55, WALL},
    {37, 50, 61, 61, WALL},
    {37, 37, 25, 60, WALL},
    {37, 60, 24, 24, WALL},
    {38, 44, 41, 41, WALL},
    {60, 80, 74, 74, WALL},
    {79, 84, 73, 73, WALL},
    {82, 87, 72, 72, WALL},
    {86, 91, 71, 71, WALL},
    {91, 95, 70, 70, WALL},
    {95, 97, 69, 69, WALL},
    {61, 97, 61, 61, WALL},
    {73, 73, 62, 73, WALL},
    {98, 155, 61, 61, WALL},
    {164, 181, 61, 61, WALL},
    {180, 181, 62, 81, WALL},
    {167, 181, 81, 81, WALL},
    {154, 155, 25, 60, WALL},
    {167, 181, 24, 24, WALL},
    {180, 181, 25, 47, WALL},
    {164, 181, 48, 48, WALL},
    {181, 181, 49, 60, WALL},
    {164, 164, 55, 60, WALL},
    {164, 174, 54, 54, WALL}
};

struct checkptRec {
    uint8_t x1;
    uint8_t x2;
    uint8_t y1;
    uint8_t y2;
    uint8_t checkpoint;
};

static const checkptRec checkptMap[] PROGMEM = {
    {74, 75, 105, 108, GEM_LABB_DOOR},
    {41, 46, 81, 81, LABB_A330},
    {97, 103, 78, 78, LABB_A331},
    {127, 132, 77, 77, SAL_A332},
    {153, 158, 77, 77, SAL_A333},
    {51, 59, 61, 61, A330_OUTSIDE_DOWN},
    {60, 60, 75, 80, A330_OUTSIDE_RIGHT},
    {156, 163, 61, 61, A333_DOWN},
    {96, 96, 70, 77, A331_OUTISDE_LEFT},
    {45, 49, 42, 42, LEFT_STAIRS},
    {51, 59, 42, 42, LEFT_CORRIDOR_DOWN},
    {164, 164, 49, 53, RIGHT_STAIRS}
};
//Detta ovanför kommer jag vidare utveckla, då detta ska vara kartan fast på flash memory istället för RAM


Patch mapOfOrkanen[mapHeight][mapWidth];

bool validCoord(int x, int y) {
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
        Serial.println("Invalid coordinates");
        return false;
    }
    return true;
}

void setTile(int x, int y, TileType type) {
    if (validCoord(x, y)) {
        mapOfOrkanen[y][x].tileType = type;
    }
}

void setCheckpoint(int x, int y, CheckpointType checkpoint) {
    if (validCoord(x, y)) {
        mapOfOrkanen[y][x].checkpoint = checkpoint;
    }
}

void setupWorld() {
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            mapOfOrkanen[y][x].tileType = FLOOR;
            mapOfOrkanen[y][x].checkpoint = NO_CHECKPOINT;
            mapOfOrkanen[y][x].doorFlag = NO_DOOR;
        }
    }
}

void setupMap() {
    setupWorld();
    setupRoomsAndCorridors();
    setupDoors();
    setupStairs();
    setupExits();
    setupWalls();

    Serial.println("Kartan är uppsatt");
}

void setupRoomsAndCorridors() {
    
    //Rum A 
    for (int x = 40; x <= 73; x++) {
        for (int y = 82; y <= 110; y++) {
           mapOfOrkanen[y][x].tileType = ROOM;
        }
    }

     //Rum B
    for (int x = 76; x <= 105; x++) {
        for (int y = 82; y <= 110; y++) {
           mapOfOrkanen[y][x].tileType = ROOM;
        }
    }
    
    //Rum C
    for (int x = 107; x <= 140; x++) {
        for (int y = 78; y <= 110; y++) {
           mapOfOrkanen[y][x].tileType = ROOM;
        }
    }

    //Rum D
    for (int x = 142; x <= 166; x++) {
        for (int y = 78; y <= 110; y++) {
           mapOfOrkanen[y][x].tileType = ROOM;
        }
    }

}

void setupDoors() {

    //Gemensam labb dörr
     for (int x = 74; x <= 75; x++) {
        for (int y = 105; y <= 108; y++) {
           mapOfOrkanen[y][x].checkpoint = GEM_LABB_DOOR;
           mapOfOrkanen[y][x].doorFlag = DOOR_OPEN;
        }
    }

    //Labb A dörr
     for (int x = 41; x <= 46; x++) {
           mapOfOrkanen[81][x].checkpoint = LABB_A330;
           mapOfOrkanen[81][x].doorFlag = DOOR_OPEN;
        }

    //Labb B dörr
    for (int x = 97; x <= 103; x++) {
           mapOfOrkanen[78][x].checkpoint = LABB_A331;
           mapOfOrkanen[78][x].doorFlag = DOOR_OPEN;
    }

    // SalCexit
    for (int x = 127; x <= 132; x++) {
        mapOfOrkanen[77][x].checkpoint = SAL_A332;
        mapOfOrkanen[77][x].doorFlag = DOOR_OPEN;
    }

    // SalDihxit
    for (int x = 153; x <= 158; x++) {
        mapOfOrkanen[77][x].checkpoint = SAL_A333;
        mapOfOrkanen[77][x].doorFlag = DOOR_OPEN;
    }

    // VänsterMittNer
    for (int x = 51; x <= 59; x++) {
        mapOfOrkanen[61][x].checkpoint = A330_OUTSIDE_DOWN;
        mapOfOrkanen[61][x].doorFlag = DOOR_OPEN;
    }

    // VänsterMittHöger
    for (int y = 75; y <= 80; y++) {
        mapOfOrkanen[y][60].checkpoint = A330_OUTSIDE_RIGHT;
        mapOfOrkanen[y][60].doorFlag = NO_DOOR;
    }

    // HögerMittNer
    for (int x = 156; x <= 163; x++) {
        mapOfOrkanen[61][x].checkpoint = A333_DOWN;
        mapOfOrkanen[61][x].doorFlag = DOOR_OPEN;
    }

    // MittenVänster
    for (int y = 70; y <= 77; y++) {
        mapOfOrkanen[y][96].checkpoint = A331_OUTISDE_LEFT;
        mapOfOrkanen[y][96].doorFlag = NO_DOOR;
    }

    // VänsterTrappa
    for (int x = 45; x <= 49; x++) {
        mapOfOrkanen[42][x].checkpoint = LEFT_STAIRS;
        mapOfOrkanen[42][x].doorFlag = NO_DOOR;
    }

    // VänsterKorridorUpp
    for (int x = 51; x <= 59; x++) {
        mapOfOrkanen[42][x].checkpoint = LEFT_CORRIDOR_DOWN;
        mapOfOrkanen[42][x].doorFlag = NO_DOOR;
    }

    // HögerTrappa
    for (int y = 49; y <= 53; y++) {
        mapOfOrkanen[y][164].checkpoint = RIGHT_STAIRS;
        mapOfOrkanen[y][164].doorFlag = NO_DOOR;
    }
    Serial.println("Dörrar skapade");
}

void setupExits() {
    
    //Utgång A, Vänster
    setTile(40, 58, EXIT);

    //Utgång B, Höger
    setTile(178, 53, EXIT);
    Serial.println("Utgångar skapade");
}

void setupStairs() {

    //Vänster trappa
    for (int x = 38; x <= 49; x++) {
        for (int y = 42; y <= 60; y++) {
            setTile(x, y, STAIRS);
        }
    }

    //Höger trappa
    for (int x = 165; x <= 180; x++) {
        for (int y = 49; y <= 60; y++) {
            setTile(x, y, STAIRS);
        }
    }
    Serial.println("Trappor skapade");
}

void setupWalls() {
    //Övre väggarna för Salarna
    for (int x = 39; x <= 167; x++) {
        for (int y = 111; y <= 112; y++) {
            setTile(x, y, WALL);
        }
    }

    //Sal D höger vägg
    for (int y = 78; y <= 110; y++) {
        setTile(167, y, WALL);
    }

    //Sal C och D vägg
    for (int y = 78; y <= 110; y++) {
        setTile(141, y, WALL);
    }

    //Sal C och B vägg
    for (int y = 78; y <= 110; y++) {
        setTile(106, y, WALL);
    }

    //Sal C och D nedre vägg
    for (int x = 106; x <= 167; x++) {
        setTile(x, 77, WALL);
    }

    //Nedre vägg för Sal B (Kommer aldirg skriva den igen)
    for (int x = 104; x <= 105; x++) {
        setTile(x, 78, WALL);
    }
    for (int x = 94; x <= 96; x++) {
        setTile(x, 78, WALL);
    }
    for (int x = 86; x <= 95; x++) {
    setTile(x, 79, WALL);
    }
    for (int x = 79; x <= 88; x++) {
    setTile(x, 80, WALL);
    }
    for (int x = 76; x <= 81; x++) {
    setTile(x, 81, WALL);
    }  

    //Sal A och B vägg
    for (int x = 74; x <= 75; x++) {
        for (int y = 82; y <= 110; y++) {
            setTile(x, y, WALL);
        }
    }

    //Sal A och B nedre vägg
    for (int x = 47; x <= 75; x++) {
        setTile(x, 81, WALL);
    }
    setTile(39, 81, WALL);

    //Vänster vägg för Sal A och korridoren
    for (int y = 65; y <= 110; y++) {
        setTile(39, y, WALL);
    }
    for (int x = 39; x <= 45; x++) {
        for (int y = 62; y <= 64; y++) {
            setTile(x, y, WALL);
        }
    }

    //Korridor mellan Sal A och trappan
    for (int y = 25; y <= 74; y++) {
        setTile(60, y, WALL);
    }
    for (int y = 42; y <= 60; y++) {
            setTile(50, y, WALL);
    }

    //Vänster trappa och kooridor utanför
    for (int y = 42; y <= 55; y++) {
        setTile(44, y, WALL);
    }
    for (int x = 37; x <= 50; x++) {
        setTile(x, 61, WALL);
    }
    for (int y = 25; y <= 60; y++) {
        setTile(37, y, WALL);
    }
    for (int x = 37; x <= 60; x++) {
        setTile(x, 24, WALL);
    }
    for (int x = 38; x <= 44; x++) {
        setTile(x, 41, WALL);
    }

    //Korridor mellan Sal A och B
    for (int x = 60; x <= 80; x++) {
        setTile(x, 74, WALL);
    }
    for (int x = 79; x <= 84; x++) {
        setTile(x, 73, WALL);
    }
    for (int x = 82; x <= 87; x++) {
        setTile(x, 72, WALL);
    }
    for (int x = 86; x <= 91; x++) {
        setTile(x, 71, WALL);
    }
    for (int x = 91; x <= 95; x++) {
        setTile(x, 70, WALL);
    }
    for (int x = 95; x <= 97; x++) {
        setTile(x, 69, WALL);
    }

    //Rummen vid korridoren mellan Sal A och B
    for (int x = 61; x <= 97; x++) {
        setTile(x, 61, WALL);
    }
    for (int y = 62; y <= 73; y++) {
        setTile(73, y, WALL);
    }

    //Korridoren mellan Sal C och D
    //Nedre väggar
    for (int x = 98; x <= 155; x++) {
        setTile(x, 61, WALL);
    }
    for (int x = 164; x <= 181; x++) {
        setTile(x, 61, WALL);
    }
    //Höger vägg
    for (int x = 180; x <= 181; x++) {
        for (int y = 62; y <= 81; y++) {
            setTile(x, y, WALL);
        }
    }
    //Uppre högra vägg
    for (int x = 167; x <= 181; x++) {
        setTile(x, 81, WALL);
    }

    //Höger trappa och korridor utanför
    //Vänster vägg
    for (int x = 154; x <= 155; x++) {
        for (int y = 25; y <= 60; y++) {
            setTile(x, y, WALL);
        }
    }
    //Nedre vägg
    for (int x = 167; x <= 181; x++) {
        setTile(x, 24, WALL);
    }
    //Höger vägg
    for (int x = 180; x <= 181; x++) {
        for (int y = 25; y <= 47; y++) {
            setTile(x, y, WALL);
        }
    }
    //Höger trappa nedre vägg
    for (int x = 164; x <= 181; x++) {
        setTile(x, 48, WALL);
    }
    //Höger trappa höger vägg
    for (int y = 49; y <= 60; y++) {
        setTile(181, y, WALL);
    }
    //Höger trappa vänster vägg och mitt
    for (int y = 55; y <= 60; y++) {
        setTile(164, y, WALL);
    }

    for (int x = 164; x <= 174; x++) {
            setTile(x, 54, WALL);
        
    }
    Serial.println("Satt ut väggar");
}

void blockDoor(CheckpointType checkpt) {

    bool blocked = false;

    for (int x = 0; x < mapWidth; x++) {

        for (int y = 0; y < mapHeight; y++) {

            if (mapOfOrkanen[y][x].checkpoint == checkpt) {
                blocked = true;
                mapOfOrkanen[y][x].doorFlag = DOOR_CLOSED;
            }
        }
    }
    if (blocked) {
        Serial.println("Dörr blockerad för checkpoint: " + checkpt);
    } else {
        Serial.println("Dörren finns inte, kan inte blockera");
    }
}

void openDoor(CheckpointType checkpt) {

    bool open = false;

    for (int x = 0; x < mapWidth; x++) {

        for (int y = 0; y < mapHeight; y++) {

            if (mapOfOrkanen[y][x].checkpoint == checkpt) {
                open = true;
                mapOfOrkanen[y][x].doorFlag = DOOR_OPEN;
            }
        }
    }
    if (open) {
        Serial.println("Dörr är öppen för checkpoint: " + checkpt);
    } else {
        Serial.println("Dörren finns inte, kan inte öppna");
    }
    
}

bool isBlocked(int x, int y) {
    if (!validCoord(x, y) or mapOfOrkanen[y][x].tileType == WALL or mapOfOrkanen[y][x].doorFlag == DOOR_CLOSED) {
        return true;
    }
    return false;
}