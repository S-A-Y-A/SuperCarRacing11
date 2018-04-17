#pragma once

#include<stdbool.h>
#include "../common/game_common.h"
#include"client_net.h"
#include"client_window.h"

typedef struct Client_Player{
  int id;
  char handle_name[11];
  int cart_type;
  int item;
  bool is_finished;
  int rank;
  int lap;
  int x,y;
  int falling_time;
  double vx,vy;//画面表示自動更新用に
  double angle;  //静止時のために追加
}Client_Player;


//---変数定義---
extern int sock;
extern fd_set fd;
extern int id;           //自分のキャラidentifer
extern sockaddr_in server_addr;
extern char map_id;       //使うマップのマップ番号

//flags
extern bool is_connected ; //サーバーに接続されているか否か
extern bool is_racing    ; //ゲームループ（レース部分）が行われているかどうか
extern bool is_finished  ; //自分がゴールしたかどうか
extern bool is_error     ; //ゲームが正常に進んでいるかどうか

extern bool ai_mode ;

extern int passed_time;
extern char *error_message;
//list
extern Client_Player player_list[PLAYER_NUM];
extern Object object_list[MAX_OBJECT_NUM];



//構造体の初期化、ポートオープンなど
void Init();
//その逆
void Quit();
void Game_Opening();
void Input_HandleName(char *handlename);
void Input_ServerIP(sockaddr_in *addr);
//サーバーに接続する関数　つないで応答を待つ
bool Connect_to_Server(const char *handlename);
//人数が集まるまで待つ
void Wait_Other_Clients();
//キャラの選択などを行う
void Cart_Select();
//ゴール後から終わりまでの処理
void Game_Ending();
void EndRoll();
void check_error();
//######################################################
void Error_exit();

#include"client_gameloop.h"
