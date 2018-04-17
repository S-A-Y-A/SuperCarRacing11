#include"client.h"


static void Window_Draw_Cart_Select(int selecting,int cart_angle);
static void Window_Draw_After_Cart_Select(int selected,int cart_angle);


//キャラの選択など
void Cart_Select(){
  int cart_type=0;
  int delay_cnt=0; //キー入力の反応間隔を調節するために追加
  int cart_angle=0;     //選択中のカートを回転させるための変数
  SDL_Event ev;
  //選択させる
  while(1){
    if(delay_cnt>8){
      while(SDL_PollEvent(&ev)){
        if(ev.type==SDL_QUIT)Error_exit();
      }
      const Uint8 *state = SDL_GetKeyboardState(NULL);
      if (state[SDL_SCANCODE_RIGHT]) {
        cart_type++;
      }
      if (state[SDL_SCANCODE_LEFT]) {
        cart_type--;
      }
      if (state[SDL_SCANCODE_Q]){
        Error_exit();
      }
      
      if(cart_type<0)cart_type=SELECTABLE_CHARA_NUM-1;
      if(cart_type>=SELECTABLE_CHARA_NUM)cart_type=0;
      
      if (state[SDL_SCANCODE_RETURN]) {
        break;
      }
      delay_cnt=0;
    }
    Window_Draw_Cart_Select(cart_type,cart_angle++);
    SDL_Delay(15);
    delay_cnt++;
  }



  
  //選択を送る
  Client_SndData_Command snddata;
  snddata.data_type=SELECT_CART_RESPONSE;
  snddata.data=cart_type;
  Send_to_Server(&snddata, sizeof(snddata));

  
  Server_SndData_t rcvbuf;
  while(!is_error){
    if(Client_Receive(&rcvbuf, 20)){
      if(rcvbuf.data_type==GAME_START){
        Server_SndData_GameStart *rcvdata=(void*)&rcvbuf;
        is_racing=true;
        map_id = rcvdata->map_id;
        for(int i=0;i<PLAYER_NUM;i++){
          memcpy(player_list[i].handle_name, rcvdata->handle_names[i], sizeof(player_list[i].handle_name));
        }
        break;
      }
    }
    //他のプレーヤの選択を待つ間の画面処理
    while(SDL_PollEvent(&ev)){
      if(ev.type==SDL_QUIT)Error_exit();
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_Q]) {
      Error_exit();
    }
    Window_Draw_After_Cart_Select(cart_type, cart_angle++);
    SDL_Delay(15);
  }
  
  check_error();
}



//Select your cart!
void Window_Draw_Cart_Select(int selecting,int cart_angle){
  if(selecting<0 || SELECTABLE_CHARA_NUM<=selecting)return ;
  
  SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
  SDL_RenderClear(renderer);
  
  SDL_Rect rect;
  int one_len=(WINDOW_W-5)/SELECTABLE_CHARA_NUM -10;
  //カートの種類数に応じて画面の隙間ができなようにキャラ枠の大きさを調節する仕様にした
  
  for(int i=0;i<SELECTABLE_CHARA_NUM;i++){
    rect = (SDL_Rect){i*(one_len+10)+5,60,one_len,one_len};
    SDL_RenderCopy(renderer, chara_img.textures[i], NULL, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
    if(i==selecting){
      //選択中を示す四角を表示
      rect=(SDL_Rect){i*(one_len+10)+5+(one_len/2-5),one_len + 60 +10,10,10};
      SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
      SDL_RenderFillRect(renderer, &rect);
    }
  }

  //選択中のカートを表示
  Img_Draw_Image_Ex(renderer, cart_img.textures[selecting],
                    WINDOW_W/2-60, WINDOW_H/2, 120, 120, cart_angle%360);
  Text_Create_Draw_And_Destroy(renderer, "カートを選択してください",
                               10, 10,(SDL_Color){255,255,255,255});
  
  SDL_RenderPresent(renderer);  
}


void Window_Draw_After_Cart_Select(int selected,int cart_angle){
  SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
  SDL_RenderClear(renderer);

  SDL_Rect rect;

  int one_len=(WINDOW_W-5)/SELECTABLE_CHARA_NUM -10;
  
  for(int i=0;i<SELECTABLE_CHARA_NUM;i++){
    rect = (SDL_Rect){i*(one_len+10)+5,60,one_len,one_len};
    SDL_RenderCopy(renderer, chara_img.textures[i], NULL, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
  }
  
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 190);
  rect=(SDL_Rect){0,0,WINDOW_W,WINDOW_H};
  SDL_RenderFillRect(renderer, &rect);
  
  Img_Draw_Image_Ex(renderer, cart_img.textures[selected], WINDOW_W/2-60, WINDOW_H/2, 120, 120, cart_angle%360);

  Text_Create_Draw_And_Destroy(renderer, "他のプレーヤーの選択を待っています", 10, 10, (SDL_Color){255,255,255,255});
  
  SDL_RenderPresent(renderer);  
}
