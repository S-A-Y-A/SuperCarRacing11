#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>


/*
  画像読み込みライブラリ <imglib.h>  : SDL_Image のラッパー
 */


typedef struct Textures{
  int size;
  SDL_Texture **textures;
}Img_Textures;



/*
  ライブラリの初期化
*/
void Init_Img();

/*
  ライブラリの解放処理
*/
void Quit_Img();

/*
  画像を読み込む関数
  src :読み込む画像のパス
  SDL_Textureへのポインタを返す。
  いらなくなったらポインタは SDL_DestroyTexture で解放する
*/
SDL_Texture* Img_Load_Texture(SDL_Renderer *renderer,const char *src);

/*
  画像をまとめて読み込む関数
  srcs : 読み込む画像のパス　の配列
  size : その配列のサイズ（読み込む画像数）

  (Img_Textures型 : 上参照)
  Img_Textures型はいらなくなったらImg_Free_Texturesで解放する
*/
Img_Textures Img_Load_Textures(SDL_Renderer *renderer,const char **srcs,int size);

/*
  画像群を解放する関数
 */
void Img_Free_Textures(Img_Textures imgs);


/*
  画像を表示する関数 SDL_RenderCopy の劣化版
  
  (x,y)を端点とする幅w 高さhの領域に画像を表示する
 */
void Img_Draw_Image(SDL_Renderer *renderer,SDL_Texture *img_texture,int x,int y,int w,int h);


/*
  画像を表示する関数 SDL_RenderCopyEx の劣化版
  
  (x,y)を端点とする幅w 高さhの領域に angle 度だけ時計回りに傾けた画像を表示する
  回転の軸は画像の中心に固定している
 */
void Img_Draw_Image_Ex(SDL_Renderer *renderer,SDL_Texture *img_texture,int x,int y,int w,int h,double angle);

