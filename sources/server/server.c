#include "../common/game_common.h"
#include "server.h"
#include "server_net.h"
#include"server_item.h"

#include<stdlib.h>
#include<time.h>

//変数実体宣言
int sock;
fd_set fd;
Server_Player player_list[PLAYER_NUM];//使われたアイテムを、その効果が持続する期間保持する。
Object object_list[MAX_OBJECT_NUM];
//flags
int connected_num;
bool is_racing;
char *error_message;
int passed_time;  //1ms単位
int ranking[PLAYER_NUM];
int finished_num;
char map_id;

SDL_Window *window;
SDL_Renderer *renderer;


void Interrupt_Exit(){
  Server_SndData_Command sndbuf;
  sndbuf.data_type=ERROR_EXIT;
  Server_Send_to_All_Clients(&sndbuf, sizeof(sndbuf));
  exit(EXIT_FAILURE);
}

void Init(int port){
  Server_Net_Init(port);
  srand(time(NULL));
  memset(player_list,0,sizeof(player_list));
  memset(object_list,0,sizeof(object_list));
  
  connected_num=0;
  is_racing=false;
  passed_time=-5000;

  for(int i=0;i<PLAYER_NUM;i++)ranking[i]=i;
  finished_num=0;
  
  Window_Init();

  map_id=rand()%SELECTABLE_MAP_NUM;
  char map_src[30];
  sprintf(map_src,"../resources/map/map%d.csv",map_id);
  Init_Map(renderer,map_src, WINDOW_H, WINDOW_W,DEFAULT_MAP_H_SCALE,DEFAULT_MAP_W_SCALE);
}

void Quit(){
  Window_Delete();
  Server_Net_Quit();
  Delete_Map();
}

void Game_Init_Param(){
  //プレーヤーのパラメーター初期化
  const Map_SubData * mapsub = Get_Map_Sub_Data();
  if(mapsub->start_point_num<PLAYER_NUM){
    fprintf(stderr,"Map data start point num is smaller than player num.");
    exit(1);
  }
  for(int i=0;i<PLAYER_NUM;i++){
    Server_Player *p = &player_list[i];
    p->x = mapsub->start_point_x[i] *DEFAULT_MAP_W_SCALE + DEFAULT_MAP_W_SCALE/2;
    p->y = mapsub->start_point_y[i] *DEFAULT_MAP_H_SCALE + DEFAULT_MAP_H_SCALE/2;
    p->vx = p->vy = 0;
    p->angle=-90;
    p->ax =1 ;//ここ調整必要
    p->ay =1 ;
    
    p->v_max=PLAYER_V_MAX[i];
    p->a_max=PLAYER_A_MAX[i];
    p->friction_a=PLAYER_FRICTION_A[i];
    p->handling_arg=PLAYER_HANDLING_ARG[i];
    p->r=PLAYER_R[i];
    p->m=PLAYER_M[i];
  }

  //アイテムボックスの生成
  for(int i=0;i<mapsub->item_box_num;i++){
    Object_Item_Box* obj = (void*)&object_list[i];
    obj->code = ITEM_BOX;
    obj->is_enable=true;
    obj->x = mapsub->item_box_x[i]*DEFAULT_MAP_W_SCALE + DEFAULT_MAP_W_SCALE/2;
    obj->y = mapsub->item_box_y[i]*DEFAULT_MAP_H_SCALE + DEFAULT_MAP_H_SCALE/2;
    obj->disable_time = 0;
  }
  
}

void Game_Ending(){
  Server_SndData_Result sndbuf;
  int i;
  sndbuf.data_type=RESULT_DATA;
  for(i=0;i<PLAYER_NUM;i++){
    sndbuf.ranking[i]=ranking[i];
    sndbuf.finish_time[i]=player_list[ranking[i]].finish_time;
  }
  Server_Send_to_All_Clients(&sndbuf, sizeof(Server_SndData_Result));
}

int main(){
  printf("%lu %lu\n",sizeof(Server_SndData_t),sizeof(Server_SndData_GameStart));
  
  Init(SERVER_PORT);

  Wait_Client();

  Select_Cart();

  Game_Loop();

  Game_Ending();
  
  Quit();
  
}
