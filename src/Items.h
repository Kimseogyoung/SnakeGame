#include <iostream>
#include <ncurses.h>
#include <random>
#include <unistd.h>
#include <thread>
#include "position.h"
#include "map.h" // 맵 배열 및 함수 사용 위해 인클루드
#ifndef ITEMS_H
#define ITEMS_H

extern position g1;
extern position g2;
// extern int map_array[21][40];
// 아이템 생성 및 소멸 함수
void makeGrowItem(int i);
void makePoisonItem(int i);
void makeGate();
#endif
