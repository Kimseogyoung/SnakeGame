#include <iostream>
#include <ncurses.h>
#include <locale.h>
#ifndef MAP_H
#define MAP_H

extern int map_array[21][40];
extern WINDOW *gamewin;
/**
* @brief 게임 윈도우 출력 함수
* @author
*/
void map();
#endif
