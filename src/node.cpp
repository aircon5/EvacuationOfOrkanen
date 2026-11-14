#include "node.h"
#include "Map.h"
#include "Algoritm.h"
#include <Arduino.h>
#include <string>
#include <unordered_map>

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


room rooms [4] = {
    {"A330", 0, false, false},
    {"A331", 0, true, false},
    {"A332", 20, true, false},
    {"A333", 55, false, false}
};

std::unordered_map<int, String> locations =
{
    {044082, "dörrA330"},
    {055062, "korridorMotTrappaVänster"},
    {100079, "dörrA331"},
    {160061, "korridorMotTrappaHöger"},
    {099071, "framförA331"},
    {128070, "framförA332"},
    {741006, "dörrMellanA330oA331"},
    {153070, "framförA333"},
    {058080, "korridor"},
    {025060, "vån2TrappaVänster"},
    {043060, "vån3TrappaVänster"},
    {178054, "vån3TrappaHöger"},
    {199066, "vån2TrappaHöger"},
    {128078, "dörrA332"},
    {153078, "dörrA333"}
};



String setupNode(int x, int y)
{
    xcoord = x;
    ycoord = y;
    String combined = String(xcoord) + String(ycoord);
    int xy = combined.toInt();
    auto it = locations.find(xy);

    if (it != locations.end()) {
        locationInBuiling = it->second;
        if (locationInBuiling.indexOf("A33") != -1) {atRoom = true;}
    } else {
        locationInBuiling = "Okänd plats";
    }
    return locationInBuiling;
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