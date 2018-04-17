#include"client.h"
#include"ai.h"


void Game_Loop(){
  SDL_Event ev;
  while(is_racing){
    while(SDL_PollEvent(&ev)){
      if(ev.type==SDL_QUIT)Error_exit();
    }
    //60fps = 約17msで動かす
    Uint32 timeout =SDL_GetTicks()+17;
    
    Receive_Game_Status();
  
    Draw_Window_Racing();
    
    Input_And_Send();

    //60fps実現のためWait
    //printf("Debug Time : %d\n",SDL_GetTicks()-timeout+17);
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), timeout))SDL_Delay(1);
  }

}

//受信したプレーヤーデータをクライアントが使う型に変換（格納）する
void Decode_Player_Data(Client_Player *player,const Server_SndData_Player *rcvbuf){
  player->id=rcvbuf->id;
  player->cart_type=rcvbuf->cart_type;
  player->item=rcvbuf->item;
  player->is_finished=rcvbuf->is_finished;
  player->rank=rcvbuf->rank;
  player->x=rcvbuf->x;
  player->y=rcvbuf->y;
  player->vx=rcvbuf->vx;
  player->vy=rcvbuf->vy;
  player->angle=rcvbuf->angle;
  player->rank=rcvbuf->rank;
  player->lap=rcvbuf->lap;
  player->falling_time = rcvbuf->falling_time;
}


//サーバーから全キャラの情報を受け取る関数　受け取って種類に応じて処理する
void Receive_Game_Status(){
  static Uint32 server_dead =0;
  if(server_dead==0)server_dead=SDL_GetTicks();
  
  Server_SndData_t rcvbuf;
  int cnt=0;
  //受信バッファをできるだけ読む　でも応答に影響はないように
  while(Client_Receive(&rcvbuf,0) && cnt++<100){
    server_dead=SDL_GetTicks();
    //プレーヤーのデータ
    if(rcvbuf.data_type==PLAYER_DATA){
      int id=((Server_SndData_Player*)&rcvbuf)->id;
      if(0 <= id && id <PLAYER_NUM){
        Decode_Player_Data(&player_list[id], (Server_SndData_Player*)&rcvbuf);
      }
      else fprintf(stderr, "Receive_Game_Status : inveild player id\n");
    }
    //オブジェクト・アイテムデータ
    else if(rcvbuf.data_type==OBJECT_DATA){
      Server_SndData_Object *rcvobj = (Server_SndData_Object*)&rcvbuf;
      if(0<=rcvobj->id && rcvobj->id < MAX_OBJECT_NUM){
        object_list[rcvobj->id] = rcvobj->object;
      }
    }
    //残り時間
    else if(rcvbuf.data_type == TIME_COUNT){
      Server_SndData_Command* rcvcom=(Server_SndData_Command*)&rcvbuf;
      passed_time = rcvcom->data;
    }
    //レース終了
    else if(rcvbuf.data_type == GAME_END){
      is_racing = false;
      break;
    }
  }

  if(SDL_TICKS_PASSED(SDL_GetTicks(), server_dead+1000)){
    Show_Error("サーバーが応答しません");
    Error_exit();
  }
  
}


//キー入力をとって送る
void Input_And_Send(){
  Client_SndData_Command com;
  com.data_type=ACTION_COMMAND;
  com.data=NOMOVE;

  //イベント情報の更新 : これしないと入力状態が変わらない
  SDL_PumpEvents();

  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_RIGHT]) {
      com.data |= MOVE_RIGHT;
  }//右へ

  if(state[SDL_SCANCODE_LEFT]){
      com.data |= MOVE_LEFT;
  }//左へ

  if (state[SDL_SCANCODE_Z]) {
  com.data |= MOVE_UP;   
  }//アクセル

  if(state[SDL_SCANCODE_X]){
      com.data |= MOVE_DOWN;
  }//ブレーキ&バック
  
  if(state[SDL_SCANCODE_A] || state[SDL_SCANCODE_C]){  
    com.data |= USE_ITEM;
  }//アイテム

  if(state[SDL_SCANCODE_Q]){  
    Error_exit();
  }//終了


  if(ai_mode && com.data==NOMOVE){
      com.data =AI_Think_Command(&player_list[id]);
  }
  
  Send_to_Server(&com,sizeof(com));

}



