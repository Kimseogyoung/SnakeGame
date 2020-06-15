#include <iostream>
#include "Snake.h"
using namespace std;

// 가장자리 벽인지 체크
int isEdge(position v){
  if ((v.r == 0) && (v.c < maxC)) return 2;   // UPPER EDGE
  else if ((v.r < maxR) && (v.c == maxC-1)) return 3;   // RIGHT EDGE
  else if ((v.r == maxR-1) && (v.c < maxC)) return 0;   // LOWER EDGE
  else if ((v.r < maxR) && (v.c == 0)) return 1;   // LEFT EDGE
  else return 4;
}

Snake::Snake(){
  head = new Element(10, 10);   // 초기 생성 위치
  dir = 0;
  size = 1;
  mvSpan = chrono::system_clock::now();
  offset[0].r = -1;   offset[0].c = 0;   // UP
  offset[1].r = 0;   offset[1].c = 1;   // RIGHT
  offset[2].r = 1;   offset[2].c = 0;   // DOWN
  offset[3].r = 0;   offset[3].c = -1;   // LEFT
  addbody();
  addbody();
}

Snake::~Snake(){
  Element* tmp;
  while (head){
    tmp = head;
    head = head->next;
    delete tmp;
  }
}

// 화면에 스네이크 출력
void Snake::printsnake(){
  Element* p=head;
  while(p){
    map_array[p->r][p->c] = (p==head?3:4);   // snake head = 3, snake body = 4로 배열값 변경
    p=p->next;
  }
  map();
}

// 스네이크 몸길이 1증가
void Snake::addbody(){
  Element* p=head;
  while(p->next){
    p=p->next;
  }
  if (map_array[p->r - offset[dir].r][p->c - offset[dir].c] == 0){
    Element* tmp = new Element( p->r - offset[dir].r, p->c - offset[dir].c);
    p->next=tmp;
    size++;
  }
  // 몸이 길어지는 부분이 벽이거나 게이트라면 멈추기
  else if (map_array[p->r - offset[dir].r][p->c - offset[dir].c] == 1 || map_array[p->r - offset[dir].r][p->c - offset[dir].c] == 7){gamerun = false; go = false;}

  // 몸이 길어지는 부분에 poison 아이템이 있으면
  else if (map_array[p->r - offset[dir].r][p->c - offset[dir].c] == 6) removebody();
  else if (map_array[p->r - offset[dir].r][p->c - offset[dir].c] == 5) addbody();
}

// 스네이크 몸길이 1 감소 (꼬리부)
void Snake::removebody(){
  Element* p, *q;
  p = head;
  while (p->next){
    q = p;
    p = p->next;
  }
  q->next = 0;
  map_array[p->r][p->c] = 0;
  delete p;
  size--;
}

// 헤드가 바디에 접촉
bool Snake::isBody(){
  if(map_array[head->r][head->c]==4){return true;}
  return false;
}
//헤드가 벽에 접촉
bool Snake::isWall(){
  if(map_array[head->r][head->c]==1){return true;}
  return false;
}
// 헤드가 growthItem에 접촉
void Snake::isGrowthItem(){
  if(map_array[head->r][head->c]==5){
    addbody();
    growItems++;
  }
}
// 헤드가 poisonItem에 접촉
void Snake::isPoisonItem(){
  if(map_array[head->r][head->c]==6){
    removebody();
    poisonItems++;
  }
}
// 헤드가 gate에 접촉
void Snake::isGate(){
  if(map_array[head->r][head->c]==7) {
    if (head->r == g1.r) passGate(g2);
    else passGate(g1);
    gates++;
  }
}

// 스네이크를 이동시키는 함수
void Snake::move(int d){
  Element* p, *q;
  Element* tmp = new Element(offset[d].r + head->r, offset[d].c + head->c);
  p = head;
  while (p->next){
    q = p;
    p = p->next;
  }

  q->next = 0;
  map_array[p->r][p->c] = 0;
  delete p;   // 맨 끝 삭제
  tmp->next = head;
  head = tmp;   // 새 몸 추가 및 head 변경
  dir = d;   // head의 방향 설정
}

// 게이트 통과 시 방향 설정 및 이동
void Snake::passGate(position to){
  int next_dir[4] = {dir, (dir+1) % 4, (dir+2) % 4, (dir+6) % 4};
  int idx = 0;
  if (isEdge(to) < 4){
    int ndir = isEdge(to);
    head->r = to.r + offset[ndir].r;
    head->c = to.c + offset[ndir].c;
    dir = ndir;
  }
  else{
    while (map_array[offset[next_dir[idx]].r + to.r][offset[next_dir[idx]].c + to.c] == 1) idx++;
    head->r = offset[next_dir[idx]].r + to.r;
    head->c = offset[next_dir[idx]].c + to.c;
    dir = next_dir[idx];
  }
}
