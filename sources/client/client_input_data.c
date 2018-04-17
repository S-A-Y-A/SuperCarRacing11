#include<string.h>
#include"client.h"


static char name[11]={};
static char ip[21]={};

void Input_HandleName(char *handlename){
  SDL_Event ev;
  SDL_Texture *info = Text_Create_Texture(renderer, "ユーザー名はASCII文字のみ、最大１０文字です", (SDL_Color){255,255,255,255});
  SDL_Texture *info2 = Text_Create_Texture(renderer, "Enterで次に進みます", (SDL_Color){255,255,255,255});
   SDL_Texture *info3 = Text_Create_Texture(renderer, "AIから始まる名前にすると自動操縦モードになります", (SDL_Color){255,255,255,255});
   
  SDL_Texture *input_data = Img_Load_Texture(renderer, "../resources/img/handle_name.png");
  SDL_StartTextInput();
  SDL_Delay(100);
  while(SDL_PollEvent(&ev));
  while(1){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer); 
    //背景と入力画面の表示
    Img_Draw_Image_Ex(renderer, input_data,0,0,640,480,0);

    Text_Draw_Text(renderer, info, 100, 330);
    Text_Draw_Text(renderer, info2, 100, 360);
    Text_Draw_Text(renderer, info3, 100, 390);
    

    while(SDL_PollEvent(&ev)){
      if(ev.type==SDL_QUIT){
        exit(1);
      }
      else if(ev.type==SDL_TEXTINPUT){
        if(strlen(ev.text.text)==1 && ev.text.text[0]!=' '){
          if(strlen(name)<10){
            name[strlen(name)] = ev.text.text[0];
          }
        }
      }
      else if(ev.type==SDL_KEYDOWN){
        if(ev.key.keysym.scancode==SDL_SCANCODE_BACKSPACE){
          if(strlen(name)>0){
            name[strlen(name)-1]='\0';
          }
        }
        else if(ev.key.keysym.scancode==SDL_SCANCODE_RETURN){{
            if(strlen(name)>0){
              strcpy(handlename, name);
              
              if(strlen(name)>=2 && name[0]=='A'&&name[1]=='I'){
                ai_mode=true;
              }
              
              SDL_StopTextInput();
              goto HA_END;
          }
          }
        }
      }
    }
    
    //描写 ここ改善すべし



    Text_Create_Draw_And_Destroy(renderer, name, 110, 135, (SDL_Color){0,0,0,255});
    
    SDL_RenderPresent(renderer);
    SDL_Delay(20);
  }
HA_END:
  SDL_DestroyTexture(input_data);
  SDL_DestroyTexture(info);
  SDL_DestroyTexture(info2);
  SDL_DestroyTexture(info3);
}



void Input_ServerIP(sockaddr_in *addr){
  SDL_Event ev;
  SDL_Texture *input_data = Img_Load_Texture(renderer, "../resources/img/server_name.png");
  SDL_Texture *info = Text_Create_Texture(renderer, "IPv4アドレスまたはlocalhostのみ指定できます", (SDL_Color){255,255,255,255});
  SDL_Texture *info2 = Text_Create_Texture(renderer, "空白の場合はlocalhostが設定されます", (SDL_Color){255,255,255,255});
  SDL_Texture *info3 = Text_Create_Texture(renderer, "Enterで次に進みます", (SDL_Color){255,255,255,255});
  
  //char text[11]={};
  SDL_StartTextInput();
  SDL_Delay(100);
  while(SDL_PollEvent(&ev));
  while(1){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer); 
    //背景と入力画面の表示
    Img_Draw_Image_Ex(renderer, input_data,0,0,640,480,0);
    Text_Draw_Text(renderer, info, 100, 300);
    Text_Draw_Text(renderer, info2, 100, 330);
    Text_Draw_Text(renderer, info3, 100, 360);
    
    while(SDL_PollEvent(&ev)){
      if(ev.type==SDL_QUIT){
        exit(1);
      }
      else if(ev.type==SDL_TEXTINPUT){
        if(strlen(ev.text.text)==1){
          if(strlen(ip)<20){
            strcat(ip,ev.text.text);
          }
        }
      }
      else if(ev.type==SDL_KEYDOWN){
        if(ev.key.keysym.scancode==SDL_SCANCODE_BACKSPACE){
          if(strlen(ip)>0){
            ip[strlen(ip)-1]='\0';
          }
        }
        else if(ev.key.keysym.scancode==SDL_SCANCODE_RETURN){{
            if(strlen(ip)>0){
              if(check_ip_v4(ip) && Get_Addr(ip, SERVER_PORT, addr)){
                  SDL_StopTextInput();
                  goto IP_END;
              }
              else{
                char *title = "IPの形式が無効です";
                char *message = "IPv4アドレスxxx.xxx.xxx.xxx形式\nまたはこのPCのホスト名localhostのみが使用できます";
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, title,message, window);
                SDL_RaiseWindow(window);
                ip[0]='\0';
                SDL_Delay(100);
                while(SDL_PollEvent(&ev));
              }
            }
            else{
              char *def_ip = "localhost";
              strcpy(ip, def_ip);
              Get_Addr(ip, SERVER_PORT, addr);
              char *title = "localhostに接続します";
              char *message = "";
              SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, title,message, window);
              SDL_RaiseWindow(window);
            }
          }
        }
      }
    }
    
    //描写 ここ改善すべし

    Text_Create_Draw_And_Destroy(renderer, name, 110, 135, (SDL_Color){255,255,255,255});
    
    Text_Create_Draw_And_Destroy(renderer, ip, 110, 233, (SDL_Color){0,0,0,255});


    
    SDL_RenderPresent(renderer);
    SDL_Delay(20);
  }
IP_END:
  SDL_DestroyTexture(input_data);
  SDL_DestroyTexture(info);
  SDL_DestroyTexture(info2);
  SDL_DestroyTexture(info3);
  
}
