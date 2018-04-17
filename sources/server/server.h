#pragma once
#include "../common/game_common.h"

#define SERVER_WINDOW_H 300
#define SERVER_WINDOW_W 300

typedef struct Server_Player{
  bool is_connected;
  int id;
  sockaddr_in addr;
  char handle_name[11];
  int chara_type;
  int next_action_command;
  int item;
  int lap;
  int rank_count;
  int rank;
  bool is_finished;
  double finish_time;
  double x,y;
  int falling_time; //>0で転倒
  
  double vx,vy;
  double ax,ay;
  double angle;  //静止時のために追加
  //固有値
  double m;     //衝突とかで使う
  double v_max; //絶対値
  double a_max; //絶対値
  double friction_a;     //減速率
  double handling_arg;//回転率
  double r;
}Server_Player;

//--変数--
extern int sock;
extern fd_set fd;
extern Server_Player player_list[PLAYER_NUM];
extern Object object_list[MAX_OBJECT_NUM];

extern char map_id;

//flags
extern int connected_num;
extern bool is_racing;

extern char *error_message;

extern int passed_time;
extern int ranking[PLAYER_NUM];
extern int finished_num;

extern int passed_time;

extern SDL_Window *window;
extern SDL_Renderer *renderer;

void Init(int port);
void Quit();
void Game_Init_Param();
void Wait_Client();
void Select_Cart();
void Game_Loop();
void Game_Ending();
//######################################################
void Collect_Command(int ms);
void Advance_Turn();
void Send_Game_Status();

void Update_Rank();
void Check_Finish();
void Encode_Player_Data(Server_SndData_Player *sndbuf,const Server_Player *player);

//###############################
void Window_Init();
void Window_Delete();
void Window_Clear(Uint8 r,Uint8 g,Uint8 b);
void Window_RenderPresent();


void Interrupt_Exit();
