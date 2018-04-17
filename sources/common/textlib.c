#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

#include"errorlib.h"
#include"textlib.h"

static TTF_Font *font;

void Init_Text(char *font_src,int font_size){
  if(TTF_Init() ==-1){
    Show_Error("フォントライブラリの初期化に失敗しました");
    Output_Error_Log(TTF_GetError(), NULL);
    exit(EXIT_FAILURE);
  }
  
  font=TTF_OpenFont(font_src, font_size);
  
  if(font==NULL){
    Show_Error("フォントファイルが開けませんでした");
    Output_Error_Log(TTF_GetError(), NULL);
    exit(EXIT_FAILURE);
  }
}

void Quit_Text(){
  TTF_CloseFont(font);
  TTF_Quit();
}


SDL_Texture* Text_Create_Texture(SDL_Renderer *renderer,const char *string,SDL_Color color){
  SDL_Surface *sf =TTF_RenderUTF8_Blended(font, string, color);
  if(!sf){
    Show_Error("文字列画像の生成に失敗しました");
    Output_Error_Log(TTF_GetError(), NULL);
    exit(EXIT_FAILURE);
  }
  SDL_Texture *tx =SDL_CreateTextureFromSurface(renderer, sf);
  if(!tx){
    Show_Error("文字列テクスチャの生成に失敗しました");
    Output_Error_Log(TTF_GetError(), NULL);
    exit(EXIT_FAILURE);
  }
  return tx;
}

Text_Textures Text_Load_Textures(SDL_Renderer *renderer,const char **strings,const SDL_Color *colors,const int size){
  Text_Textures texts;
  texts.size=size;
  texts.textures=malloc(sizeof(SDL_Texture*)*size);
  for(int i=0;i<size;i++){
    texts.textures[i]=Text_Create_Texture(renderer, strings[i],colors[i]);
  }
  return texts;
}


void Text_Free_Textures(Text_Textures textures){
  for(int i=0;i<textures.size;i++){
    SDL_DestroyTexture(textures.textures[i]);
  }
  free(textures.textures);
}

void Text_Draw_Text(SDL_Renderer *renderer,SDL_Texture *text_texture,int x,int y){
  SDL_Rect show_rect;
  show_rect.x=x;
  show_rect.y=y;
  SDL_QueryTexture(text_texture, NULL, NULL, &show_rect.w, &show_rect.h);
  SDL_RenderCopy(renderer,text_texture, NULL, &show_rect);
}

void Text_Draw_Text_Ex(SDL_Renderer *renderer,SDL_Texture *text_texture,int x,int y,double angle){
    SDL_Rect show_rect;
  show_rect.x=x;
  show_rect.y=y;
  SDL_QueryTexture(text_texture, NULL, NULL, &show_rect.w, &show_rect.h);
  SDL_RenderCopyEx(renderer, text_texture, NULL, &show_rect, angle, NULL, 0);
}

//ちょっとコストがかかるよ
void Text_Create_Draw_And_Destroy(SDL_Renderer *renderer,const char *string,int x,int y,SDL_Color color){
  if(strlen(string)<=0)return;
  SDL_Texture *texture = Text_Create_Texture(renderer, string, color);
  Text_Draw_Text(renderer, texture, x, y);
  SDL_DestroyTexture(texture);
  
}
