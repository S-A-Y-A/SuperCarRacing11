#pragma once

#include <SDL2/SDL.h>
#include "../common/game_common.h"

//--定数など
enum ITEM_IMG{
  ITEMIMG_ITEM_BOX,
  ITEMIMG_SQUID,ITEMIMG_THUNDER,ITEMIMG_GREENSHELL,ITEMIMG_REDSHELL,ITEMIMG_BLUESHELL,
  ITEMIMG_SIZE
};
enum ITEM_RECT_IMG{
  ITEMRECTIMG_SQUID,ITEMRECTIMG_THUNDER,ITEMRECTIMG_GREENSHELL,ITEMRECTIMG_REDSHELL,ITEMRECTIMG_BLUESHELL,ITEMRECTIMG_MUSHROOM,
  ITEMRECTIMG_SIZE
};

//--変数--
extern SDL_Window *window;
extern SDL_Renderer *renderer;


extern Img_Textures cart_img;
extern const char *cart_img_src[];
extern Img_Textures start_counts;
extern const char *start_counts_src[];
extern Img_Textures chara_img;
extern const char *chara_img_src[];
extern SDL_Rect item_rect;
extern Img_Textures item_img;
extern const char *item_img_src[];
extern Img_Textures item_rect_img;
extern const char *item_rect_img_src[];
extern SDL_Surface *icon_surface;

//--関数---

/**
   クライアントのウィンドウ描写関連の初期化
   テクスチャ（画像）、フォント、マップの読み込みなど
 */
void Window_Init();

/**
   クライアントのウィンドウ描写関連の解放処理
 */
void Window_Delete();

/**
   指定の色でWindowのキャンバスを塗りつぶす
   SDL_RenderClearと同じ
 */
void Window_Clear(Uint8 r,Uint8 g,Uint8 b);

/**
   SDL_RenderPresentと同じ
 */
void Window_RenderPresent();



