#include <iostream>
#include <ncurses.h>
#include <time.h>
#include "map.h" // 맵 배열 및 함수 사용 위해 인클루드
#include "position.h"
#ifndef SNAKE_H
#define SNAKE_H

extern int maxR;
extern int maxC;
extern bool go;
extern int growItems;
extern int poisonItems;
extern int gates;
extern position g1;
extern position g2;

int isEdge(position v);

class Element{
private:
  int r;
  int c;
  Element* next;
public:
  Element(int a, int b): r(a), c(b){next = 0;}
friend class Snake;
};

class Snake{
private:
  Element* head;   // SLL으로 몸체 구현
  clock_t mvSpan;
  int dir;   // head의 방향
  int size;
  position offset[4];   // 이동에 이용할 이동좌표

public:
  Snake();
  // 화면에 스네이크 출력
  void printsnake();
  // 스네이크 몸길이 1증가
  void addbody();
  // 스네이크 몸길이 1 감소 (꼬리부)
  void removebody();
  // 헤드가 바디에 접촉하는 경우
  bool isBody();
  // 헤드가 Wall에 접촉하는 경우
  bool isWall();
  // 헤드가 growthItem에 접촉하는 경우
  void isGrowthItem();
  // 헤드가 poisonItem에 접촉하는 경우
  void isPoisonItem();
  // 헤드가 gate에 접촉하는 경우
  bool isGate();
  // 스네이크 head의 방향을 얻어오는 함수
  int getdir(){return dir;}
  int getSize(){return size;}
  // 스네이크를 이동시키는 함수
  void move(int d);
  // Gate 통과 시 스네이크의 좌표와 방향 지정해주는 함수
  void passGate(position to);
// 스네이크 이동 제어 함수
friend void run(Snake& s);
};
#endif
