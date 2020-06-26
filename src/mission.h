#include <ncurses.h>
#include <iostream>
#include <locale.h>
#ifndef MISSION_H
#define MISSION_H

/**
* 파일 전체
* @author 김서경(100%)
*/

struct mission_result{
  int leng;
  int gitem;
  int pitem;
  int gate;
  int runtime;
};

class mission{
private:
  int level=0;
  int leng=0;
  int gitem=0;
  int pitem=0;
  int gate=0;
  int runtime=0;
  bool lock;

public:
  mission(int lv);
  int getLeng(){return leng;}
  int getgItem(){return gitem;}
  int getpItem(){return pitem;}
  int getGate(){return gate;}
  int getTime(){return runtime;}
  bool getLock(){return lock;}
  void set_mission();
  void isMissoncomplete(mission_result &p,int l,int gi, int pi, int gt, int rt);
};
#endif
