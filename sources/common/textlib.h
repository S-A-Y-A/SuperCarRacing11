#ifndef TEXTLIB_H
#define TEXTLIB_H

#include<SDL2/SDL_ttf.h>

typedef struct Tetx_Textures{
  int size;
  SDL_Texture **textures;
}Text_Textures;


void Init_Text(char *font_src,int font_size);

void Quit_Text();

SDL_Texture* Text_Create_Texture(SDL_Renderer *renderer,const char *string,SDL_Color color);
Text_Textures Text_Load_Textures(SDL_Renderer *renderer,const char **strings,const SDL_Color *colors,const int size);


void Text_Free_Textures(Text_Textures textures);

void Text_Draw_Text(SDL_Renderer *renderer,SDL_Texture *text_texture,int x,int y);

void Tetx_Draw_Text_Ex(SDL_Renderer *renderer,SDL_Texture *text_texture,int x,int y,double angle);

//多用はしないほうが無難
void Text_Create_Draw_And_Destroy(SDL_Renderer *renderer,const char *string,int x,int y,SDL_Color color);

#endif
