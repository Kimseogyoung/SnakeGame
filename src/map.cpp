#include <ncurses.h>
#include <iostream>
#include<clocale>
using namespace std;


struct mission_result{
  int leng;
  int gitem;
  int pitem;
  int gate;
};


class mission{
private:
  int level=0;
  int leng=0;
  int gitem=0;
  int pitem=0;
  int gate=0;
  bool lock;

public:
  mission(int lv){
    level=lv;
  }
  int getLeng(){return leng;}
  int getgItem(){return gitem;}
  int getpItem(){return pitem;}
  int getGate(){return gate;}
  bool getLock(){return lock;}
  void set_mission(){
    switch (level) {
      case 1:
        leng=5;
        gitem=5;
        pitem=1;
        gate=1;
        lock=false;
        break;
      case 2:
        leng=7;
        gitem=7;
        pitem=3;
        gate=3;
        lock=false;
        break;
     case 3:
       leng=15;
       gitem=10;
       pitem=2;
       gate=5;
       lock=true;
       break;
    case 4:
      leng=15;
      gitem=10;
      pitem=0;
      gate=10;
      lock=true;
      break;
    }
  }
  void isMissoncomplete(mission_result &p,int l,int gi, int pi, int gt){

    if(leng<=l)
      p.leng=1;
    if(gitem<=gi)
      p.gitem=1;
    if(lock==false && pitem<=pi)
      p.pitem=1;
    else if(lock==true && pitem>=pi)
      p.pitem=1;
    if(gate<=gt)
      p.gate=1;

  }
};


int map_array[21][40] = {{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,1,1,1,1,1,1,1,1,2},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2}};

// map 변경 -> game window만 얘가 다룸. 메인 윈도우는 메인함수에서 따로 정의
void map(){
  WINDOW *gamewin = newwin(21, 40, 3, 3); //int nlines, int ncols, int begy, int begx
  wbkgd(gamewin, COLOR_PAIR(1));

  // 배열의 크기 = 윈도우의 크기
  for (int r = 0; r < 21; r++){
    for (int c = 0; c < 40; c++){
      if (map_array[r][c] == 2) {mvwaddch(gamewin,r,c,'+');} // immune wall (y,x)
      else if (map_array[r][c] == 1 && (r == 0 || r == 20)) {mvwaddch(gamewin,r,c,'-');} // wall
      else if (map_array[r][c] == 1) {mvwprintw(gamewin,r,c,"\u2B1C");} // wall
      else if (map_array[r][c] == 3) {mvwaddch(gamewin,r,c,'@');} // snake head
      else if (map_array[r][c] == 4) {mvwaddch(gamewin,r,c,'#');} // snake body
      else if (map_array[r][c] == 5) {mvwaddch(gamewin,r,c,'G');} // Grow item
      else if (map_array[r][c] == 6) {mvwaddch(gamewin,r,c,'P');} // Poison item
      else if (map_array[r][c] == 7) {mvwaddch(gamewin,r,c,'X');}
      else {mvwaddch(gamewin,r,c,' ');} // space
    }
  }
  wrefresh(gamewin); // game window 변경한 부분 적용
  // refresh();
  // return gamewin;
}
