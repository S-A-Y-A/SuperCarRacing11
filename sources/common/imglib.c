#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

#include"imglib.h"
#include"errorlib.h"

void Init_Img(){
  int flags=IMG_INIT_PNG|IMG_INIT_JPG;
  int initted=IMG_Init(flags);
  if((initted&flags) != flags) {
    Show_Error("画像ライブラリの初期化に失敗しました");
    Output_Error_Log(IMG_GetError(), NULL);
    exit(EXIT_FAILURE);
  }
}


void Quit_Img(){
  IMG_Quit();
}


SDL_Texture* Img_Load_Texture(SDL_Renderer *renderer,const char *src){
  SDL_Surface *sf = IMG_Load(src);
  if(!sf){
    char message[200];
    sprintf(message,"画像 %s の読み込みに失敗しました",src);
    Show_Error(message);
    Output_Error_Log(IMG_GetError(), NULL);
    exit(EXIT_FAILURE);
  }
  SDL_Texture * tx = SDL_CreateTextureFromSurface(renderer, sf);
  if(!tx){
    Show_Error("テクスチャの作成に失敗しました");
    Output_Error_Log(IMG_GetError(), NULL);
    exit(EXIT_FAILURE);
  }
  SDL_FreeSurface(sf);
  return tx;
}

Img_Textures Img_Load_Textures(SDL_Renderer *renderer,const char **src,int size){
  Img_Textures imgs;
  imgs.size=size;
  imgs.textures=malloc(sizeof(SDL_Texture*)*size);
  for(int i=0;i<size;i++){
    imgs.textures[i]=Img_Load_Texture(renderer, src[i]);
  }
  return imgs;
}


void Img_Free_Textures(Img_Textures imgs){
  for(int i=0;i<imgs.size;i++){
    SDL_DestroyTexture(imgs.textures[i]);    
  }
  free(imgs.textures);
}


void Img_Draw_Image(SDL_Renderer *renderer,SDL_Texture *img_texture,int x,int y,int w,int h){
  SDL_Rect show_rect;
  show_rect.x=x;
  show_rect.y=y;
  show_rect.w=w;
  show_rect.h=h;
  SDL_RenderCopy(renderer,img_texture, NULL, &show_rect);
}

void Img_Draw_Image_Ex(SDL_Renderer *renderer,SDL_Texture *img_texture,int x,int y,int w,int h,double angle){
  SDL_Rect show_rect;
  show_rect.x=x;
  show_rect.y=y;
  show_rect.w=w;
  show_rect.h=h;
  SDL_RenderCopyEx(renderer, img_texture, NULL, &show_rect, angle, NULL, 0);
}
