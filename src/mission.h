#include <ncurses.h>
#include <iostream>
#include <clocale>
#ifndef MISSION_H
#define MISSION_H

/**
* @brief
* @author
*/
struct mission_result{
  int leng;
  int gitem;
  int pitem;
  int gate;
  int runtime;
};

/**
* @brief
* @author
*/
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
  /**
  * @brief
  * @author
  */
  mission(int lv);

  /**
  * @brief
  * @author
  */
  int getLeng(){return leng;}

  /**
  * @brief
  * @author
  */
  int getgItem(){return gitem;}

  /**
  * @brief
  * @author
  */
  int getpItem(){return pitem;}

  /**
  * @brief
  * @author
  */
  int getGate(){return gate;}

  /**
  * @brief
  * @author
  */
  int getTime(){return runtime;}

  /**
  * @brief
  * @author
  */
  bool getLock(){return lock;}

  /**
  * @brief
  * @author
  */
  void set_mission();

  /**
  * @brief
  * @author
  */
  void isMissoncomplete(mission_result &p,int l,int gi, int pi, int gt, int rt);
};
#endif
