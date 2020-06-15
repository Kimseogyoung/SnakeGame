#include <iostream>
#include "Items.h"
using namespace std;

extern bool go;
// 아이템 개수 - 한번에 나올 수 있는 아이템의 수는 3개 이하
int itemCnt = 0;

void makeGrowItem(){
  random_device rd;   // 시드값을 얻음 - srand보다 완벽한 무작위
  mt19937 gen(rd());   // random_device 난수 생성 엔진 초기화
  uniform_int_distribution<int> b(3,7);   // 첫 아이템 출현 시간 (3초~7초) 랜덤
  sleep(b(gen));

  while(go){
    if(itemCnt < 3){
      random_device rd;
      mt19937 gen(rd());

      // wall 아닌 부분에서 좌표 선택
      uniform_int_distribution<int> r(1, 19);
      uniform_int_distribution<int> c(1, 38);

      int x = r(gen);
      int y = c(gen);

      // 아무것도 없는 공간일 경우에만 아이템 출현
      if (!map_array[x][y]){
        itemCnt++;
        map_array[x][y] = 5;
        uniform_int_distribution<int> p(11, 15);   // 아이템 출현 유지 기간 (11초~15초) 랜덤
        sleep(p(gen));
        map_array[x][y] = 0;
        itemCnt--;
      }
      sleep(b(gen));   // 다음 아이템까지 쉬는 시간(3초 ~ 7초) 랜덤
    }
    else continue;
  }
}

void makePoisonItem(){
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> b(3,7);   // 첫 아이템 출현 시간 (3초~7초) 랜덤
  sleep(b(gen));

  while(go){
    if(itemCnt < 3){

      uniform_int_distribution<int> r(1, 19);
      uniform_int_distribution<int> c(1, 38);

      int x = r(gen);
      int y = c(gen);

      // 아무것도 없는 공간일 경우에만 아이템 출현
      if (!map_array[x][y]){
        itemCnt++;
        map_array[x][y] = 6;
        uniform_int_distribution<int> p(11, 15); // 아이템 출현 유지 기간 (11초~15초) 랜덤
        sleep(p(gen));
        map_array[x][y] = 0;
        itemCnt--;
      }
      sleep(b(gen));
    }
    else continue;
  }
}

void makeGate(){
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> b(3,7);    // 첫 아이템 출현 시간 (3초~7초) 랜덤
  sleep(b(gen));

  while(go){
    uniform_int_distribution<int> r(0, 20);
    uniform_int_distribution<int> c(0, 40);

    int x1 = r(gen);
    int y1 = c(gen);
    int x2 = r(gen);
    int y2 = c(gen);
    while ((x1 == x2) && (y1 == y2)) {x2 = r(gen); y2 = c(gen);}   // g1과 g2의 좌표가 같지 않도록

    int prev_g1;
    int prev_g2;

    // 벽 또는 아무것도 없는 공간일 경우에만 gate 출현
    if ((map_array[x1][y1] == 0 || map_array[x1][y1] == 1) && (map_array[x2][y2] == 0 || map_array[x2][y2] == 1)){
      prev_g1 = map_array[x1][y1];
      prev_g2 = map_array[x2][y2];
      map_array[x1][y1] = 7; map_array[x2][y2] = 7;
      g1.r = x1; g1.c = y1;
      g2.r = x2; g2.c = y2;
      sleep(7);
      map_array[x1][y1] = prev_g1;
      map_array[x2][y2] = prev_g2;
    }
    sleep(b(gen));
  }
}
