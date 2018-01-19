#pragma once

#include <math.h>

#define M_PI       3.14159265358979323846   // pi

const int SCREEN_W = 1600;
const int SCREEN_H = 900;

const int HALF_SW = SCREEN_W >> 1;
const int HALF_SH = SCREEN_H >> 1;

// player vitals
const float PLAYER_HEALTH = 100;
const float PLAYER_SIZE = 15;
const float PLAYER_MINDISTANCE = 10.0f;

// player movement
const float PLAYER_MAXSPEED = 2.0f;
const float PLAYER_MAXACCEL = 1.5f;
const float PLAYER_LIVELYNESS = 3.0f;
const float PLAYER_SIGHT = 40.0f;

// companion vitals
const float BUDDY_HEALTH = 100;
const float BUDDY_SIZE = 20;
const float BUDDY_MINDISTANCE = 10.0f;

// companion movement
const float BUDDY_MAXSPEED = 2.0f;
const float BUDDY_MAXACCEL = 4.0f;
const float BUDDY_SIGHT = 50.0f;

// enemy vitals
const float ENEMY_HEALTH = 100;
const float ENEMY_SIZE = 20;
const float ENEMY_MINDISTANCE = 10.0f;

// enemy movement
const float ENEMY_MAXSPEED = 2.0f;
const float ENEMY_MAXACCEL = 4.0f;
const float ENEMY_SIGHT = 50.0f;