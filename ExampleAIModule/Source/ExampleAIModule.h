#pragma once
#include"../../DAPI/Include/DAPI.h"
#include<vector>
#include<random>
#include<chrono>

extern DAPI::PlayerCharacter my_player;
extern std::mt19937 mt;

extern int getRandomInteger(int min, int max);