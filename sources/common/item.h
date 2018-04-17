#pragma once
#include"definitions.h"
/*
  アイテム・オブジェクトの構造体
  アイテムなどはObject型の配列として管理する
  ネットワーク通信のデータみたいな感じでcodeによりキャストして使う
 */


//--共通構造体--------//
typedef struct Object{
  char code;
  char is_enable;
  char data[46];
}Object;
typedef Object Item;

//--個別構造体--------//

typedef struct Object_Item_Box{
  char code;
  char is_enable;
  double x,y;
  int disable_time;
}Object_Item_Box;

typedef struct Item_Squid_Ink{
  char code;
  char is_enable;
  char user;
  int left_time;
}Item_Squid_Ink;

typedef struct Item_Thunder{
  char code;
  char is_enable;
  char user;
  int left_time;
}Item_Thunder;

typedef struct Item_Green_Shell{
  char code;
  char is_enable;
  char user;
  double x,y;
  double vx,vy;
  int left_time;//そのうち自動消滅する仕様に。
}Item_Green_Shell;

typedef struct Item_Red_Shell{
  char code;
  char is_enable;
  char user;
  char target;
  double x,y;
  double vx,vy;
  int left_time;//そのうち自動消滅する仕様に。
}Item_Red_Shell;

typedef struct Item_Blue_Shell{
  char code;
  char is_enable;
  char user;
  char target;
  double x,y;
  double vx,vy;
  int left_time;//そのうち自動消滅する仕様に。
}Item_Blue_Shell;

typedef struct Item_MushRoom{
  char code;
  char is_enable;
  char target;
  int left_time;
}Item_MushRoom;
