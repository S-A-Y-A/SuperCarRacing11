#include"client.h"

void Game_Opening(){
  //画像読み込み
  SDL_Texture *press_space = Img_Load_Texture(renderer, "../resources/img/press_space_key.png");
  SDL_Texture *nanika = Img_Load_Texture(renderer, "../resources/img/nanika.png");
  SDL_Texture *title_img = Img_Load_Texture(renderer, "../resources/img/opening_back.png");
  SDL_Texture *title_logo = Img_Load_Texture(renderer, "../resources/img/title_logo.png");
  int nanika_x=-450;
  int nanika_time=SDL_GetTicks();
  SDL_Event ev;
  while(1){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    //titleっぽいものを表示
    Img_Draw_Image(renderer, title_img, 0, 0, WINDOW_W, WINDOW_H);
    Img_Draw_Image(renderer, title_logo, WINDOW_W/2-150, 80, 300, 90);
    Img_Draw_Image(renderer, press_space, WINDOW_W/2-150,400 ,300,50);

    //何かを表示
    Img_Draw_Image(renderer, nanika, nanika_x,250 ,90,60);
    //何かを動かすための処理 (100msごとにxを+10している)
    if(SDL_TICKS_PASSED(SDL_GetTicks(), nanika_time+100)){
      nanika_x+=10;
      if(nanika_x>WINDOW_W+450)nanika_x=-450;
    }
    //入力対応
    while(SDL_PollEvent(&ev)){
      if(ev.type==SDL_QUIT)exit(EXIT_FAILURE);
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_SPACE]) {
      break;
    }
    
    //画面反映
    SDL_RenderPresent(renderer);
    SDL_Delay(20);
  }

  SDL_DestroyTexture(press_space);
  SDL_DestroyTexture(nanika);
  SDL_DestroyTexture(title_img);
  SDL_DestroyTexture(title_logo);
}
