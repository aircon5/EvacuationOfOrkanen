#ifndef ALGORTIM_H
#define ALGORTIM_H

#include <Arduino.h>
#include <vector>
#include "Map.h"

enum exitChoice : uint8_t {
    LEFT_EXIT,
    RIGHT_EXIT,
    BOTH_EXIT
};

bool Astar(
    int startX,
    int startY,
    exitChoice exitChoice,
    std::vector<int>& path,
    std::vector<CheckpointType>& checkpointList
);

#endif