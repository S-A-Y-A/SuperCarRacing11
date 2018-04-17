
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include"game_common.h"
#include"errorlib.h"
#include "maplib.h"
#include "imglib.h"

#define MIN(i,j) ((i)<=(j)?(i):(j))
#define MAX(i,j) ((i)>=(j)?(i):(j))

static int window_h=WINDOW_H;
static int window_w=WINDOW_W;
static int MAP_H_Scale = DEFAULT_MAP_H_SCALE;
static int MAP_W_Scale = DEFAULT_MAP_W_SCALE;
static Map mapdata;
static bool is_inited=false;

static void Load_Map(Map *mapdata,const char *src);
static void Free_Map(Map *mapdata);

static Img_Textures map_images;
static SDL_Renderer *renderer;

/*
  Init_Map
  src       : 読み込むファイル
  window_h  : ウィンドウの幅
  window_w  : ウィンドウの幅
  scale_h   : 配列の倍率
  scale_w   : 配列の倍率
*/
void Init_Map(SDL_Renderer *renderer_, const char *src , int window_h_,int window_w_,int scale_h_,int scale_w_){
  if(is_inited){
    Free_Map(&mapdata);
  }
  is_inited=true;
  renderer=renderer_;
  Load_Map(&mapdata, src);
  window_h=window_h_;
  window_w=window_w_;
  MAP_H_Scale=scale_h_;
  MAP_W_Scale=scale_w_;
}

void Init_Map_sv(const char *src , int window_h_,int window_w_,int scale_h_,int scale_w_){ 
  if(is_inited){
    Free_Map(&mapdata);
  }
  is_inited=true;
  Load_Map(&mapdata, src);
  window_h=window_h_;
  window_w=window_w_;
  MAP_H_Scale=scale_h_;
  MAP_W_Scale=scale_w_;  
}

void Delete_Map(){
  if(is_inited){
    Free_Map(&mapdata);
    is_inited=false;
  }
}

bool Is_In_Map(int y,int x){
  y/=MAP_H_Scale;
  x/=MAP_W_Scale;
  if(y<0 || mapdata.H<=y || x<0 || mapdata.W<=x)return false;
  return mapdata.subdata.can_move[mapdata.data[y][x]/3]!=0;
}


//1 :進行 0:変化なし -1:後退 +-2 :スタートラインに入った +-3:スタートラインから出た
int Get_Progress_Param(int pre_y,int pre_x,int y,int x){
  pre_y/=MAP_H_Scale;
  pre_x/=MAP_W_Scale;
  y/=MAP_H_Scale;
  x/=MAP_W_Scale;
  if(y<0 || mapdata.H<=y || x<0 || mapdata.W<=x)return 0;

  int pre_param = mapdata.data[pre_y][pre_x]%3;
  int param = mapdata.data[y][x]%3;

  int ret=0;
  if(param==pre_param+1 || (param==0&& pre_param==2)){
    ret++;
    if(mapdata.data[y][x]/3==0)ret++;
    else if(mapdata.data[pre_y][pre_x]/3==0)ret+=2;
  }
  //後退
  else if(param==pre_param-1 || (param==2&& pre_param==0)){
    ret--;
    if(mapdata.data[y][x]/3==0)ret--;
    else if(mapdata.data[pre_y][pre_x]/3==0)ret-=2;
  }
  else if(param!=pre_param){
    Show_Error("深刻なバグが生じました　ランキングが正常に表示されないでしょう");
  }
  return ret;
}

void Show_Map(){
  for(int y=0;y<mapdata.H;y++){
    for(int x=0;x<mapdata.W;x++){
      //画面範囲外
      if((x+1)*MAP_W_Scale > window_w)continue;
      if((y+1)*MAP_H_Scale > window_h)continue;
      SDL_Rect rect;
      rect.y=y*MAP_H_Scale;
      rect.x=x*MAP_W_Scale;
      rect.h=MAP_H_Scale;
      rect.w=MAP_W_Scale;
      int img_numer = mapdata.data[y][x];
      SDL_RenderCopy(renderer, map_images.textures[img_numer], NULL, &rect);
    }
  }
  
}


/*
  座標(my,mx) が中心となるようにWindow幅分だけマップを表示
  gy,gx : ゲーム座標(window座標)
  mx,my : マップ配列座標 <-縮小されている
 */
void Show_MapRect(int c_gy,int c_gx){  
  Show_MapRect_under(c_gy, c_gx);
  Show_MapRect_over(c_gy, c_gx);
}

void Show_MapRect_under(int c_gy,int c_gx){
  int l_gy=c_gy-window_h/2;
  int r_gy=c_gy+window_h/2;
  int l_gx=c_gx-window_w/2;
  int r_gx=c_gx+window_w/2;

  int shift_x = l_gx % MAP_W_Scale;
  int shift_y = l_gy % MAP_H_Scale;

  int l_my=(c_gy-window_h/2)/MAP_H_Scale;
  int r_my=(c_gy+window_h/2)/MAP_H_Scale;
  int l_mx=(c_gx-window_w/2)/MAP_W_Scale;
  int r_mx=(c_gx+window_w/2)/MAP_W_Scale;
  
  for(int my=l_my-5;my<r_my+5;my++){
    if(my<0 || mapdata.H<=my)continue;
    for(int mx=l_mx-5;mx<r_mx+5;mx++){
      //画面範囲外
      if(mx<0 || mapdata.W<=mx)continue;

      SDL_Rect rect;
      rect.y=(my-l_my)*MAP_H_Scale-shift_y;
      rect.x=(mx-l_mx)*MAP_W_Scale-shift_x;
      rect.h=MAP_H_Scale;
      rect.w=MAP_W_Scale;
      //ベースの画像のみ描写
      int img_number = mapdata.subdata.base_img[ mapdata.data[my][mx]/3 ];
      if(mapdata.subdata.images_rate[img_number]>1){
        int rate =mapdata.subdata.images_rate[img_number];
        rect.x-=rect.w*(rate-1);
        rect.y-=rect.h*(rate-1);
        rect.h*=rate;
        rect.w*=rate;
      }
      SDL_RenderCopy(renderer, mapdata.subdata.images.textures[img_number], NULL, &rect);
    }
  }
}

void Show_MapRect_over(int c_gy,int c_gx){ 
  int l_gy=c_gy-window_h/2;
  int r_gy=c_gy+window_h/2;
  int l_gx=c_gx-window_w/2;
  int r_gx=c_gx+window_w/2;

  int shift_x = l_gx % MAP_W_Scale;
  int shift_y = l_gy % MAP_H_Scale;

  int l_my=(c_gy-window_h/2)/MAP_H_Scale;
  int r_my=(c_gy+window_h/2)/MAP_H_Scale;
  int l_mx=(c_gx-window_w/2)/MAP_W_Scale;
  int r_mx=(c_gx+window_w/2)/MAP_W_Scale;
  
  for(int my=l_my-5;my<r_my+5;my++){
    if(my<0 || mapdata.H<=my)continue;
    for(int mx=l_mx-5;mx<r_mx+5;mx++){
      //画面範囲外
      if(mx<0 || mapdata.W<=mx)continue;

      SDL_Rect rect;
      rect.y=(my-l_my)*MAP_H_Scale-shift_y;
      rect.x=(mx-l_mx)*MAP_W_Scale-shift_x;
      rect.h=MAP_H_Scale;
      rect.w=MAP_W_Scale;
      //本画像のみ描写
      int img_number = mapdata.data[my][mx]/3;
      //ベースと本画像が同じだったら描写しない
      if(img_number == mapdata.subdata.base_img[img_number])continue;
      if(mapdata.subdata.images_rate[img_number]>1){
        int rate =mapdata.subdata.images_rate[img_number];
        rect.x-=rect.w*(rate-1);
        rect.y-=rect.h*(rate-1);
        rect.h*=rate;
        rect.w*=rate;
      }
      SDL_RenderCopy(renderer, mapdata.subdata.images.textures[img_number], NULL, &rect);
    }
  }
}


void Draw_MiniMap_Image(int gy,int gx,int h,int w){
  //全体画像
  SDL_Rect map_rect;
  map_rect.x=gx;
  map_rect.y=gy;
  map_rect.w=w;
  map_rect.h=h;
  SDL_RenderCopy(renderer,mapdata.map_image,NULL,&map_rect);
}

void Draw_MiniMap_Player(int gy,int gx,int h,int w,int player_y,int player_x,SDL_Color color){
  //プレーヤ位置表示
  SDL_Rect map_player_rect;
  map_player_rect.w=5;
  map_player_rect.h=5;
  double rate_x=(double)w/(mapdata.W*50);
  double rate_y=(double)h/(mapdata.H*50);
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b,color.a);
  map_player_rect.x=player_x*rate_x+gx-map_player_rect.h/2;
  map_player_rect.y=player_y*rate_y+gy-map_player_rect.w/2;
  SDL_RenderFillRect(renderer,&map_player_rect);
}

void Draw_Object_In_MapRect(SDL_Texture *obj,int c_gy,int c_gx,int y,int x,int h,int w){
  int l_gy=c_gy-window_h/2;
  int r_gy=c_gy+window_h/2;
  int l_gx=c_gx-window_w/2;
  int r_gx=c_gx+window_w/2;
  
  if(l_gy-h<=y && y<=r_gy+h){
    if(l_gx-w<=x && x<=r_gx+w){
      SDL_Rect rect = (SDL_Rect){x-l_gx,y-l_gy,w,h};
      SDL_RenderCopy(renderer, obj, NULL, &rect);
    }
  }
}

//内部で使っている関数

void Load_Map_Sub_Data(Map_SubData *mapsub,const char *map_src){
  char *sub_src = calloc(strlen(map_src)+10, sizeof(char));
  if(sub_src==NULL){
    errno_t eno=errno;
    Show_Error("メモリ領域の取得に失敗しました");
    Output_Error_Log(strerror(eno), NULL);
    exit(1);
  }
  strcpy(sub_src, map_src);
  strcat(sub_src, "_sub.txt");
  
  FILE *f = fopen(sub_src, "r");
  if(f==NULL){
    errno_t eno=errno;
    Show_Error("マップサブデータファイルが開けませんでした");
    Output_Error_Log(strerror(eno), NULL);
    exit(1);
  }

  //ゴールまでの周回数
  fscanf(f, "%d",&GOAL_LAP);
  //スピードの倍率
  double speed_rate;
  fscanf(f,"%lf",&speed_rate);
  for(int i=0;i<PLAYER_NUM;i++){
    PLAYER_V_MAX[i]*=speed_rate;
  }
  BLUE_SHELL_SPEED*=speed_rate;
  RED_SHELL_SPEED*=speed_rate;
  GREEN_SHELL_SPEED*=speed_rate;
  

  //マップ画像データ
  int img_num;
  const char **img_srcs;
  fscanf(f,"%d",&img_num);
  mapsub->images_rate=malloc(sizeof(char)*img_num);
  mapsub->can_move=malloc(sizeof(char)*img_num);
  mapsub->base_img=malloc(sizeof(char)*img_num);
  img_srcs=malloc(sizeof(char*)*img_num);
  char *buf=NULL;
  size_t buf_size=0;
  for(int i=0;i<img_num;i++){
    if(getline(&buf, &buf_size, f)==-1){Show_Error("マップデータの読み込みに失敗しました");exit(EXIT_FAILURE);}
    if(getline(&buf, &buf_size, f)==-1){Show_Error("マップデータの読み込みに失敗しました");exit(EXIT_FAILURE);}
    
    char *pt=malloc(sizeof(char)*buf_size+1);
    if(pt==NULL){Show_Error("メモリ領域の取得に失敗しました");exit(EXIT_FAILURE);}
    
    int j=0;
    while(buf[j]!='\n'&&buf[j]!='\0'){pt[j]=buf[j];j++;}
    pt[j]='\0';
    //puts("line : ");
    //printf("%s\n",pt);
    fflush(stdout);
    img_srcs[i]=pt;
    int t1,t2,t3;
    fscanf(f,"%d%d%d",&t1,&t2,&t3);
    mapsub->base_img[i]=t1;
    mapsub->images_rate[i]=t2;
    mapsub->can_move[i]=t3;
    //printf("%d %d %d\n",t1,t2,t3);
  }
  free(buf);

  mapsub->images = Img_Load_Textures(renderer, img_srcs, img_num);

  
  //アイテムボックスデータ
  fscanf(f,"%d",&mapsub->item_box_num);
  mapsub->item_box_x=malloc(mapsub->item_box_num * sizeof(int));
  mapsub->item_box_y=malloc(mapsub->item_box_num * sizeof(int));

  for(int i=0;i<mapsub->item_box_num;i++){
    if(fscanf(f,"%d%d",&mapsub->item_box_x[i],&mapsub->item_box_y[i]) < 2){
      Show_Error("マップサブデータに不備があります");exit(EXIT_FAILURE);
      Output_Error_Log("the number of itembox in MapSubdata", NULL);
      exit(EXIT_FAILURE);
    }
  }

  //スタート地点データ
  fscanf(f,"%d",&mapsub->start_point_num);
  mapsub->start_point_x=malloc(mapsub->start_point_num * sizeof(int));
  mapsub->start_point_y=malloc(mapsub->start_point_num * sizeof(int));

  for(int i=0;i<mapsub->start_point_num;i++){
    if(fscanf(f,"%d%d",&mapsub->start_point_x[i],&mapsub->start_point_y[i]) < 2){
      Show_Error("マップサブデータに不備があります");exit(EXIT_FAILURE);
      Output_Error_Log("the number of start point in MapSubdata", NULL);
      exit(EXIT_FAILURE);
    }
  }
  free(sub_src);
}


void Load_Map(Map *mapdata,const char *src){
  FILE *f=fopen(src, "r");
  if(f==NULL){
    errno_t eno=errno;
    Show_Error("マップデータファイルが開けませんでした");
    Output_Error_Log(strerror(eno), NULL);
    exit(EXIT_FAILURE);
  }

  fscanf(f, "%d,%d",&mapdata->H,&mapdata->W);
  if(mapdata->H <= 0 || mapdata->W <= 0){
    Show_Error("マップデータサイズが無効です");
    exit(EXIT_FAILURE);
  }

  
  char *line=NULL;
  size_t bufsize=0;
  char *tok;
  
  mapdata->data=(char**)malloc(sizeof(char*)*mapdata->H);
  getline(&line, &bufsize, f);//空読み
  for(int h=0;h<mapdata->H;h++){
    getline(&line,&bufsize,f);
    if(line==NULL){
      Show_Error("マップデータに不備があります");
      free(line);
      exit(EXIT_FAILURE);
    }
    
    mapdata->data[h]=(char*)malloc(sizeof(char)*mapdata->W);
    
    tok=strtok(line, ",");
    for(int w=0;w<mapdata->W;w++){
      if(tok==NULL){
        Show_Error("マップデータに不備があります");
        free(line);
        exit(EXIT_FAILURE);
      }
      mapdata->data[h][w]=atoi(tok);
      tok=strtok(NULL,",");
    }
  }
  free(line);
  fclose(f);
  
  Load_Map_Sub_Data(&mapdata->subdata, src);

  mapdata->map_image = Create_Mini_Map(renderer, mapdata);

  return;
}

void Free_Map_Sub_Data(Map_SubData *sub){
  free(sub->item_box_x);
  free(sub->item_box_y);
  free(sub->start_point_x);
  free(sub->start_point_y);
  free(sub->base_img);
  free(sub->images_rate);
  free(sub->can_move);
}

void Free_Map(Map *mapdata){
  for(int i=0;i<mapdata->H;i++)free(mapdata->data[i]);
  free(mapdata->data);
  SDL_DestroyTexture(mapdata->map_image);
}

void Test_Print_Map_Data(Map *mapdata){
  puts("mapdata : \n");
  for(int i=0;i<mapdata->H;i++){
    printf("\t");
    for(int j=0;j<mapdata->W;j++){
      printf("%d ",mapdata->data[i][j]);
    }
    puts("");
  }
}

const Map_SubData* Get_Map_Sub_Data(){
  return &mapdata.subdata;
}

SDL_Texture* Create_Mini_Map(SDL_Renderer *renderer,const Map* mapdata){
  //コピペ
  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  
  SDL_Surface *surface = SDL_CreateRGBSurface(0, mapdata->W, mapdata->H, 32, rmask, gmask, bmask, amask);
  if(surface==NULL){
    Show_Error("マップ縮小図の生成に失敗しました");
    Output_Error_Log(SDL_GetError(), NULL);
    exit(1);
  }
  SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
  Uint32 movable_color=SDL_MapRGBA(surface->format, 255, 255, 255, 150);
  Uint32 wall_color=SDL_MapRGBA(surface->format, 0,0,0, 150);
  SDL_Rect rect =(SDL_Rect){0,0,1,1};
  for(int h=0;h<mapdata->H;h++){
    for(int w=0;w<mapdata->W;w++){
      char movable = mapdata->subdata.can_move[mapdata->data[h][w]/3];
      rect.x=w;
      rect.y=h;
      if(mapdata->data[h][w]/3==0){//スタート画面
        SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, 255, 255, 100, 200));
      }
      else if(movable)SDL_FillRect(surface, &rect, movable_color);
      else SDL_FillRect(surface, &rect, wall_color);
    }
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if(texture==NULL){
    Show_Error("マップ縮小テクスチャの生成に失敗しました");
    Output_Error_Log(SDL_GetError(), NULL);
    exit(1);
  }
  SDL_FreeSurface(surface);
  return texture;
}
