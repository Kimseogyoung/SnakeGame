#include <iostream>
#include <time.h>
#include <ncurses.h>
#include "map.cpp" // 맵 배열 및 함수 사용 위해 인클루드
using namespace std;

struct position{int x, y;};
class Element{
private:
  int x;
  int y;
  Element* next;
public:
  Element(int r, int c): x(r), y(c){next = 0;}
friend class Snake;
};

class Snake{
private:
  Element* head;   // SLL으로 몸체 구현
  time_t mvSpan;
  int dir;   // head의 방향
  int size;
  position offset[4];   // 이동에 이용할 이동좌표
public:
  Snake(){
    head = new Element(10, 10);   // 초기 생성 위치
    dir = 0;
    size = 1;
    mvSpan = time(NULL);
    offset[0].x = -1;   offset[0].y = 0;   // UP
    offset[1].x = 0;   offset[1].y = 1;   // RIGHT
    offset[2].x = 0;   offset[2].y = -1;   // LEFT
    offset[3].x = 1;   offset[3].y = 0;   // DOWN
    addbody();
    addbody();
  }

  // 화면에 스네이크 출력
  void printsnake(){
    Element* p=head;
    while(p){
      map_array[p->x][p->y] = (p==head?3:4); // snake head = 3, snake body = 4로 배열값 변경
      p=p->next;
    }
    map();
  }

  // 스네이크 몸길이 1증가(머리부분)
  void addbody(){
    Element* p=head;
    while(p->next){
      p=p->next;
    }
    Element* tmp = new Element( p->x - offset[dir].x, p->y - offset[dir].y);
    p->next=tmp;
    size++;

  }
// 스네이크 몸길이 1 감소 (꼬리부)
  void removebody(){
    Element* p, *q;
    p = head;
    while (p->next){
      q = p;
      p = p->next;
    }
    q->next = 0;
    map_array[p->x][p->y] = 0;// 없어지는 body의 위치를 배열에서 0으로 변경
    delete p;// 맨 끝 삭제
  }

  bool isItem(){
    if(map_array[head->x][head->y]==5) return true;
    else return false;

  }
  bool isGate(){
    if(map_array[head->x][head->y]==6) return true;
    else return false;
  }
  // 스네이크 head의 방향을 얻어오는 함수
  int getdir(){return dir;}

  // 스네이크를 이동시키는 함수
  void move(int d){
    Element* p, *q;
    Element* tmp = new Element(offset[d].x + head->x, offset[d].y + head->y);
    p = head;
    while (p->next){
      q = p;
      p = p->next;
    }
    q->next = 0;
    map_array[p->x][p->y] = 0; // 없어지는 body의 위치를 배열에서 0으로 변경
    delete p;   // 맨 끝 삭제
    tmp->next = head;
    head = tmp;   // 새 몸 추가 및 head 변경
    dir = d;   // head의 방향 설정
  }

// 스네이크 이동 제어 함수
friend void run(Snake& s);
};

// nodelay()함수 통해 getch() 딜레이 없앰 + time(NULL) 이용해 시간 제한
void run(Snake& s){
  bool terminate = true;
  int key;
  int dir = s.dir;
  nodelay(stdscr, TRUE);
  while (terminate){
    while (time(NULL) - s.mvSpan < 0.5){
      key = getch();
      if (key == KEY_UP) dir = 0;
      else if (key == KEY_RIGHT) dir = 1;
      else if (key == KEY_LEFT) dir = 2;
      else if (key == KEY_DOWN) dir = 3;
    }
    s.move(dir);
    if(s.isItem()) s.addbody();
    if(s.isGate()) s.removebody();
    s.printsnake();
    s.mvSpan = time(NULL);
    refresh();
  }
}

int main()
{
  Snake snake;

  initscr(); //main window start
  setlocale(LC_ALL, "");
  resize_term(200, 400);
  mvprintw(2,15,"Snake Game"); // y,x
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK); // 글씨색, 배경색
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);
  run(snake);

  nodelay(stdscr, FALSE);
  getch();
  endwin();
  return 0;
}
