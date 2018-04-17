#include"../common/game_common.h"
#include"server.h"

void Window_Init(){
  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0){
    Output_Error_Log(SDL_GetError(), NULL);
    exit(EXIT_FAILURE);
  }
  
  Init_Img();
  Init_Text("../resources/font.ttf", 20);
  
  if(SDL_CreateWindowAndRenderer(SERVER_WINDOW_W, SERVER_WINDOW_H, 0, &window, &renderer)!=0){
    Output_Error_Log(SDL_GetError(), NULL);
    exit(EXIT_FAILURE);
  }
  
  SDL_SetWindowTitle(window, "Super Cart Racing 11 Server");

}


void Window_Delete(){
  Delete_Map();

  Quit_Text();
  Quit_Img();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Window_Clear(Uint8 r,Uint8 g,Uint8 b){
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
  SDL_RenderClear(renderer);
}

void Window_RenderPresent(){
  SDL_RenderPresent(renderer);
}
