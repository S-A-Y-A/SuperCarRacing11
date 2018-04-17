#include"client.h"

static void Window_Draw_Waiting(int param);

static SDL_Texture *title_img;

void Wait_Other_Clients(){
  Server_SndData_t rcvbuf;
  bool is_gatherd=false;
  int param=0;
  title_img = Img_Load_Texture(renderer, "../resources/img/opening_back.png");
  SDL_Event ev;
  while(!is_gatherd){
    if(!Client_Receive(&rcvbuf, 20)){
      //受信待ち中の処理
      Window_Draw_Waiting(param++);

      while(SDL_PollEvent(&ev)){
        if(ev.type==SDL_QUIT){
          Error_exit();
        }
      }
      
    }
    else{
      if(rcvbuf.data_type==SELECT_CART_ORDER){
        is_gatherd=true;
        break;
      }
      printf("受信した SE\n");
      fflush(stdout);
    }
  }
  SDL_DestroyTexture(title_img);
}


void Window_Draw_Waiting(int param){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    //titleっぽいものを表示
    char message[20]="待機中";
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
