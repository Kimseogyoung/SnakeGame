#include <iostream>
#include <ncurses.h>
#include <random>
#include <unistd.h>
#include <thread>
#include "position.h"
#include "map.h"
#ifndef ITEMS_H
#define ITEMS_H

extern position g1;
extern position g2;

// 아이템 생성 및 소멸 함수

/**
* @brief grow Item 생성 및 관리
* @author 이아연(100%)
*/
void makeGrowItem();
/**
* @brief poison Item 생성 및 관리
* @author 이아연(100%)
*/
void makePoisonItem();
/**
* @brief gate 생성 및 관리
* @author 이아연(100%)
*/
void makeGate();
#endif
