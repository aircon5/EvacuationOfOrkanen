#include "Algoritm.h"
#include <Arduino.h>
#include <vector>
#include "Map.h"
#include <stdint.h>
#include <queue>
#include <algorithm>
#include <cstdlib>


const int mapCells = mapWidth * mapHeight;
const uint16_t INF = UINT16_MAX; 

static uint16_t g_cost[mapCells];
static uint8_t cameFromPacked[(mapCells + 3) / 4];
//static bool visited[mapCells];

static void setDirection(int index, uint8_t direction) {

    int byteIndex = index / 4;
    int bitOffset = (index % 4) * 2;

    cameFromPacked[byteIndex] &= ~(0x03 << bitOffset);

    cameFromPacked[byteIndex] |=
        (direction & 0x03) << bitOffset;
}


static uint8_t getDirection(int index) {

    int byteIndex = index / 4;
    int bitOffset = (index % 4) * 2;

    return (cameFromPacked[byteIndex] >> bitOffset) & 0x03;
}

const uint8_t NO_PARENT = 255;

static const int dx[4] = {0, 1, 0, -1};
static const int dy[4] = {1, 0, -1, 0};

struct QueueHandle {
    uint16_t f;
    uint16_t g;
    uint16_t index;
};

static int toIndex(int x, int y) {
    return y * mapWidth + x;
}

static int getX(int index) {
    return index % mapWidth;
}

static int getY(int index) {
    return index / mapWidth;
}

static bool AvailableExit(int x, int y, exitChoice choice) {

    bool exitA = (x == 40 && y == 58);
    bool exitB = (x == 178 && y == 53);

    switch(choice) {
        case LEFT_EXIT:
            return exitA;

        case RIGHT_EXIT:
            return exitB;

        case BOTH_EXIT:
            return exitA || exitB;

        default:
            return false;
    }


}

static bool Reachable(int x, int y, exitChoice choice) {
    if (!(x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)) {
        return false;
    }
    Patch& p = mapOfOrkanen[y][x];

    if (p.tileType == WALL or p.doorFlag == DOOR_CLOSED) {
        return false;
    }
    if (p.tileType == EXIT) {
        return AvailableExit(x, y, choice);
    }
    return true;


}

struct CompareQueueHandle {
    bool operator()(const QueueHandle& a, const QueueHandle& b) const {
        return a.f > b.f;
    }
};

static uint16_t heuristic(int x, int y, exitChoice choice) {
    
    uint16_t best = INF;

    if(choice == LEFT_EXIT or choice == BOTH_EXIT) {
        uint16_t d = abs(x - 40) + abs(y - 58);
        best = std::min(best,d);
    }

    if (choice == RIGHT_EXIT or choice == BOTH_EXIT) {
        uint16_t d = abs(x - 178) + abs(y - 53);
        best = std::min(best,d);
    }

    return best;
}


void Astar(
    int startX,
    int startY,
    exitChoice exitChoice,
    std::vector<int>& path,
    std::vector<CheckpointType>& checkpointList
) {
    //Rensar tidigare arrays
    path.clear();
    checkpointList.clear();

    //Checkar om startvärden och mål är tillgängliga eller innanför kartan
    if (!Reachable(startX, startY, exitChoice)) {
        Serial.println("Koordinater utanför kartan eller oåtkomliga.");
        return;
    }

    for (int i = 0; i < mapCells; i++) {
        g_cost[i] = INF;
        //cameFrom[i] = NO_PARENT;
        //visited[i] = false;
    }

    std::priority_queue<
       QueueHandle,
       std::vector<QueueHandle>,
       CompareQueueHandle
       > openQueue;

       int startIndex = toIndex(startX, startY);

       g_cost[startIndex] = 0;

       openQueue.push({
            heuristic(startX, startY, exitChoice),
            0,
            (uint16_t)startIndex
        }
        );

       int target = -1;

       while (!openQueue.empty()) {

            QueueHandle currentNode = openQueue.top();
            openQueue.pop();

            int current = currentNode.index;

            if (currentNode.g != g_cost[current]) {
                continue;
            }
/*
            if (visited[current]) {
                continue;
            }
                

            visited[current] = true;
*/
            int x = getX(current);
            int y = getY(current);

            if (mapOfOrkanen[y][x].tileType == EXIT && AvailableExit(x,y,exitChoice)) {

                target = current;
                break;
            }
            
            for (int d = 0; d < 4; d++) {

                    int nX = x + dx[d];
                    int nY = y + dy[d];

                    if (!Reachable(nX, nY, exitChoice)){
                        continue;
                    }

                    int neighbor = toIndex(nX, nY);
/*
                    if (visited[neighbor]) {
                        continue;
                    }
                        */

                    uint16_t newCost = g_cost[current] + 1;

                    if (newCost < g_cost[neighbor]) {

                        g_cost[neighbor] = newCost;

                        setDirection(neighbor,(d + 2) % 4);

                        uint16_t f = newCost + heuristic(nX, nY, exitChoice);

                        openQueue.push({
                            f,
                            newCost,
                            (uint16_t)neighbor
                        });
                    }
            }
       }

       if (target == -1) {
           return;
       }

       int current = target;

       while (current != startIndex) {

            path.push_back(current);
            
            uint8_t direction = getDirection(current);

            if(direction == NO_PARENT) {
                path.clear();
                return;
            }

            int x = getX(current);
            int y = getY(current);

            x += dx[direction];
            y += dy[direction];

            current = toIndex(x,y);
            
       }

       path.push_back(startIndex);

       std::reverse(path.begin(), path.end());

       for (int index : path) {

        int x = getX(index);
        int y = getY(index);

        CheckpointType checkpt = (CheckpointType)mapOfOrkanen[y][x].checkpoint;

        if (checkpt != NO_CHECKPOINT) {

            if (std::find(
                checkpointList.begin(),
                checkpointList.end(),
                checkpt
            ) == checkpointList.end()) {

                checkpointList.push_back(checkpt);
            }
        }
    }
    
    return;
}