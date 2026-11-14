#include "Map.h"
#include <Arduino.h>    

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
           mapOfOrkanen[81][x].doorFlag = DOOR_OPEN;
        }

    //Labb B dörr
    for (int x = 97; x <= 103; x++) {
           mapOfOrkanen[78][x].checkpoint = LABB_B_MAIN;
           mapOfOrkanen[78][x].doorFlag = DOOR_OPEN;
    }

    // SalCexit
    for (int x = 127; x <= 132; x++) {
        mapOfOrkanen[77][x].checkpoint = SAL_C_EXIT;
        mapOfOrkanen[77][x].doorFlag = DOOR_OPEN;
    }

    // SalDihxit
    for (int x = 153; x <= 158; x++) {
        mapOfOrkanen[77][x].checkpoint = SAL_DIH_EXIT;
        mapOfOrkanen[77][x].doorFlag = DOOR_OPEN;
    }

    // VänsterMittNer
    for (int x = 51; x <= 59; x++) {
        mapOfOrkanen[61][x].checkpoint = LEFT_MIDDLE_DOWN;
        mapOfOrkanen[61][x].doorFlag = DOOR_OPEN;
    }

    // VänsterMittHöger
    for (int y = 75; y <= 80; y++) {
        mapOfOrkanen[y][60].checkpoint = LEFT_MIDDLE_RIGHT;
        mapOfOrkanen[y][60].doorFlag = NO_DOOR;
    }

    // HögerMittNer
    for (int x = 156; x <= 163; x++) {
        mapOfOrkanen[61][x].checkpoint = RIGHT_MIDDLE_DOWN;
        mapOfOrkanen[61][x].doorFlag = DOOR_OPEN;
    }

    // MittenVänster
    for (int y = 70; y <= 77; y++) {
        mapOfOrkanen[y][96].checkpoint = MIDDLE_LEFT;
        mapOfOrkanen[y][96].doorFlag = NO_DOOR;
    }

    // VänsterTrappa
    for (int x = 45; x <= 49; x++) {
        mapOfOrkanen[42][x].checkpoint = LEFT_STAIRS;
        mapOfOrkanen[42][x].doorFlag = NO_DOOR;
    }

    // VänsterKorridorUpp
    for (int x = 51; x <= 59; x++) {
        mapOfOrkanen[42][x].checkpoint = LEFT_CORRIDOR_UP;
        mapOfOrkanen[42][x].doorFlag = NO_DOOR;
    }

    // HögerTrappa
    for (int y = 49; y <= 53; y++) {
        mapOfOrkanen[y][164].checkpoint = RIGHT_STAIRS;
        mapOfOrkanen[y][164].doorFlag = NO_DOOR;
    }

}

void setupExits() {
    
    //Utgång A, Vänster
    setTile(40, 58, EXIT);

    //Utgång B, Höger
    setTile(178, 53, EXIT);
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
}

void blockDoor(CheckpointType checkpt) {

    for (int x = 0; x < mapWidth; x++) {

        for (int y = 0; y < mapHeight; y++) {

            if (mapOfOrkanen[y][x].checkpoint == checkpt) {

                mapOfOrkanen[y][x].doorFlag = DOOR_CLOSED;
            }
        }
    }
}

void openDoor(CheckpointType checkpt) {

    for (int x = 0; x < mapWidth; x++) {

        for (int y = 0; y < mapHeight; y++) {

            if (mapOfOrkanen[y][x].checkpoint == checkpt) {

                mapOfOrkanen[y][x].doorFlag = DOOR_OPEN;
            }
        }
    }
}

bool isBlocked(int x, int y) {
    if (!validCoord(x, y) or mapOfOrkanen[y][x].tileType == WALL or mapOfOrkanen[y][x].doorFlag == DOOR_CLOSED) {
        return true;
    }
    return false;
}