#include <iostream>
#include "map.h"
using namespace std;

int map_array[21][40];
WINDOW *gamewin;

// 게임 윈도우 출력 
void map(){
  for (int r = 0; r < 21; r++){
    for (int c = 0; c < 40; c++){
      // 색만 다르고 모두 같은 문자 출력
      wattron(gamewin, COLOR_PAIR(map_array[r][c] + 1));
      mvwprintw(gamewin,r,c,"\u2B1B");
    }
  }
  wrefresh(gamewin);
}
