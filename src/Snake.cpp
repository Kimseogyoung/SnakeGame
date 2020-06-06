#include <iostream>
#include <time.h>
#include <ncurses.h>
#include "map.cpp" // 맵 배열 및 함수 사용 위해 인클루드
#include <random>
#include <unistd.h>
#include <thread>
//#include "board.cpp"//스코어보드 함수 사용위해 인클루드  했지만 사실 다른 파일에 굳이 없어도될거같기도하고...
using namespace std;

#define UEDGE = 1;
#define REDGE = 2;
#define DEDGE = 3;
#define LEDGE = 4;
/////////////////////////////전역변수
int snSize=0;
int growItems=0;
int poisonItems=0;
int gates=0;
int maxR = 21;
int maxC = 40;
bool go = true;

/////////////////////////////////서브윈도우
WINDOW *state_board;
WINDOW *mission_board;
//////////////////////////////



void State_board(){//스코어 보드 프린트함수

  //WINDOW *state_board=newwin(7, 40, 3, 53);

  wborder(state_board,'|','|','-','-','+','+','+','+');

  mvwprintw(state_board , 1, 1, "State board");
  mvwprintw(state_board , 2, 1, "current length / max length : %d / %d",snSize,20);
  mvwprintw(state_board , 3, 1, "growthItems : %d",growItems);
  mvwprintw(state_board , 4, 1, "poisonItems : %d",poisonItems);
  mvwprintw(state_board , 5, 1, "gates : %d",gates);
  mvwprintw(state_board , 5, 1, "time : %d",0);

  //wborder(state_board,"\u2500","\u2500","\u2502","\u2502","\u250C","\u2510","\u2514","\u2518");
  wrefresh(state_board);
}
void Mission_board(mission m, mission_result p){//미션보드 미구현


  wborder(mission_board,'|','|','-','-','+','+','+','+');

  mvwprintw(mission_board , 1, 1, "Mission_board");
  mvwprintw(mission_board, 2, 1, "mission 1 : size >= %d (%d)",m.getLeng(),p.leng);
  mvwprintw(mission_board, 3, 1, "mission 2 : growthItem >= %d (%d)",m.getgItem(),p.gitem);
  if(m.getLock()) mvwprintw(mission_board, 4, 1, "mission 3 : poisonItem <= %d (%d)",m.getpItem(),p.pitem);
  else mvwprintw(mission_board, 4, 1, "mission 3 : poisonItem >= %d (%d)",m.getpItem(),p.pitem);
  mvwprintw(mission_board, 5, 1, "mission 4 : gate >= %d (%d)",m.getGate(),p.gate);

  wrefresh(mission_board);
}

struct position{int r, c;};
///////////////////////////////////////////////////////////////////////////
position g1;
position g2;
///////////////////////////////////////////////////////////////////////////
int isEdge(position v){
  int result = 0;
  if ((v.r == 0) && (v.c < maxC)) result = 1;   // UPPER EDGE
  else if ((v.r < maxR) && (v.c == maxC-1)) result = 2;   // RIGHT EDGE
  else if ((v.r == maxR-1) && (v.c < maxC)) result = 3;   // LOWER EDGE
  else if ((v.r < maxR) && (v.c == 0)) result = 4;// LEFT EDGE
  else {
    mvprintw( 30, 70, "NO");
    result= 0;
  }
  return result;
}
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
  position offset[4];   // 이동에 이용할 이동좌표


public:
  Snake(){
    head = new Element(10, 10);   // 초기 생성 위치
    dir = 0;
    snSize = 1;
    mvSpan = clock();
    offset[0].r = -1;   offset[0].c = 0;   // UP
    offset[1].r = 0;   offset[1].c = 1;   // RIGHT
    offset[2].r = 1;   offset[2].c = 0;   // DOWN
    offset[3].r = 0;   offset[3].c = -1;   // LEFT
    addbody();
    addbody();
    g1.r = 0;
    g1.c = 30;
    g2.r = 15;
    g2.c = 20;
  }
  // 화면에 스네이크 출력
  void printsnake(){
    Element* p=head;
    while(p){
      map_array[p->r][p->c] = (p==head?3:4); // snake head = 3, snake body = 4로 배열값 변경
      p=p->next;
    }
    map();
  }

  // 스네이크 몸길이 1증가
  void addbody(){
    Element* p=head;
    while(p->next){
      p=p->next;
    }
    Element* tmp = new Element( p->r - offset[dir].r, p->c - offset[dir].c);
    p->next=tmp;
    snSize++;

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
    map_array[p->r][p->c] = 0;// 없어지는 body의 위치를 배열에서 0으로 변경
    delete p;// 맨 끝 삭제
    snSize--;
  }
  void isBody(){//헤드가 바디에 접촉 //////////////////////////////////아직 미구현
    if(map_array[head->r][head->c]==4){
      go = false;
    }
  }
  void isWall(){//헤드가 바디에 접촉 //////////////////////////////////아직 미구현
    if(map_array[head->r][head->c]==1){
      go = false;
    }
  }
  void isGrowthItem(){//헤드가 growthItem접촉
    if(map_array[head->r][head->c]==5){
      addbody();
      growItems++;
    }
  }
  void isPoisonItem(){//헤드가 poisonitem접촉
    if(map_array[head->r][head->c]==6){
      removebody();
      poisonItems++;
    }
  }
  bool isGate(){//헤드가 gate접촉
    if(map_array[head->r][head->c]==7) {
      //미구현
      if (head->r == g1.r) passGate(g2);
      else passGate(g1);
      return true;
    }
    return false;
  }
  // 스네이크 head의 방향을 얻어오는 함수
  int getdir(){return dir;}

  // 스네이크를 이동시키는 함수
  void move(int d){
    Element* p, *q;
    Element* tmp = new Element(offset[d].r + head->r, offset[d].c + head->c);
    p = head;
    while (p->next){
      q = p;
      p = p->next;
    }

    q->next = 0;
    map_array[p->r][p->c] = 0; // 없어지는 body의 위치를 배열에서 0으로 변경
    delete p;   // 맨 끝 삭제
    tmp->next = head;
    head = tmp;   // 새 몸 추가 및 head 변경
    dir = d;   // head의 방향 설정
  }

  void passGate(position to){
    int next_dir[4] = {dir, (dir+1) % 4, (dir+2) % 4, (dir+6) % 4};
    int idx = 0;
    if (isEdge(to) > 0){
      switch (isEdge(to)){
      case 1:
        head->r = to.r + 1;
        head->c = to.c;
        dir = 2;
        break;
      case 2:
        head->r = to.r;
        head->c = to.c -1;
        dir = 3;
        break;
      case 3:
        head->r = to.r - 1;
        head->c = to.c;
        dir = 0;
        break;
      case 4:
        head->r = to.r;
        head->c = to.c + 1;
        dir = 1;
        break;
      }
    }
    else{
      while (map_array[offset[next_dir[idx]].r + to.r][offset[next_dir[idx]].c + to.c] == 1) idx++;
      head->r = offset[next_dir[idx]].r + to.r;
      head->c = offset[next_dir[idx]].c + to.c;
      dir = next_dir[idx];
    }
  }

// 스네이크 이동 제어 함수
friend void run(Snake& s);
};



//아이템 개수를 판단하는 함수 - 한번에 나올 수 있는 아이템의 수는 3개 이하
int itemCnt(){
  int itemcnt = 0;
  for(int r=1; r < 20; r++){
    for(int c=1; c < 39; c++){
      if (map_array[r][c] == 5 || map_array[r][c] == 6) itemcnt++;
    }
  }
  return itemcnt;
}

// 아이템 생성 및 소멸 함수
void makeGrowItem(){
  random_device rd; // 시드값을 얻음 - srand보다 완벽한 무작위
  mt19937 gen(rd()); // random_device 난수 생성 엔진 초기화
  uniform_int_distribution<int> b(3,7); // 첫 아이템 출현 시간 (3초~7초) 랜덤
  sleep(b(gen));

  while(go){
    int cnt = itemCnt();
    if(cnt < 3){
      random_device rd; // 시드값을 얻음 - srand보다 완벽한 무작위
      mt19937 gen(rd()); // random_device 난수 생성 엔진 초기화

      uniform_int_distribution<int> r(1, 19); // wall 아닌 부분에서 x좌표 선택
      uniform_int_distribution<int> c(1, 38); // wall 아닌 부분에서 y좌표 선택

      int x = r(gen);
      int y = c(gen);

      if ((map_array[x][y] != 3 || map_array[x][y] == 4) || (map_array[x][y] != 6 || map_array[x][y] != 5)){
        map_array[x][y] = 5;
      }

      uniform_int_distribution<int> p(11, 15); // 아이템 출현 유지 기간 (11초~15초) 랜덤
      sleep(p(gen));
      map_array[x][y] = 0;

      sleep(b(gen)); // 다음 아이템까지 쉬는 시간(3초 ~ 7초) 랜덤적으로
    }
    else continue;
  }
}

void makePoisonItem(){
  random_device rd; // 시드값을 얻음 - srand보다 완벽한 무작위
  mt19937 gen(rd()); // random_device 난수 생성 엔진 초기화
  uniform_int_distribution<int> b(3,7);
  sleep(b(gen));

  while(go){
    int cnt = itemCnt();;
    if(cnt < 3){

      uniform_int_distribution<int> r(1, 19); // wall 아닌 부분에서 x좌표 선택
      uniform_int_distribution<int> c(1, 38); // wall 아닌 부분에서 y좌표 선택

      int x = r(gen);
      int y = c(gen);

      if ((map_array[x][y] != 3 || map_array[x][y] == 4) || (map_array[x][y] != 6 || map_array[x][y] != 5)){
        map_array[x][y] = 6;
      }
      uniform_int_distribution<int> p(11, 15); // 아이템 출현 유지 기간 (11초~15초) 랜덤
      sleep(p(gen));
      map_array[x][y] = 0;

      sleep(b(gen));
    }
    else continue;
  }
}

void makeGate(){
  random_device rd; // 시드값을 얻음 - srand보다 완벽한 무작위
  mt19937 gen(rd()); // random_device 난수 생성 엔진 초기화
  uniform_int_distribution<int> b(7,10);
  sleep(b(gen));

  while(go){
    int cnt = itemCnt();;
    if(cnt < 3){

      uniform_int_distribution<int> r(0, 21); // wall 아닌 부분에서 x좌표 선택
      uniform_int_distribution<int> c(0, 40); // wall 아닌 부분에서 y좌표 선택

      int x1 = r(gen);
      int y1 = c(gen);
      int x2 = r(gen);
      int y2 = c(gen);
      if ((x1 == x2) && (y2 == y2)) {x2 = r(gen); y2 = c(gen);}


      if ((map_array[x1][y1] != 3 || map_array[x1][y1] == 4) || (map_array[x1][y1] != 6 || map_array[x1][y1] != 5)
    && (map_array[x2][y2] != 3 || map_array[x2][y2] == 4) || (map_array[x2][y2] != 6 || map_array[x2][y2] != 5)){
        map_array[x1][y1] = 7; map_array[x2][y2] = 7;
      }
      g1.r = x1; g1.c = y1;
      g2.r = x2; g2.c = y2;

      uniform_int_distribution<int> p(11, 15); // 아이템 출현 유지 기간 (11초~15초) 랜덤
      sleep(p(gen));
      map_array[x1][y1] = 0;
      map_array[x2][y2] = 0;

      sleep(b(gen));
    }
    else continue;
  }
}

// nodelay()함수 통해 getch() 딜레이 없앰 + time(NULL) 이용해 시간 제한
void run(Snake& s){
  int key;
  int dir = s.dir;
  thread t1(makeGrowItem);
  thread t2(makePoisonItem);
  thread t3(makeGrowItem);
  thread t4(makePoisonItem);
  thread t5(makeGate);

  mission m(1);
  m.set_mission();
  mission_result ms={0,0,0,0};


  nodelay(stdscr, TRUE);
  while (go){
    dir = s.dir;
    while (clock() - s.mvSpan < 300000){
      key = getch();
      if (key == KEY_UP) dir = 0;
      else if (key == KEY_RIGHT) dir = 1;
      else if (key == KEY_DOWN) dir = 2;
      else if (key == KEY_LEFT) dir = 3;

    }
    s.move(dir);

    s.isBody();/////////////////////////
    s.isGrowthItem();
    s.isPoisonItem();
    s.isGate();//////////////////////////////////

    m.isMissoncomplete(ms,snSize, growItems,poisonItems,gates);


    s.printsnake();
    State_board();
    Mission_board(m,ms);

    s.mvSpan = clock();
    refresh();
  }
}

int main()
{
  Snake snake;
setlocale(LC_ALL, "");
  initscr(); //main window start

  resize_term(200, 400);
  mvprintw(2,15,"Snake Game"); // y,x
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK); // 글씨색, 배경색
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

  state_board=newwin(7, 40, 3, 53);//서브윈도우 위치설정
  mission_board=newwin(7, 40, 10, 53);


  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);
  run(snake);

  nodelay(stdscr, FALSE);
  getch();
  endwin();
  return 0;
}
