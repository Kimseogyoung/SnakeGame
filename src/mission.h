#include <ncurses.h>
#include <iostream>
#include <clocale>
#ifndef MISSION_H
#define MISSION_H

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
  mission(int lv);
  int getLeng(){return leng;}
  int getgItem(){return gitem;}
  int getpItem(){return pitem;}
  int getGate(){return gate;}
  bool getLock(){return lock;}
  void set_mission();
  void isMissoncomplete(mission_result &p,int l,int gi, int pi, int gt);
};
#endif
