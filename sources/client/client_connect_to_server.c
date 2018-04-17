#include"client.h"

static void Window_Draw_Connecting(int param);

static SDL_Texture *title_img;

bool Connect_to_Server(const char *handlename){
  Client_SndData_Connect sndbuf;
  sndbuf.data_type=CONNECT_ORDER;
  strcpy(sndbuf.handle_name, handlename);
  Send_to_Server(&sndbuf, sizeof(sndbuf));
  
  Server_SndData_t rcvbuf;
  SDL_Event ev;
  int param=0;
  bool ret_flg=false;
  title_img = Img_Load_Texture(renderer, "../resources/img/opening_back.png");
  Uint32 time_out =SDL_GetTicks() +4000;
  while(!is_connected && !is_error){
    Window_Draw_Connecting(param++);
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
  SDL_DestroyTexture(title_img);
  return ret_flg;
}

void Window_Draw_Connecting(int param){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    //titleっぽいものを表示
    char message[20]="接続中";
    int len=strlen(message);
    for(int i=0;i<param%5;i++){
      message[len++]='.';
    }
    message[len]='\0';
    
    Img_Draw_Image(renderer, title_img, 0, 0, WINDOW_W, WINDOW_H);
    Text_Create_Draw_And_Destroy(renderer, message, 10, 10, (SDL_Color){0,0,0,255});
    //画面反映
    SDL_RenderPresent(renderer);
    SDL_Delay(20);
}
