#include <iostream>
#include "Items.h"
using namespace std;

extern bool go;
extern bool passing;
// 아이템 개수 - 한번에 나올 수 있는 아이템의 수는 3개 이하
int itemCnt = 0;

// 아이템 생성 및 소멸 함수
void makeGrowItem(){
  random_device rd; // 시드값을 얻음 - srand보다 완벽한 무작위
  mt19937 gen(rd()); // random_device 난수 생성 엔진 초기화
  uniform_int_distribution<int> b(3,7); // 첫 아이템 출현 시간 (3초~7초) 랜덤
  sleep(b(gen));

  while(go){
    if(itemCnt < 3){
      random_device rd; // 시드값을 얻음 - srand보다 완벽한 무작위
      mt19937 gen(rd()); // random_device 난수 생성 엔진 초기화

      uniform_int_distribution<int> r(1, 19); // wall 아닌 부분에서 x좌표 선택
      uniform_int_distribution<int> c(1, 38); // wall 아닌 부분에서 y좌표 선택

      int x = r(gen);
      int y = c(gen);

      if (!map_array[x][y]){
        itemCnt++;
        map_array[x][y] = 5;
        uniform_int_distribution<int> p(11, 15); // 아이템 출현 유지 기간 (11초~15초) 랜덤
        sleep(p(gen));
        map_array[x][y] = 0;
        itemCnt--;
      }
      sleep(b(gen)); // 다음 아이템까지 쉬는 시간(3초 ~ 7초) 랜덤적으로
    }
    else continue;
  }
}

void makePoisonItem(){
  random_device rd; // 시드값을 얻음 - srand보다 완벽한 무작위
  mt19937 gen(rd()); // random_device 난수 생성 엔진 초기화
  uniform_int_distribution<int> b(3,7);
  sleep(b(gen));

  while(go){
    if(itemCnt < 3){

      uniform_int_distribution<int> r(1, 19); // wall 아닌 부분에서 x좌표 선택
      uniform_int_distribution<int> c(1, 38); // wall 아닌 부분에서 y좌표 선택

      int x = r(gen);
      int y = c(gen);

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
  random_device rd; // 시드값을 얻음 - srand보다 완벽한 무작위
  mt19937 gen(rd()); // random_device 난수 생성 엔진 초기화
  uniform_int_distribution<int> b(3,7);
  sleep(b(gen));

  while(go){
    uniform_int_distribution<int> r(0, 20); // x좌표 선택
    uniform_int_distribution<int> c(0, 40); // y좌표 선택

    int x1 = r(gen);
    int y1 = c(gen);
    int x2 = r(gen);
    int y2 = c(gen);
    while ((x1 == x2) && (y1 == y2)) {x2 = r(gen); y2 = c(gen);}

    int prev_g1;
    int prev_g2;

    if ((map_array[x1][y1] == 0 || map_array[x1][y1] == 1) && (map_array[x2][y2] == 0 || map_array[x2][y2] == 1)){
      prev_g1 = map_array[x1][y1];
      prev_g2 = map_array[x2][y2];
      map_array[x1][y1] = 7; map_array[x2][y2] = 7;
      g1.r = x1; g1.c = y1;
      g2.r = x2; g2.c = y2;
      sleep(7);
      while (passing){continue;}
      map_array[x1][y1] = prev_g1;
      map_array[x2][y2] = prev_g2;
    }
    sleep(b(gen));
  }
}
