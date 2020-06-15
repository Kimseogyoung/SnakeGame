#include <iostream>
#include <ncurses.h>
#include <chrono>
#include "map.h"
#include "position.h"
#ifndef SNAKE_H
#define SNAKE_H

extern int maxR;
extern int maxC;
extern bool gamerun;
extern bool go;
extern int growItems;
extern int poisonItems;
extern int gates;
extern position g1;
extern position g2;

/**
* @brief 가장자리 벽 여부 판단
* @param position v
* 검사할 좌표
* @return int
* 가장자리일 경우 해당 위치에 따라 0~3 리턴, 아닌 경우 4 리턴
* @author 이소영
*/
int isEdge(position v);

/**
* @brief Snake Head, Body 요소로 이용할 클래스
* @author 이소영
*/
class Element{
private:
  int r;
  int c;
  Element* next;
public:
  Element(int a, int b): r(a), c(b){next = 0;}
friend class Snake;
};

/**
* @brief Snake 저장 및 이동 함수 구현한 클래스
* @author
*/
class Snake{
private:
  Element* head;   // Single Linked List로 몸체 구현
  std::chrono::system_clock::time_point mvSpan;   // 이동 시간 제어를 위한 변수 
  int dir;   // head의 방향
  int size;
  position offset[4];   // 이동에 이용할 이동좌표

public:
  /**
  * @brief 클래스 생성자
  * @details 최초 스네이크 생성 및 offset 설정
  * @author 이소영
  */
  Snake();

  /**
  * @brief 클래스 소멸자
  * @author 이소영
  */
  ~Snake();

  /**
  * @brief 화면에 스네이크 출력
  * @author
  */
  void printsnake();

  /**
  * @brief 스네이크 몸길이 1증가
  * @author
  */
  void addbody();

  /**
  * @brief 스네이크 몸길이 1 감소 (꼬리부)
  * @author
  */
  void removebody();

  /**
  * @brief 헤드가 바디에 접촉하는 경우
  * @author
  */
  bool isBody();

  /**
  * @brief 헤드가 Wall에 접촉하는 경우
  * @author
  */
  bool isWall();

  /**
  * @brief 헤드가 growthItem에 접촉하는 경우
  * @author
  */
  void isGrowthItem();

  /**
  * @brief 헤드가 poisonItem에 접촉하는 경우
  * @author
  */
  void isPoisonItem();

  /**
  * @brief 헤드가 gate에 접촉하는 경우
  * @author
  */
  void isGate();

  int getdir(){return dir;}
  int getSize(){return size;}

  /**
  * @brief 스네이크를 이동시키는 함수
  * @author 이소영
  */
  void move(int d);

  /**
  * @brief  Gate 통과 시 스네이크의 좌표와 방향 지정해주는 함수
  * @author 이소영
  */
  void passGate(position to);

// 스네이크 이동 제어 함수
friend void run(Snake& s);
};
#endif
