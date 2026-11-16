#include "node.h"
#include "Map.h"
#include "Algoritm.h"
#include <Arduino.h>
#include <string>
#include <unordered_map>
#include <vector>

//setup node
//kolla om det är mycket personer, för då ska den inte köra a*, fast bara ifall den är vid dörrarna
//kör a* och har då en direction
//

char direction = 0;
int xcoord = 0;
int ycoord = 0;
String locationInBuiling = "";
bool active = false;
bool atRoom = false;

int totalPeople = 0;

bool manyPeople = false;
std::vector<String> freeToGo;

std::vector<NearbyCheckpt> nearbyCheckpts;


room rooms [4] = {
    {"A330", 0, false, false},
    {"A331", 0, true, false},
    {"A332", 20, true, false},
    {"A333", 55, false, false}
};

std::unordered_map<int16_t, String> locations =
{
    {44082, "dörrA330"},
    {55062, "korridorMotTrappaVänster"},
    {100079, "dörrA331"},
    {160061, "korridorMotTrappaHöger"},
    {99071, "framförA331"},
    {128070, "framförA332"},
    {74106, "dörrMellanA330oA331"},
    {153070, "framförA333"},
    {58080, "korridor"},
    {25060, "vån2TrappaVänster"},
    {43060, "vån3TrappaVänster"},
    {178054, "vån3TrappaHöger"},
    {199066, "vån2TrappaHöger"},
    {128078, "dörrA332"},
    {153078, "dörrA333"},
    {158053, "vänsterTrappa"},
    {500053, "högerTrappa"}
};



String setupNode(int x, int y)
{
    xcoord = x;
    ycoord = y;
    int xy = x * 1000 + y;
    auto it = locations.find(xy);

    setupCheckpt(xy);

    if (it != locations.end()) {
        locationInBuiling = it->second;
        if (locationInBuiling.indexOf("A33") != -1) {atRoom = true;}
    } else {
        locationInBuiling = "Okänd plats";
    }
    return locationInBuiling;
}

void setupCheckpt(int ID) {

    nearbyCheckpts.clear();

    switch (ID)
    {
    case 44082:
        nearbyCheckpts = {
            {GEM_LABB_DOOR, "N"},
            {A330_OUTSIDE_DOWN, "S"},
            {A330_OUTSIDE_RIGHT, "S"}
        };
        break;

    case 55062:
        nearbyCheckpts = {
            {LABB_A330, "N"},
            {A330_OUTSIDE_RIGHT, "N"},
            {LEFT_CORRIDOR_DOWN, "S"}
        };
        break;

    case 100079:
        nearbyCheckpts = {
            {GEM_LABB_DOOR, "N"},
            {A333_DOWN, "S"},
            {A331_OUTISDE_LEFT, "W"}
        };
        break;

    case 50035:
        nearbyCheckpts = {
            {LEFT_STAIRS, "N"}, //Den här borde egentligen vara NW
            {LEFT_CORRIDOR_DOWN, "N"}
        };
        break;

    case 160061:
        nearbyCheckpts = {
            {LABB_A331, "N"},
            {A331_OUTISDE_LEFT, "N"},
            {RIGHT_STAIRS, "S"}
        };
        break;

    case 158053:
        nearbyCheckpts = {
            {A333_DOWN, "S"},
            {RIGHT_STAIRS, "E"}
        };
        break;

    case 99071:
        nearbyCheckpts = {
            {LABB_A331, "S"},
            {A331_OUTISDE_LEFT, "W"},
            {A333_DOWN, "E"}
        };
        break;

    case 128070:
        nearbyCheckpts = {
            {LABB_A331, "W"},
            {A331_OUTISDE_LEFT, "W"},
            {A333_DOWN, "E"}
        };
        break;

    case 74106:
        nearbyCheckpts = {
            {LABB_A330, "W"}, //SW
            {LABB_A331, "E"}, //SE
        };
        break;
    
    case 153070:
        nearbyCheckpts = {
            {LABB_A331, "W"},
            {A331_OUTISDE_LEFT, "W"},
            {A333_DOWN, "E"}
        };
        break;

    case 58080:
        nearbyCheckpts = {
            {A330_OUTSIDE_DOWN, "S"},
            {LABB_A330, "S"},
            {A331_OUTISDE_LEFT, "E"}
        };
        break;

    default:
        Serial.print("Dessa koordinater/ID finns inte: " + String(ID)); 
        break;
    }
    Serial.println("Node ID: " + String(ID));
    Serial.print("Checkpoints: ");
    for (int i = 0; i < nearbyCheckpts.size(); i++) {
        Serial.print(nearbyCheckpts[i].checkpt);
        Serial.print(", " + nearbyCheckpts[i].direction + "| ");
    }
}

String getCheckptDirection(CheckpointType checkpoint) {

    for (int i = 0; i < nearbyCheckpts.size(); i++) {

        if (nearbyCheckpts[i].checkpt == checkpoint) {
            return nearbyCheckpts[i].direction;
        }
    }
    return "Checkpoint has not been given to node";
}

void updateNodeDirection()
{

    if (atRoom)
    {

        checkRooms();
    }
    /* Test Purposej
    else 
    {
        Serial.println("Är vid trappor så ska inte byta riktning");
    }
    */
    //run a*
}


void checkRooms()
{
    int antalA = rooms[0].amountPeople; // A330
    int antalB = rooms[1].amountPeople; // A331
    int antalC = rooms[2].amountPeople; // A332
    int antalD = rooms[3].amountPeople; // A333

    // Om skillnaden mellan vänster och höger sida är mindre än 10
    if (abs((antalA + antalB) - (antalC + antalD)) < 10)
    {
        Serial.println("Normalt");
        // NORMALT LÄGE
        // Här ska du senare lägga in:
        // - Hitta alla noder med priority == true
        // - Kör A* för varje sådan nod
        // - Uppdatera nodens riktning
        // - Sätt priority = false
    }
    else
    {
        if (antalA > (antalB + 20))
        {
            if (locationInBuiling.indexOf("A331") != -1) 
            {
                Serial.println("de i A331 går till höger");
                direction = 'E'; //höger
            }
            else 
            {
                Serial.println("Inte rätt rum (1)");
            }
        }
        else
        {
            if (antalD > (antalC + 10))
            {
                Serial.println("de i A332 går till vänster");
                if (locationInBuiling.indexOf("A332") != -1) 
                {
                    direction = 'W'; //vänster
                }
                else 
                {
                    Serial.println("Inte rätt rum (2)");
                }
            }
        }
    }
}


void checkPeopleInRooms(String nodeStartedHere)
{
    freeToGo.clear();

    // Räkna totalt antal personer
    int totalPeople = 0;

    for (int i = 0; i < 4; i++) {
        totalPeople += rooms[i].amountPeople;
    }

    // Hitta vilket rum noden befinner sig i
    int currentRoom = -1;

    for (int i = 0; i < 4; i++) {
        if (rooms[i].roomName == nodeStartedHere) {
            currentRoom = i;
            break;
        }
    }

    if (currentRoom == -1) {
        return; // Hittade inte rummet

    }
    // Om det finns fler än 100 personer totalt
    if (totalPeople > 100) {
        manyPeople = true;

        // Lägg till rummet där noden befinner sig
        freeToGo.push_back(nodeStartedHere);
    }
    else {
        manyPeople = false;

        // Alla rum är tillåtna
        for (int i = 0; i < 4; i++) {
            freeToGo.push_back(rooms[i].roomName);
        }
    }
        // Grannar till vänster och höger
    int leftRoom = currentRoom - 1;
    int rightRoom = currentRoom + 1;

    // Om det finns färre än 5 personer i nuvarande rum
    if (rooms[currentRoom].amountPeople < 5) {

        if (leftRoom >= 0) {
            freeToGo.push_back(rooms[leftRoom].roomName);
        }

        if (rightRoom < 4) {
            freeToGo.push_back(rooms[rightRoom].roomName);
        }
    }
}