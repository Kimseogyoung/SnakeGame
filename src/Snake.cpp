#include <iostream>
#include <time.h>
#include <ncurses.h>
#include <conio.h>
using namespace std;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

struct position{int x, y;};
class Element{
private:
  int x;
  int y;
  Element* next;
public:
  Element(int r, int c): x(r), y(c){next = 0;}
};

class Snake{
private:
  Element* head;
  clock_t mvSpan;
  int dir;
  int size;
  position offset[4];
public:
  Snake(){
    head = new Element(10, 10);
    Element* ptr = head;
    for (int i = 0; i<2; i++){
      Element*
    }
    dir = 0;
    size = 3;
    mvSpan = clock();
    offset[0].x = -1;   offset[0].y = 0;   // UP
    offset[1].x = 0;   offset[1].y = 1;   // RIGHT
    offset[2].x = 0;   offset[2].y = -1;   // LEFT
    offset[3].x = 1;   offset[3].y = 0;   // DOWN
  }
  void move(int d){
    Element* p, q;
    Element* tmp = new Element(offset[d].x + head->x, offset[d].y + head->y);
    p = head;
    while (p->next){
      q = p;
      p = p->next;
    }
    q->next = 0;
    delete p;
    tmp->next = head;
    head = tmp;
    dir = d;
  }
friend void run(Snake& s);
};

void run(Snake& s){
  keypad(stdscr, TRUE);
  noecho();
  while (gameover){
    while (clock() - s.mvSpan < 500){
      if (khbit()){   // 키보드 입력 들어오면
        char key = getch();   // 키보드의 경우 224 75 이런 식으로 두 번 들어옴
        if (key = 224){   // 방향키이면
          key = getch();   // 방향키 종류 구별
          switch(key){
            case UP:
              s.move(0);
              break;
            case RIGHT:
              s.move(1);
              break;
            case LEFT:
              s.move(2);
              break;
            case DOWN:
              s.move(3);
              break;
          }
        }
      }
    }
    move(s.dir);
  }
}
