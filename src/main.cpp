#include <iostream>
#include "mission.h" //미션 클래스
#include "display.h"
#include "Snake.h"
#include "Items.h"
#include "map.h"
#include "position.h"
using namespace std;

/////////////////////////////전역변수
int growItems=0;
int poisonItems=0;
int gates=0;
int snSize=0;

bool go = true;
bool gamerun=true;

int stageLevel=1;
int nowMap=0;

int maxR = 21;
int maxC = 40;

position g1;
position g2;

// extern int map_array[21][40];
extern int mapList[8][21][40];
/////////////////////////////////서브윈도우
WINDOW *state_board;
WINDOW *mission_board;
//////////////////////////////

void checkTime(){
  while(go){
    sleep(0.5);
  }
}

// nodelay()함수 통해 getch() 딜레이 없앰 + time(NULL) 이용해 시간 제한
void run(Snake& s){
  int key;
  int dir = s.dir;

  mission m(stageLevel);//미션 초기화
  m.set_mission();
  mission_result ms={0,0,0,0};

  nodelay(stdscr, TRUE);
  while (go){
    dir = s.dir;
    chrono::duration<double> sp;
    sp = chrono::system_clock::now()-s.mvSpan;
    while (sp.count() < 0.5){
      key = getch();
      if (key == KEY_UP) dir = 0;
      else if (key == KEY_RIGHT) dir = 1;
      else if (key == KEY_DOWN) dir = 2;
      else if (key == KEY_LEFT) dir = 3;
      sp = chrono::system_clock::now()-s.mvSpan;
    }
    //mvprintw(200, 80, "%f", (double)(clock()-s.mvSpan) / CLOCKS_PER_SEC);
    s.move(dir);

    // s.isBody() 임시로 bool return
    if (s.isBody()) { gamerun=false; go = false; }
    if (s.isWall()) { gamerun=false; go = false;}
    s.isGrowthItem();
    s.isPoisonItem();
    s.isGate();
    if (s.getSize() < 3){ gamerun=false; go=false;}

    m.isMissoncomplete(ms,s.getSize(), growItems,poisonItems,gates);//미션 성공인지
    if(ms.leng==1 && ms.gitem==1 && ms.pitem==1 && ms.gate ==1){//미션 모두 완료
      go=false;
      stageLevel++;
    }
    s.printsnake();
    snSize = s.getSize();
    State_board();
    Mission_board(m,ms);

    s.mvSpan = chrono::system_clock::now();
    refresh();
  }
}

int main(){
  setlocale(LC_ALL, "");
  initscr(); //main window start
  resize_term(400, 600);
  start_color();
  bkgd(COLOR_PAIR(1));

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
  gamewin = newwin(21, 40, 3, 3); //int nlines, int ncols, int begy, int begx
  wbkgd(gamewin, COLOR_PAIR(1));


  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  fancy_lighting(1);

  stageLevel=1;
  //게임시작
  while(gamerun && stageLevel<=4){

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
    // 현재 상태 초기화
    growItems=0;
    poisonItems=0;
    gates=0;
    snSize=0;
    Snake snake = Snake();
    g1.r = 0; g1.c = 0;
    g2.r = 0; g2.c = 0;
    thread t1(makeGrowItem);
    thread t2(makePoisonItem);
    thread t3(makeGrowItem);
    thread t4(makePoisonItem);
    thread t5(makeGate);
    run(snake);
    // go == false이면 스테이지 종료
    if (go == false) {
      t1.join(); t2.join(); t3.join(); t4.join(); t5.join(); //쓰레드 종료 -> 터미널 오류x
    }
  }
  if(gamerun==false && stageLevel<=4)//게임오버
    fancy_lighting(3);
  else if(gamerun==false && stageLevel>=5)//성공
    fancy_lighting(2);
  nodelay(stdscr, FALSE);
  getch();
  endwin();
  return 0;
}
