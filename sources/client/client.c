#include "client.h"

//変数実体宣言
int sock;
fd_set fd;
int id;           //自分のキャラidentifer
char map_id;
sockaddr_in server_addr;
char handle_name[11];
//flags
bool is_connected ; //サーバーに接続されているか否か
bool is_racing    ; //ゲームループ（レース部分）が行われているかどうか
bool is_finished  ; //自分がゴールしたかどうか
bool is_error     ; //ゲームが正常に進んでいるかどうか
int passed_time;
char *error_message;
Client_Player player_list[PLAYER_NUM];
Object object_list[MAX_OBJECT_NUM];

bool is_inited=false;
bool ai_mode=false;

//簡易終了処理付きexit
void Error_exit(){
  if(is_inited){
    Client_SndData_Command sndbuf;
    sndbuf.data_type=ERROR_EXIT;
    Send_to_Server(&sndbuf, sizeof(sndbuf));
  }
  exit(EXIT_FAILURE);
}

void check_error(){
  if(is_error){
    fputs(error_message,stderr);
    fputc('\n', stderr);
    //終了処理する?
    exit(EXIT_FAILURE);
  }
}

void Init(){
  Client_Net_Init();
  id=-1;
  is_connected=false;
  is_racing=false;
  is_finished=false;
  is_error=false;
  passed_time=-4;
  memset(player_list,0,sizeof(player_list));
  memset(object_list,0,sizeof(object_list));

  Window_Init();
  is_inited=true;
}

void Quit(){
  Client_Net_Quit();
  Window_Delete();
}


int main(){

  Init();
  
  Game_Opening();

  char handlename[11];

  Input_HandleName(handlename);
  do{
    Input_ServerIP(&server_addr);
  }while(Connect_to_Server(handlename)==false);

  
  Wait_Other_Clients();

  Cart_Select();

  char map_src[30];
  sprintf(map_src,"../resources/map/map%d.csv",map_id);
  Init_Map(renderer,map_src,WINDOW_H, WINDOW_W, DEFAULT_MAP_H_SCALE,  DEFAULT_MAP_W_SCALE);
  
  Game_Loop();

  Game_Ending();

  EndRoll();

  Quit();
}
