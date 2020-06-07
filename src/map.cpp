#include <ncurses.h>
#include <iostream>
#include <locale.h>
using namespace std;

int map_array[21][40] = {{2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
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
                     {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                     {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                     {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                     {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                     {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                     {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2}};



// map 변경 -> game window만 얘가 다룸. 메인 윈도우는 메인함수에서 따로 정의
void map(){
  WINDOW *gamewin = newwin(21, 40, 3, 3); //int nlines, int ncols, int begy, int begx
  wbkgd(gamewin, COLOR_PAIR(1));
  // 배열의 크기 = 윈도우의 크기, 게임윈도우 출력 내용
  for (int r = 0; r < 21; r++){
    for (int c = 0; c < 40; c++){
      // 유니코드 사용 전 코드
      // if (map_array[r][c] == 0) {mvwaddch(gamewin,r,c,'+');} // immune wall (y,x)
      // else if (map_array[r][c] == 1 && (r == 0 || r == 20)) {mvwaddch(gamewin,r,c,'-');} // wall
      // else if (map_array[r][c] == 1) {mvwaddch(gamewin,r,c,'|');} // wall
      // else if (map_array[r][c] == 3) {mvwaddch(gamewin,r,c,'@');} // snake head
      // else if (map_array[r][c] == 4) {mvwaddch(gamewin,r,c,'#');} // snake body
      // else if (map_array[r][c] == 5) {mvwaddch(gamewin,r,c,'G');} // Grow item
      // else if (map_array[r][c] == 6) {mvwaddch(gamewin,r,c,'P');} // Poison item
      // else if (map_array[r][c] == 7) {mvwaddch(gamewin,r,c,'X');}
      // else {mvwaddch(gamewin,r,c,' ');} // space
      // if (map_array[r][c] == 2) {mvwprintw(gamewin,r,c,"\u2B1B");} // immune wall (y,x)
      // else if (map_array[r][c] == 0 && map_array[r][c] == 1) {mvwprintw(gamewin,r,c,"\u2B1B");} // wall
      // else if (map_array[r][c] == 1) {mvwprintw(gamewin,r,c,"\u2B1B");} // wall

      // 유니코드 사용 시 종류 별 색과 출력 내용 설정
      // if (map_array[r][c] == 0) {
      //   wattron(gamewin, COLOR_PAIR(1));
      //   mvwprintw(gamewin,r,c,"\u2B1B");
      // } // Space
      // else if (map_array[r][c] == 1) {
      //   wattron(gamewin, COLOR_PAIR(2));
      //   mvwprintw(gamewin,r,c,"\u2B1B");
      // } // Wall
      // else if (map_array[r][c] == 2) {
      //   wattron(gamewin, COLOR_PAIR(3));
      //   mvwprintw(gamewin,r,c,"\u2B1B");
      // } // Immune wall
      // else if (map_array[r][c] == 3) {
      //   wattron(gamewin, COLOR_PAIR(4));
      //   mvwprintw(gamewin,r,c,"\u2B1B");
      // } // Snake head
      // else if (map_array[r][c] == 4) {
      //   wattron(gamewin, COLOR_PAIR(5));
      //   mvwprintw(gamewin,r,c,"\u2B1B");
      // } // Snake body
      // else if (map_array[r][c] == 5) {
      //   wattron(gamewin, COLOR_PAIR(6));
      //   mvwprintw(gamewin,r,c,"\u2B1B");
      // } // Grow item
      // else if (map_array[r][c] == 6) {
      //   wattron(gamewin, COLOR_PAIR(7));
      //   mvwprintw(gamewin,r,c,"\u2B1B");
      // } // Poison item
      // else if (map_array[r][c] == 7) {
      //   wattron(gamewin, COLOR_PAIR(8));
      //   mvwprintw(gamewin,r,c,"\u2B1B");
      // } // Gate
      // // else {mvwprintw(gamewin,r,c," ");}

      // 색만 다르고 모두 같은 내용 출력
      wattron(gamewin, COLOR_PAIR(map_array[r][c] + 1));
      mvwprintw(gamewin,r,c,"\u2B1B");

    }
  }
  wrefresh(gamewin); // game window 변경한 부분 적용
  // refresh();
  // return gamewin;
}
