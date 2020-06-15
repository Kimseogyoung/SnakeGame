#include <iostream>
#include <ncurses.h>
#include <random>
#include <unistd.h>
#include <clocale>
#include "mission.h"
#ifndef DISPLAY_H
#define DISPLAY_H

extern int growItems;
extern int poisonItems;
extern int gates;
extern int snSize;
extern int runtime;
extern WINDOW *state_board;
extern WINDOW *mission_board;

/**
* @brief 게임 시작, 성공, 실패 화면 출력 함수
* @param int n
* 화면 종류 판단을 위한 매개변수
* @author 김서경
*/
void fancy_lighting(int n);

/**
* @brief 스테이지 전환 화면 출력 함수
* @param int num
* 스테이지 판단을 위한 매개변수
* @author 김서경
*/
void nextStageEffect(int num);

/**
* @brief score board 출력 함수
* @author 김서경
*/
void State_board();

/**
* @brief mission board 출력 함수
* @param mission m
*
* @param mission_result p
* 
* @author 김서경
*/
void Mission_board(mission m, mission_result p);
#endif
