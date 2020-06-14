#include <iostream>
#include "mission.h"
using namespace std;

mission::mission(int lv){level=lv;}
void mission::set_mission(){
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
void mission::isMissoncomplete(mission_result &p,int l,int gi, int pi, int gt){

  if(leng<=l)p.leng=1;
  else p.leng=0;

  if(gitem<=gi) p.gitem=1;
  else p.gitem=0;

  if(lock==false && pitem<=pi)
    p.pitem=1;
  else if(lock==true && pitem>=pi)
    p.pitem=1;
  else p.pitem=0;

  if(gate<=gt) p.gate=1;
  else p.gate=0;
}
