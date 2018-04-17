#include"client.h"

static void Window_Draw_Map_under();
static void Window_Draw_Map_over();
static void Window_Draw_Status();
static void Window_Draw_Time();
static void Window_Draw_Players();
static void Window_Draw_Objects();


//画面描写する関数　内部で　マップとキャラ　その他のものを描写する
void Draw_Window_Racing(){
  Window_Clear(255, 255,255);
  Window_Draw_Map_under();
  Window_Draw_Players();
  Window_Draw_Map_over();
  Window_Draw_Objects();
  Draw_MiniMap_Image(5,WINDOW_W-125,120,120);
  for(int i=0;i<PLAYER_NUM;i++){
    SDL_Color color;
    if(i==id)color=(SDL_Color){255,0,0,255};
    else color=(SDL_Color){0,255,0,255};
    Draw_MiniMap_Player(5, WINDOW_W-125, 120, 120, player_list[i].y, player_list[i].x, color);
  }
  Window_Draw_Status();
  Window_RenderPresent();
}

//プレーヤを中心としてマップを表示
void Window_Draw_Map_under(){
  Show_MapRect_under(player_list[id].y, player_list[id].x);
}

void Window_Draw_Map_over(){
  Show_MapRect_over(player_list[id].y, player_list[id].x);
}

void Window_Draw_Status(){
  //所有アイテムを左下に
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
  SDL_RenderFillRect(renderer, &item_rect);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(renderer,&item_rect);
  if(player_list[id].item!=0){
    int code = player_list[id].item;
    int img_idx=-1;
    if(code == SQUID_INK)img_idx = ITEMRECTIMG_SQUID;
    else if(code == THUNDER)img_idx = ITEMRECTIMG_THUNDER;
    else if(code == GREEN_SHELL)img_idx = ITEMRECTIMG_GREENSHELL;
    else if(code == RED_SHELL)img_idx = ITEMRECTIMG_REDSHELL;
    else if(code == BLUE_SHELL)img_idx = ITEMRECTIMG_BLUESHELL;
    else if(code == MUSHROOM)img_idx = ITEMRECTIMG_MUSHROOM;

    if(img_idx>=0){
      SDL_RenderCopy(renderer,item_rect_img.textures[img_idx],NULL,&item_rect);
    }

  }
  
  //周回表示
  SDL_Rect statrect=(SDL_Rect){10,10,150,70};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
  SDL_RenderFillRect(renderer, &statrect);

  char lap[15] ={};
  sprintf(lap,"LAP %d/%d",player_list[id].lap,GOAL_LAP);
  Text_Create_Draw_And_Destroy(renderer, lap, 15, 15, (SDL_Color){0,0,0,255});
  //順位表示
  char rank[15] ={};
  sprintf(rank,"RANK %dth",player_list[id].rank);
  Text_Create_Draw_And_Destroy(renderer, rank, 15, 35, (SDL_Color){0,0,0,255});
  //タイム表示
  Window_Draw_Time();
  
  //遊び方表示
  statrect=(SDL_Rect){0,WINDOW_H-30,450,30};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
  SDL_RenderFillRect(renderer, &statrect);
  Text_Create_Draw_And_Destroy(renderer, "Z:進む X:戻る AorC:アイテム 左右:ハンドル",10, WINDOW_H-25,(SDL_Color){0,0,0,255});
  

  if(player_list[id].is_finished){
    Text_Create_Draw_And_Destroy(renderer, "Finish!", 300, 230,(SDL_Color){200,200,0,255});
  }
  
}

void Window_Draw_Time(){
  //スタート秒読み
  if(passed_time<=0){
    if(-3000<=passed_time){
      // -999は-3000から-2001までを３秒前と出すための補正
      Img_Draw_Image(renderer,
                     start_counts.textures[abs(passed_time-999)/1000],
                     WINDOW_W/2-15, WINDOW_H/2-40, 60, 60);
    }
  }
  else{
    int minute = passed_time/1000/60;
    int second = passed_time/1000%60;
    int msecond = passed_time%1000;
    char time_string[10]={};
    sprintf(time_string,"%02d:%02d:%01d",minute,second,msecond/100);
    SDL_Texture *time_texture
        = Text_Create_Texture(renderer, time_string, (SDL_Color){0,0,0,255});
    Text_Draw_Text(renderer, time_texture, 15, 55);
    SDL_DestroyTexture(time_texture);
  }
}

//レース中の（全ての）プレーヤの表示
void Window_Draw_Players(){
  int my_wx = WINDOW_W/2;    //ウィンドウの左上をゼロとした時の座標
  int my_wy = WINDOW_H/2;
  int my_x=player_list[id].x;//マップ座標（配列でない）
  int my_y=player_list[id].y;

  static int falling_sub[PLAYER_NUM]={};
  for(int i=0;i<PLAYER_NUM;i++){
    //ゴールしたら消す
    if(player_list[i].is_finished)continue;
    
    int wx = player_list[i].x - my_x + my_wx;
    int wy = player_list[i].y - my_y + my_wy;

    if(player_list[i].falling_time>0){
      falling_sub[i] = (falling_sub[i]+5)%360;
    }
    else falling_sub[i] = 0;
    
    if( -CART_W<=wx && wx<WINDOW_W+CART_W && -CART_H<=wy && wy<WINDOW_H+CART_H ){//ウィンドウ内か？
      Img_Draw_Image_Ex(renderer, cart_img.textures[player_list[i].cart_type], wx-CART_W/2,wy-CART_H/2, CART_W, CART_H,player_list[i].angle + CART_ANGLE + falling_sub[i]);
      /*
      //テスト用円表示
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      for(double  x=-1*PLAYER_R[i];x<=PLAYER_R[i];x+=0.1){
        SDL_RenderDrawPoint(renderer, wx + x, -1*sqrt(PLAYER_R[i]*PLAYER_R[i] -x*x)+wy);
        SDL_RenderDrawPoint(renderer, wx + x,    sqrt(PLAYER_R[i]*PLAYER_R[i] -x*x)+wy);
      }
      */
      //テスト用終わり
      
    }
    
  }
  
}

//アイテムなどいろいろ表示
void Window_Draw_Objects(){
  for(int i=0;i<MAX_OBJECT_NUM;i++){
    if(object_list[i].is_enable){
      int code = object_list[i].code;
      if(code == ITEM_BOX){
        Object_Item_Box * item=(void*)&object_list[i];
        if(item->disable_time==0)
          Draw_Object_In_MapRect(item_img.textures[ITEMIMG_ITEM_BOX], player_list[id].y, player_list[id].x, item->y-15, item->x-15, 30, 30);
      }
      else if(code == SQUID_INK){
        Item_Squid_Ink *item=(void*)&object_list[i];
        if(item->user!=id){
          Img_Draw_Image(renderer, item_img.textures[ITEMIMG_SQUID], 0, 0, WINDOW_W, WINDOW_H);
        }
      }
      else if(code == THUNDER){
        Item_Thunder *item=(void*)&object_list[i];
        if(item->user!=id){
          Img_Draw_Image(renderer, item_img.textures[ITEMIMG_THUNDER], 0, 0, WINDOW_W, WINDOW_H);
        }
      }
      else if(code == GREEN_SHELL){
        Item_Green_Shell *item=(void*)&object_list[i];
        Draw_Object_In_MapRect(item_img.textures[ITEMIMG_GREENSHELL], player_list[id].y, player_list[id].x, item->y-15, item->x-15, 30, 30);
      }
      else if(code == RED_SHELL){
        Item_Red_Shell *item=(void*)&object_list[i];
        Draw_Object_In_MapRect(item_img.textures[ITEMIMG_REDSHELL], player_list[id].y, player_list[id].x, item->y-15, item->x-15, 30, 30);
      }
      else if(code == BLUE_SHELL){
        Item_Blue_Shell *item=(void*)&object_list[i];
        Draw_Object_In_MapRect(item_img.textures[ITEMIMG_BLUESHELL], player_list[id].y, player_list[id].x, item->y-15, item->x-15, 30, 30);

      }
      else if(code == MUSHROOM){
        //?
      }
    }
  }
}
