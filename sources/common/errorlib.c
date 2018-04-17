#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<SDL2/SDL.h>
#include<stdbool.h>

#include"errorlib.h"

#define ERROR_LOG_FILE "./error_log.txt"
static bool Show_Error_Stdout(const char *message);
static bool Show_Error_Window(const char *message);


void Show_Error(const char *message){
  if(Show_Error_Window(message))return;
  if(Output_Error_Log(message, ERROR_LOG_FILE))return;
  if(Show_Error_Stdout(message))return;
  //致命的...
  abort();
}


bool Output_Error_Log(const char *message,const char *path){
  if(path==NULL)path=ERROR_LOG_FILE;
  FILE *f = fopen(path, "a");//追加書き込み
  if(f==NULL)return false;
  time_t timer;
  time(&timer);
  struct tm *now = localtime(&timer);
  fprintf(f,"Error : %d/%d/%d %d:%d:%d\n",now->tm_year,now->tm_mon,now->tm_mday,
          now->tm_hour,now->tm_min,now->tm_sec);
  fprintf(f,"\t%s\n",message);
  fclose(f);
  return true;
}

static bool Show_Error_Stdout(const char *message){
  printf("Error : %s\n",message);
  return true;
}


static bool Show_Error_Window(const char *message){
  Uint32 subsystem_mask =SDL_INIT_VIDEO|SDL_INIT_EVENTS;
    bool init_flg=false;
    if(SDL_WasInit(subsystem_mask) != subsystem_mask ){
      if(SDL_Init(subsystem_mask)!=0)return false;//SDL初期化失敗
      init_flg=true;
    }
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "エラーが発生しました", message, NULL);
    if(init_flg)SDL_Quit();
    return true;
}
