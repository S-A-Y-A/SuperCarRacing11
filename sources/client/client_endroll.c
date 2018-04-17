#include"client.h"


static const char *endtexts[]={
  "製作：１１班",
  "",
  "このゲームでは以下のライブラリ、フォント、素材を使用しました",
  "  SDL2 , SDL_image2.0 , SDL_ttf2.0  (zlib license)",
  "  Ricty Diminished (SIL Open Font License (OFL) Version 1.1)",
  "  ドット絵世界 http://yms.main.jp",
  "  ねくらマップ http://nekuramap.blog.fc2.com",
};


void EndRoll(){
  SDL_Event ev;
  SDL_Color color= (SDL_Color){255,255,255,255};
  SDL_Delay(500);
  Uint32 time_wait =SDL_GetTicks()+1000;
  while(true){
    while(SDL_PollEvent(&ev)){
      if(ev.type==SDL_QUIT)goto ROLL_END;
      if(ev.type==SDL_KEYDOWN){
        if(SDL_TICKS_PASSED(SDL_GetTicks(), time_wait)){
          goto ROLL_END;    
        }
      }
    }


    Window_Clear(0, 0, 0);

    int text_num = sizeof(endtexts)/sizeof(char*);

    for(int i=0;i<text_num;i++){
      Text_Create_Draw_And_Destroy(renderer, endtexts[i], 30, 30 + i*30, color);
    }


    Window_RenderPresent();
    SDL_Delay(20);
  }
ROLL_END:
  ;
}
