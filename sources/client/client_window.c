#include<stdio.h>

#include "../common/game_common.h"
#include "client.h"

//--変数宣言--

SDL_Window *window;
SDL_Renderer *renderer;

Img_Textures cart_img;
const char *cart_img_src[]={
  "../resources/img/cart/cart1.png",
  "../resources/img/cart/cart2.png",
  "../resources/img/cart/cart3.png",
  "../resources/img/cart/cart4.png",
  "../resources/img/cart/cart5.png"
};

Img_Textures start_counts;
const char *start_counts_src[] = {
  "../resources/img/start_count/go.png",
  "../resources/img/start_count/one.png",
  "../resources/img/start_count/two.png",
  "../resources/img/start_count/three.png",
};

Img_Textures chara_img;
const char *chara_img_src[] = {
  "../resources/img/chara/chara1.png",
  "../resources/img/chara/chara2.png",
  "../resources/img/chara/chara3.png",
  "../resources/img/chara/chara4.png",
  "../resources/img/chara/chara5.png",
};


SDL_Rect item_rect = {WINDOW_W-65,WINDOW_H-65,60,60};

Img_Textures item_img;
const char *item_img_src[] = {
  "../resources/img/items/itembox.png",
  "../resources/img/items/squid.png",
  "../resources/img/items/thunder.png",
  "../resources/img/items/greenshell.png",
  "../resources/img/items/redshell.png",
  "../resources/img/items/blueshell.png"
};

Img_Textures item_rect_img;
const char *item_rect_img_src[] = {
  "../resources/img/items/squid_rect.png",
  "../resources/img/items/thunder_rect.png",
  "../resources/img/items/greenshell_rect.png",
  "../resources/img/items/redshell_rect.png",
  "../resources/img/items/blueshell_rect.png",
  "../resources/img/items/mushroom_rect.png"
};

SDL_Surface *icon_surface;

//--関数--

void Window_Init(){
  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0){
    Output_Error_Log(SDL_GetError(), NULL);
    Error_exit();
  }
  
  if(SDL_CreateWindowAndRenderer(WINDOW_W, WINDOW_H, 0, &window, &renderer)!=0){
    Output_Error_Log(SDL_GetError(), NULL);
    Error_exit();
  }
  
  icon_surface = IMG_Load("../resources/icon.png");
  if(icon_surface==NULL){
    Show_Error("アイコン画像が見つかりません");
    Error_exit();
  }
  SDL_SetWindowTitle(window, "Super Cart Racing 11");
  SDL_SetWindowIcon(window, icon_surface);
  
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  //マップ
  
  // -> レース直前部に移動した
  
  //画像
  Init_Img();
  start_counts = Img_Load_Textures(renderer, start_counts_src, 4);
  item_img = Img_Load_Textures(renderer, item_img_src,ITEMIMG_SIZE);
  item_rect_img = Img_Load_Textures(renderer, item_rect_img_src, ITEMRECTIMG_SIZE);
  chara_img = Img_Load_Textures(renderer, chara_img_src, SELECTABLE_CHARA_NUM);
  cart_img = Img_Load_Textures(renderer, cart_img_src, SELECTABLE_CHARA_NUM);
  //テキスト
  Init_Text("../resources/font.ttf", 20);
  
}

void Window_Delete(){
  Delete_Map();

  //画像解放
  Img_Free_Textures(cart_img);
  Img_Free_Textures(start_counts);
  Img_Free_Textures(item_img);
  Img_Free_Textures(item_rect_img);
  //テキスト解放
  
  Quit_Text();
  Quit_Img();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

//Press Space Key!

void Window_Clear(Uint8 r,Uint8 g,Uint8 b){
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
  SDL_RenderClear(renderer);
}

void Window_RenderPresent(){
  SDL_RenderPresent(renderer);
}


