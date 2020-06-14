#include <iostream>
#include <ncurses.h>
#include <random>
#include <unistd.h>
#include "mission.h"
#ifndef DISPLAY_H
#define DISPLAY_H

extern int growItems;
extern int poisonItems;
extern int gates;
extern int snSize;
extern WINDOW *state_board;
extern WINDOW *mission_board;

void fancy_lighting(int);

void nextStageEffect(int num);

void State_board();

void Mission_board(mission m, mission_result p);
#endif
