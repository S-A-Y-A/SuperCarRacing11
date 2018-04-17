#include"client.h"

bool AI_set_ip(const char *ip){
  if(check_ip_v4(ip) && Get_Addr(ip, SERVER_PORT, &server_addr))
    return true;
  else return false;
}

void AI_connect(const char *handlename){
  Client_SndData_Connect sndbuf;
  sndbuf.data_type=CONNECT_ORDER;
  strcpy(sndbuf.handle_name, handlename);
  Send_to_Server(&sndbuf, sizeof(sndbuf));
  
  Server_SndData_t rcvbuf;
  SDL_Event ev;
  int param=0;
  bool ret_flg=false;
  Uint32 time_out =SDL_GetTicks() +4000;
  while(!is_connected && !is_error){
    if( !Client_Receive(&rcvbuf, 100)){
      //受信していないのでとりあえず送り続ける
      Send_to_Server(&sndbuf, sizeof(sndbuf));

      while(SDL_PollEvent(&ev)){
        if(ev.type==SDL_KEYDOWN && ev.key.keysym.scancode==SDL_SCANCODE_Q){
          Error_exit();
        }
        if(ev.type==SDL_QUIT){
          Error_exit();
        }
      }
      if(SDL_TICKS_PASSED(SDL_GetTicks(), time_out)){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "接続がタイムアウトしました",
                                 "サーバーが立ち上がっていないようです\n有効なPCのIPを入力してください",window);
        SDL_RaiseWindow(window);
        break;
      }
    }
    else{
      //受信
      if(rcvbuf.data_type==CONNECT_ACCEPT){
        Server_SndData_Command *rcv_command=(Server_SndData_Command*)&rcvbuf;
        is_connected=true;
        id=rcv_command->data;
        ret_flg=true;
        break;
      }
    }
  }
}

int AI_Think_Command(Client_Player *p){
  int px =p->x,py=p->y,angle=p->angle;

  double best_angle=angle,best_dest2=0;
  //各方向を見て一番進めるところへ行こうとする
  for(double nxt_angle=angle-180;nxt_angle<angle+180;nxt_angle+=20){
    double x=px,y=py,nx=px,ny=py;
    while(Is_In_Map(ny, nx) && Get_Progress_Param(y, x, ny, nx)>=0){
      x=nx;y=ny;
      nx+=cos(nxt_angle*M_PI/180)*DEFAULT_MAP_W_SCALE/2;
      ny+=sin(nxt_angle*M_PI/180)*DEFAULT_MAP_H_SCALE/2;
    }

    double dist2=(x-px)*(x-px)+(y-py)*(y-py);
    if(best_dest2<dist2){
      best_dest2=dist2;
      best_angle=nxt_angle;
    }
  }
  int ret=0;
  if(p->item!=ITEM_NOTHING)ret|=USE_ITEM;
  ret|=MOVE_UP;
  ret|= (best_angle==angle ? 0 : (best_angle<angle ? MOVE_LEFT : MOVE_RIGHT));
  return ret;
}
