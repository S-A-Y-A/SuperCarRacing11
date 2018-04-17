#pragma once
#include <SDL2/SDL.h>

#define WINDOW_H 480
#define WINDOW_W 640

#define SERVER_PORT 50000

//共通？のパラメータ
#define PLAYER_NUM 4
#define MAX_OBJECT_NUM 150
#define SELECTABLE_CHARA_NUM 5
#define SELECTABLE_MAP_NUM 3
extern int  GOAL_LAP;
#define SERVER_LOOP_MS 50

//カート画像のサイズ
#define CART_W 120
#define CART_H 120
#define CART_ANGLE 90  //カート画像の向き調整用

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef SDL_Point myPoint;
typedef SDL_Rect myRectange;

enum DATA_TYPE{
  NODATA,PLAYER_DATA,OBJECT_DATA,
  //サーバーのコマンド
  CONNECT_ACCEPT,SELECT_CART_ORDER,
  GAME_START,
  TIME_COUNT,
  GAME_END,
  RESULT_DATA,
  //クライアントのコマンド
  ACTION_COMMAND,
  CONNECT_ORDER,SELECT_CART_RESPONSE,
  //共通のコマンド
  ERROR_EXIT
};

//仕様変更 : 複数のコマンドの同時入力に対応のため
#define NOMOVE     0
#define MOVE_UP    (1<<0)
#define MOVE_DOWN  (1<<1)
#define MOVE_LEFT  (1<<2)
#define MOVE_RIGHT (1<<3)
#define USE_ITEM   (1<<4)


enum ITEM_AND_OBJECT{
  //アイテム
  ITEM_NOTHING=0,SQUID_INK,THUNDER,GREEN_SHELL,RED_SHELL,BLUE_SHELL,MUSHROOM,
  //オブジェクト
  ITEM_BOX
};
//アイテムのパラメータ
#define ITEM_BOX_R 5
#define ITEM_BOX_DISABLETIME 2000 

#define GREEN_SHELL_R 5
#define GREEN_SHELL_FALLTIME 3000

#define RED_SHELL_R 5
#define RED_SHELL_FALLTIME 3000

#define BLUE_SHELL_R 20
#define BLUE_SHELL_FALLTIME 4000

extern double BLUE_SHELL_SPEED;
extern double RED_SHELL_SPEED; 
extern double GREEN_SHELL_SPEED; 

#define SQUID_INK_TIME 5000
#define THUNDER_TIME 5000
#define GREENSHELL_TIME 40000
#define REDSHELL_TIME 40000
#define BLUESHELL_TIME 40000
#define MUSHROOM_TIME 3000

#define THUNDER_DECELERATION 0.5  //Vx,y=V_MAX/この値となる
#define MUSHROOM_ACCELERATION 2  //Vx,y=V_MAX*この値となる

#define DT 20.0

extern double PLAYER_M[];
extern double PLAYER_V_MAX[];
extern double PLAYER_A_MAX[];
extern double PLAYER_FRICTION_A[];
extern double PLAYER_HANDLING_ARG[];
extern double PLAYER_R[];


