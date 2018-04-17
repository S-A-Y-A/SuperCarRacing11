#include"server.h"
#include "server_net.h"
#include"server_item.h"

static void Window_Select_Cart(int selectednum);

void Select_Cart(){
  SDL_Event ev;
  Server_SndData_Command snddata;
  snddata.data_type=SELECT_CART_ORDER;
  Server_Send_to_All_Clients((Server_SndData_t*)&snddata, sizeof(snddata));

  for(int i=0;i<PLAYER_NUM;i++){
    player_list[i].chara_type=-1;
      
  }

  int received_num=0;
  Client_SndData_t rcvbuf;
  while(received_num<PLAYER_NUM){
    while(SDL_PollEvent(&ev)){
      if(ev.type==SDL_QUIT){
        Interrupt_Exit();
      }
    }
    if(Server_Receive(&rcvbuf, 20)){
      if(rcvbuf.data_type==SELECT_CART_RESPONSE){
        if(0<=rcvbuf.client_id && rcvbuf.client_id<PLAYER_NUM){
          //２回以上送られてきた対策
          if(player_list[rcvbuf.client_id].chara_type >= 0)continue;

          Client_SndData_Command *rcv_command = (Client_SndData_Command*)&rcvbuf;
          player_list[rcvbuf.client_id].chara_type = rcv_command->data;
          received_num++;
                  
        }
              
      }
    }
    Window_Select_Cart(received_num);
  }

  Server_SndData_GameStart snddata2;
  snddata2.data_type=GAME_START;
  snddata2.map_id =map_id;
  for(int i=0;i<PLAYER_NUM;i++){
    memcpy(snddata2.handle_names[i], player_list[i].handle_name, sizeof(player_list[i].handle_name));
      
  }
  Server_Send_to_All_Clients((Server_SndData_t*)&snddata2, sizeof(snddata2));
}

static void Window_Select_Cart(int selectednum){
  Window_Clear(100,30,10);
  Text_Create_Draw_And_Destroy(renderer, "カートを選択中", 10,10,
                               (SDL_Color){255,255,255,255});
  char str[30];
  sprintf(str,"選択済み : %d台",selectednum);
  Text_Create_Draw_And_Destroy(renderer, str, 10,40,
                               (SDL_Color){255,255,255,255});
  Window_RenderPresent();
}
