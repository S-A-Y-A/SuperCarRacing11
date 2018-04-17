#include"client.h"

static void Window_Draw_Ending(const Server_SndData_Result *result);


void Game_Ending(){
  Server_SndData_t rcvbuf;
  bool is_end=false;
  while(!is_end){
    if(!Client_Receive(&rcvbuf, 500)){
      ;
    }
    else{
      if(rcvbuf.data_type==RESULT_DATA){
        Server_SndData_Result *result = (Server_SndData_Result*)&rcvbuf;
        //結果をwindowに表示
        Window_Draw_Ending(result);
        is_end=true;
      }
    }
  }
}


//結果表示
void Window_Draw_Ending(const Server_SndData_Result *result){
  //メモ : タイムを動的に入れるために一部変更
  //       first_t,second_tなどを一つの配列で使い回すように変更した
  
  SDL_Texture *endbase = Img_Load_Texture(renderer, "../resources/img/end-base.png");
  SDL_Texture *result_img = Img_Load_Texture(renderer, "../resources/img/ending.png");
  SDL_Texture *back = Img_Load_Texture(renderer,"../resources/img/ending_back.png");
  SDL_Texture *message_img = Text_Create_Texture(renderer, "Spaceで終了", (SDL_Color){20,0,0,255});
  //1位
  char char_n[20]={};
  char char_t[10] ={};
  sprintf(char_n,"%s",player_list[result->ranking[0]].handle_name);
  sprintf(char_t, "%.1fsec",result->finish_time[0]/1000.0);
  SDL_Texture * first_name = Text_Create_Texture(renderer,char_n,(SDL_Color){0,0,0,255});
  SDL_Texture * first_time = Text_Create_Texture(renderer,char_t,(SDL_Color){0,0,0,255});
  //2位
  if(PLAYER_NUM>=2){
    sprintf(char_n,"%s",player_list[result->ranking[1]].handle_name);
    sprintf(char_t, "%.1fsec",result->finish_time[1]/1000.0);
  }
  else{
    strcpy(char_n, "None");
    strcpy(char_t, " ");
  }
  SDL_Texture * second_name = Text_Create_Texture(renderer,char_n,(SDL_Color){0,0,0,255});
  SDL_Texture * second_time = Text_Create_Texture(renderer,char_t,(SDL_Color){0,0,0,255});
  //3位
  if(PLAYER_NUM>=3){
    sprintf(char_n,"%s",player_list[result->ranking[2]].handle_name);
    sprintf(char_t, "%.1fsec",result->finish_time[2]/1000.0);
  }
  else{
    strcpy(char_n, "None");
    strcpy(char_t, " ");
  }
  SDL_Texture * third_name = Text_Create_Texture(renderer,char_n,(SDL_Color){0,0,0,255});
  SDL_Texture * third_time = Text_Create_Texture(renderer,char_t,(SDL_Color){0,0,0,255});
  //4位
  if(PLAYER_NUM>=4){
    sprintf(char_n,"%s",player_list[result->ranking[3]].handle_name);
    sprintf(char_t, "%.1fsec",result->finish_time[3]/1000.0);
  }
  else{
    strcpy(char_n, "None");
    strcpy(char_t, " ");
  }
  SDL_Texture * fourth_name = Text_Create_Texture(renderer,char_n,(SDL_Color){0,0,0,255});
  SDL_Texture * fourth_time = Text_Create_Texture(renderer,char_t,(SDL_Color){0,0,0,255});
  while(true){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    //中心に画像表示
    Img_Draw_Image(renderer, back, 0, 0, 640, 480);
    Img_Draw_Image(renderer, endbase, 0, 0, 640, 480);
    Img_Draw_Image(renderer, result_img, 70, 45, 500, 350);
    //1位
    Text_Draw_Text(renderer,first_name,200,90);
    Text_Draw_Text(renderer,first_time,350,90);
    //2位
    Text_Draw_Text(renderer,second_name,200,165);
    Text_Draw_Text(renderer,second_time,350,165);
    //3位
    Text_Draw_Text(renderer,third_name,200,240);
    Text_Draw_Text(renderer,third_time,350,240);
    //4位
    Text_Draw_Text(renderer,fourth_name,200,315);
    Text_Draw_Text(renderer,fourth_time,350,315);
    //メッセージ表示
    Text_Draw_Text(renderer, message_img, WINDOW_W/2-50, WINDOW_H-30);
    //画面反映
    SDL_RenderPresent(renderer);

    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_SPACE]) {
      break;
    }

    SDL_Delay(20);
  }
  SDL_DestroyTexture(first_name);
  SDL_DestroyTexture(first_time);
  SDL_DestroyTexture(second_name);
  SDL_DestroyTexture(second_time);
  SDL_DestroyTexture(third_name);
  SDL_DestroyTexture(third_time);
  SDL_DestroyTexture(fourth_name);
  SDL_DestroyTexture(fourth_time);
  SDL_DestroyTexture(endbase);
  SDL_DestroyTexture(result_img);
}
