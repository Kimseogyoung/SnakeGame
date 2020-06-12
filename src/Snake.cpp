#include <iostream>
#include <time.h>
#include <ncurses.h>
#include "map.cpp" // 맵 배열 및 함수 사용 위해 인클루드
#include "mission.cpp" //미션 클래스
#include "map_array.cpp"
#include <random>
#include <unistd.h>
#include <thread>
#include<algorithm>//copy함수

using namespace std;

/////////////////////////////전역변수
int snSize=0;
int growItems=0;
int poisonItems=0;
int gates=0;

int maxR = 21;
int maxC = 40;

bool go = true;
bool gamerun=true;

int stageLevel=1;
int nowMap=0;
/////////////////////////////////서브윈도우
WINDOW *state_board;
WINDOW *mission_board;
//////////////////////////////
void nextStageEffect(int num){
  clear();
  bkgd(COLOR_PAIR(6));
  mvprintw(5,10,"Stage < %d >",num);
  mvprintw(6,10,"Press any key to start.");
  refresh();
  nodelay(stdscr, FALSE);
  getch();
};
void gameover(){
  clear(); bkgd(COLOR_PAIR(9));  mvprintw(10,30,"FAIL");
  nodelay(stdscr, FALSE); getch();
}

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
void Mission_board(mission m, mission_result p){//미션보드


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
  if ((v.r == 0) && (v.c < maxC)) return 2;   // UPPER EDGE
  else if ((v.r < maxR) && (v.c == maxC-1)) return 3;   // RIGHT EDGE
  else if ((v.r == maxR-1) && (v.c < maxC)) return 0;   // LOWER EDGE
  else if ((v.r < maxR) && (v.c == 0)) return 1;// LEFT EDGE
  else return 4;
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
    g1.r = 0;
    g1.c = 30;
    g2.r = 15;
    g2.c = 20;
    addbody();
    addbody();
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
    if (map_array[p->r - offset[dir].r][p->c - offset[dir].c] == 0){
      Element* tmp = new Element( p->r - offset[dir].r, p->c - offset[dir].c);
      p->next=tmp;
      snSize++;
    }
    // 몸이 길어지는 부분이 벽이거나 게이트라면 멈추기
    else if (map_array[p->r - offset[dir].r][p->c - offset[dir].c] == 1 || map_array[p->r - offset[dir].r][p->c - offset[dir].c] == 7) go = false;

    // 몸이 길어지는 부분에 poison 아이템이 있으면
    else if (map_array[p->r - offset[dir].r][p->c - offset[dir].c] == 6) removebody();
    else if (map_array[p->r - offset[dir].r][p->c - offset[dir].c] == 5) addbody(); //재귀함수니까 마지막에
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
  bool isBody(){//헤드가 바디에 접촉
    if(map_array[head->r][head->c]==4){return true;}
    return false;
  }
  bool isWall(){//헤드가 바디에 접촉
    if(map_array[head->r][head->c]==1){return true;}
    return false;
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
      if (head->r == g1.r) passGate(g2);
      else passGate(g1);
      gates++;
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

// 스네이크 이동 제어 함수
friend void run(Snake& s);
};



// 아이템 개수를 판단하는 함수 - 한번에 나올 수 있는 아이템의 수는 3개 이하
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

      if (!map_array[x][y]) map_array[x][y] = 5;

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

      if (!map_array[x][y]) map_array[x][y] = 6;
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
  uniform_int_distribution<int> b(3,7);
  sleep(b(gen));

  while(go){
      uniform_int_distribution<int> r(0, 21); // wall 아닌 부분에서 x좌표 선택
      uniform_int_distribution<int> c(0, 40); // wall 아닌 부분에서 y좌표 선택

      int x1 = r(gen);
      int y1 = c(gen);
      int x2 = r(gen);
      int y2 = c(gen);
      if ((x1 == x2) && (y2 == y2)) {x2 = r(gen); y2 = c(gen);}

      int prev_g1;
      int prev_g2;

      if ((map_array[x1][y1] == 0 || map_array[x1][y1] == 1) && (map_array[x2][y2] == 0 || map_array[x2][y2] == 1)){
        prev_g1 = map_array[x1][y1];
        prev_g2 = map_array[x2][y2];
        map_array[x1][y1] = 7; map_array[x2][y2] = 7;
      }
      g1.r = x1; g1.c = y1;
      g2.r = x2; g2.c = y2;

      // uniform_int_distribution<int> p(5, 7); // 아이템 출현 유지 기간 (11초~15초) 랜덤
      // sleep(p(gen));
      sleep(7);
      map_array[x1][y1] = prev_g1;
      map_array[x2][y2] = prev_g2;

      sleep(b(gen));
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


    mission m(stageLevel);//미션 초기화         &&&&&&&&&&&&&&&&후에 스테이지전환 기능 추가 필요
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

          // s.isBody() 임시로 bool return
        if (s.isBody()) { gamerun=false; go = false; }
        if(s.isWall()) { gamerun=false; go = false;}
        s.isGrowthItem();
        s.isPoisonItem();
        s.isGate();

        m.isMissoncomplete(ms,snSize, growItems,poisonItems,gates);//미션 성공인지
        if(ms.leng==1 && ms.gitem==1 && ms.pitem==1 && ms.gate ==1){//미션 모두 완료
          go=false;
          stageLevel++;
        }
        s.printsnake();
        State_board();
        Mission_board(m,ms);

        s.mvSpan = clock();
        refresh();
    }
    // go == false이면 스테이지 종{}
    if (go == false) {
      t1.join(); t2.join(); t3.join(); t4.join(); t5.join(); //쓰레드 종료 -> 터미널 오류x
    }


}

int main()
{

  setlocale(LC_ALL, "");
  initscr(); //main window start
  resize_term(200, 400);
  start_color();
  bkgd(COLOR_PAIR(1));
  mvprintw(2,15,"Snake Game"); // y,x
  init_pair(1, COLOR_WHITE, COLOR_WHITE);
  init_pair(2, COLOR_BLACK, COLOR_BLACK);   // 글씨색, 배경색 > 기본 벽
  init_pair(3, COLOR_BLACK, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_WHITE);
  init_pair(5, COLOR_CYAN, COLOR_WHITE);
  init_pair(6, COLOR_GREEN, COLOR_WHITE);   // growItems
  init_pair(7, COLOR_RED, COLOR_WHITE);
  init_pair(8, COLOR_MAGENTA, COLOR_WHITE);
  init_pair(9, COLOR_BLACK, COLOR_RED); // 끝났을때 배경

  state_board=newwin(7, 40, 3, 53);//서브윈도우 위치설정
  mission_board=newwin(7, 40, 10, 53);


  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  stageLevel=1;
  //게임시작
  while(gamerun && stageLevel<=4){
    Snake snake;
    go=true;
    nextStageEffect(stageLevel);
    //맵 랜덤 결정
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> mapnum(0, 7);
    nowMap=mapnum(gen);

    //맵 초기화
    for(int i=0; i<maxR;i++)
      for(int j=0; j<maxC;j++)
        map_array[i][j]=mapList[nowMap][i][j];

    run(snake);
    //clear(); bkgd(COLOR_PAIR(9));  mvprintw(10,30,"FAIL");
  }
  if(gamerun==false && stageLevel<=4)//게임오버
    gameover();
  else if(gamerun==false && stageLevel>=5)//성공



  nodelay(stdscr, FALSE);
  getch();
  endwin();
  return 0;
}
