#include"server.h"
#include "server_net.h"
#include"server_item.h"

static void Draw_Wait_Client_Window();

void Wait_Client(){
  SDL_Event ev;
  Client_SndData_t rcvbuf;
  
  while(connected_num<PLAYER_NUM){
    while(SDL_PollEvent(&ev)){
      if(ev.type==SDL_QUIT){
        Interrupt_Exit();
      }
    }
    
    sockaddr_in cl_adder;
    if(Server_Receive_from(&rcvbuf, 20,&cl_adder)){
      if(rcvbuf.data_type==CONNECT_ORDER){
        Client_SndData_Connect *rcv_connect=(void*)&rcvbuf;
        for(int i=0;i<connected_num;i++){
          //同じクライアントを重複登録しない
          if(Is_Same_sockaddr_in(&cl_adder, &player_list[i].addr))continue;
        }
        
        int id=connected_num;
        player_list[id].is_connected=true;
        player_list[id].id=id;
        player_list[id].addr=cl_adder;
        memcpy(player_list[id].handle_name,rcv_connect->handle_name,sizeof(player_list[id].handle_name));

        Server_SndData_Command snddata;
        snddata.data_type=CONNECT_ACCEPT;
        snddata.data=id;
        //接続受理を知らせる
        Server_Send_to((Server_SndData_t*)&snddata, sizeof(snddata), &cl_adder);
        
        connected_num+=1;
      }
    }
    Draw_Wait_Client_Window();
    
  }
  SDL_Delay(1000);
}


static void Draw_Wait_Client_Window(){
  Window_Clear(20,100,10);
  Text_Create_Draw_And_Destroy(renderer, "クライアントを待機中", 10,10, (SDL_Color){255,255,255,255});
  char str[30];
  sprintf(str,"接続済み : %d台",connected_num);
  Text_Create_Draw_And_Destroy(renderer, str, 10,40, (SDL_Color){255,255,255,255});
  Window_RenderPresent();
}
