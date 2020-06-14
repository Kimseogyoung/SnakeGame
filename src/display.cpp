#include <iostream>
#include "display.h"

void fancy_lighting(){
  int wow[6][50]={{1,1,1,1,1,1,1,2,1,1,1,2,},
                  {},
                  {},
                  {},
                  {}};
}

void nextStageEffect(int num){
  clear();
  bkgd(COLOR_PAIR(6));
  mvprintw(5,10,"Stage < %d >",num);
  for(int i=5; i>0;i--){
    mvprintw(6,10,"Start in %d seconds",i);
    refresh();
    sleep(1);
  }
}

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
