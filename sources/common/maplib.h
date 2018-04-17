#pragma once

/*
  配列によるマップ管理ライブラリ
  CSVファイル(改行コードCL)からマップデータを読み込む
  rendererを受け取ってマップを表示
 */


/*
  CSVの形式:
  
  H,W
  1,2,3,4,...
  1,3,2,4,...

  1などはその座標の色番号
 */

#include<stdbool.h>
#include<SDL2/SDL.h>
#include"imglib.h"
#define DEFAULT_MAP_H_SCALE 50
#define DEFAULT_MAP_W_SCALE 50

enum MAP_BLOCK_TYPE{NONE,ROAD,WATER,C1,C2,C3,C4};

typedef struct Map_SubData{
  int item_box_num;
  int *item_box_x;
  int *item_box_y;
  
  int start_point_num;
  int *start_point_x;
  int *start_point_y;

  Img_Textures images;
  char *base_img;
  char *images_rate;
  char *can_move;
}Map_SubData;

typedef struct Map{
  int H,W;
  char **data;
  SDL_Texture *map_image;
  Map_SubData subdata;
}Map;

extern const unsigned char COLORS[][3];


void Init_Map(SDL_Renderer *renderer_, const char *src , int window_h_,int window_w_,int scale_h_,int scale_w_);

void Init_Map_sv(const char *src , int window_h_,int window_w_,int scale_h_,int scale_w_);

void Delete_Map();

void Show_Map();
void Show_MapRect(int c_gy,int c_gx);
void Show_MapRect_over(int c_gy,int c_gx);
void Show_MapRect_under(int c_gy,int c_gx);
void Draw_MiniMap_Image(int gy,int gx,int h,int w);
void Draw_MiniMap_Player(int gy,int gx,int h,int w,int player_y,int player_x,SDL_Color color);

void Show_Map_Fill_Color_ver();
void Show_MapRect_fill_Color_ver(int c_gy,int c_gx);

bool Is_In_Map(int y,int x);

void Test_Print_Map_Data(Map *mapdata);

int Get_Progress_Param(int pre_y,int pre_x,int y,int x);

const Map_SubData* Get_Map_Sub_Data();

void Draw_Object_In_MapRect(SDL_Texture *obj,int c_gy,int c_gx,int y,int x,int h,int w);

SDL_Texture* Create_Mini_Map(SDL_Renderer *renderer,const Map* mapdata);
