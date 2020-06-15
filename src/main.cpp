#include <iostream>
#include "mission.h"
#include "display.h"
#include "Snake.h"
#include "Items.h"
#include "map.h"
#include "position.h"
using namespace std;

// 전역 변수
int growItems = 0;
int poisonItems = 0;
int gates = 0;
int runtime = 0;
int snSize = 0;

bool go = true;   // 스테이지별 흐름 제어를 위한 변수
bool gamerun = true;   // 전체 게임 제어를 위한 변수

int stageLevel = 1;
int nowMap = 0;

int maxR = 21;
int maxC = 40;

position g1;
position g2;

extern int mapList[8][21][40];

// 서브윈도우
WINDOW *state_board;
WINDOW *mission_board;


/**
* @brief 게임 진행 시간 기록
* @author 이소영
*/
void checkTime(){
  chrono::system_clock::time_point start = chrono::system_clock::now();
  while(go){
    chrono::duration<double> rt = chrono::system_clock::now() - start;
    runtime = (int)rt.count();
  }
}

/**
* @brief 게임 진행 시 Snake의 움직임 제어 및 미션 보드와 스코어 보드 내용 업데이트 및 출력
* @param Snake& s
* @author 김서경 %, 이소영 %, 이아연 %
*/
void run(Snake& s){
  int key;
  int dir = s.dir;

  mission m(stageLevel);   //미션 초기화
  m.set_mission();
  mission_result ms={0,0,0,0};

  nodelay(stdscr, TRUE);
  while (go){
    dir = s.dir;
    chrono::duration<double> sp = chrono::system_clock::now()-s.mvSpan;
    // 0.5초마다 키 입력 받아 이동
    while (sp.count() < 0.5){
      key = getch();
      if (key == KEY_UP) dir = 0;
      else if (key == KEY_RIGHT) dir = 1;
      else if (key == KEY_DOWN) dir = 2;
      else if (key == KEY_LEFT) dir = 3;
      sp = chrono::system_clock::now()-s.mvSpan;
    }
    s.move(dir);

    // 이동에 따른 효과 발생
    if (s.isBody()) { gamerun=false; go = false; }
    if (s.isWall()) { gamerun=false; go = false;}
    s.isGrowthItem();
    s.isPoisonItem();
    s.isGate();
    if (s.getSize() < 3){ gamerun=false; go=false;}
    if(gamerun==false){mvprintw(20,54,"Your snake is dead. Loading fail screen...",stageLevel);}

    m.isMissoncomplete(ms,s.getSize(), growItems,poisonItems,gates,runtime);   // 미션 성공 여부 체크
    if(ms.leng==1 && ms.gitem==1 && ms.pitem==1 && ms.gate ==1 && ms.runtime==1){   //미션 모두 완료 시
      go=false;
      mvprintw(20,54,"stage %d clear. Loading....",stageLevel);
      stageLevel++;
    }

    // 화면 출력
    s.printsnake();
    snSize = s.getSize();
    State_board();
    Mission_board(m,ms);

    s.mvSpan = chrono::system_clock::now();
    refresh();
  }
}

/**
* @brief 게임 전체 제어
* @author 김서경 %, 이소영 %, 이아연 %
*/
int main(){
  // 셋업
  setlocale(LC_ALL, "");
  initscr();
  resize_term(400, 600);
  start_color();
  bkgd(COLOR_PAIR(1));
  init_color(COLOR_WHITE, 1000, 1000, 1000);

  init_pair(1, COLOR_WHITE, COLOR_WHITE);
  init_pair(2, COLOR_BLACK, COLOR_BLACK);   // 글씨색, 배경색 > 기본 벽
  init_pair(3, COLOR_BLACK, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_WHITE);   // Snake Head
  init_pair(5, COLOR_CYAN, COLOR_WHITE);   // Snake Body
  init_pair(6, COLOR_GREEN, COLOR_WHITE);   // grow Items
  init_pair(7, COLOR_RED, COLOR_WHITE);   // posion Items
  init_pair(8, COLOR_MAGENTA, COLOR_WHITE);   // gate
  init_pair(9, COLOR_BLACK, COLOR_RED); // 끝났을때 배경

  // 서브 윈도우 크기 및 위치 설정
  state_board = newwin(8, 40, 3, 53);
  mission_board = newwin(8, 40, 10, 53);
  gamewin = newwin(21, 40, 3, 3);
  wbkgd(gamewin, COLOR_PAIR(1));

  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  // 프로그램 시작 화면 출력
  fancy_lighting(1);
  int mapdata[8]={0};
  stageLevel=1;

  // 게임시작
  while(gamerun && stageLevel<=4){
    go=true;
    nextStageEffect(stageLevel);
    // 맵 랜덤 결정
    while(true){
      random_device rd;
      mt19937 gen(rd());
      uniform_int_distribution<int> mapnum(0, 7);
      nowMap=mapnum(gen);
      if(mapdata[nowMap]==0) {mapdata[nowMap]=1; break;}
    }
    // 맵 초기화
    for(int i=0; i<maxR;i++)
      for(int j=0; j<maxC;j++)
        map_array[i][j]=mapList[nowMap][i][j];
    // 현재 상태 초기화 및 게임 진행
    growItems=0;
    poisonItems=0;
    gates=0;
    snSize=0;
    runtime=0;
    Snake snake = Snake();
    g1.r = 0; g1.c = 0;
    g2.r = 0; g2.c = 0;
    thread t1(makeGrowItem);
    thread t2(makePoisonItem);
    thread t3(makeGrowItem);
    thread t4(makePoisonItem);
    thread t5(makeGate);
    thread t6(checkTime);
    run(snake);

    // 스테이지 종료
    if (go == false) t1.join(); t2.join(); t3.join(); t4.join(); t5.join(); t6.join();
  }
  if(gamerun==false && stageLevel<=4)   // 게임 오버
    fancy_lighting(3);
  else if(stageLevel>=5)   // 성공
    fancy_lighting(2);

  nodelay(stdscr, FALSE);
  getch();
  endwin();
  return 0;
}
