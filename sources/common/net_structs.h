#pragma once
#include<stdbool.h>
#include"definitions.h"
#include"item.h"

//*ネットワーク***************************//

//--基本構造体----------//

typedef struct Server_SndData_t{
  char data_type;
  char data[60];
}Server_SndData_t;

typedef struct Client_SndData_t{
  char data_type;
  char client_id;
  char data[15];
}Client_SndData_t;

//--拡張構造体----------//
//サーバ
typedef struct Server_SndData_Command{
  char data_type;
  int data;     //ちょっと汎用的に使うために追加..
}Server_SndData_Command;

typedef struct Server_SndData_Player{
  char data_type;
  char id;
  char cart_type;
  char item;
  bool is_finished;
  char lap;
  char rank;
  int falling_time; //>0で転倒
  int x,y;
  double vx,vy;//画面表示自動更新用に
  double angle;  //静止時のために追加
}Server_SndData_Player;

typedef struct Server_SndData_Object{
  char data_type;
  unsigned char id;
  Object object;
}Server_SndData_Object;

typedef struct Server_SndData_Result{
  char data_type;
  char ranking[PLAYER_NUM];  // 1位から順位に id
  int finish_time[PLAYER_NUM]; // 1位から順に..
}Server_SndData_Result;

typedef struct Server_SndData_GameStart{
  char data_type;
  char handle_names[PLAYER_NUM][11];
  char map_id;
  char player_num;
}Server_SndData_GameStart;


//クライアント
typedef struct Client_SndData_Command{
  char data_type;
  char client_id;//送信関数によって送信時に自動的に付加するので設定不要!
  int data;     //ちょっと汎用的に使うために追加..
}Client_SndData_Command;

typedef struct Client_SndData_Connect{
  char data_type;
  char client_id;//送信関数によって送信時に自動的に付加するので設定不要!
  char handle_name[11];//10文字+null
}Client_SndData_Connect;
