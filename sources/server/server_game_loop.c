#include"server.h"
#include "server_net.h"
#include"server_item.h"


static void Draw_Game_Window();

void Game_Loop(){
  SDL_Event ev;
  //始まりの儀式入れる
  Game_Init_Param();
  
  //レース中
  int next_time=passed_time+100;
  int debug_time=0;
  while(finished_num<PLAYER_NUM){
    while(SDL_PollEvent(&ev)){
      if(ev.type==SDL_QUIT){
        Interrupt_Exit();
      }
    }
    
    debug_time=SDL_GetTicks();
    //コマンド収集する
    //他の部分はほとんど時間がかからないのでここでwhile1回の時間がほぼ決まる
    Collect_Command(SERVER_LOOP_MS);
    
    Advance_Turn();
    Send_Game_Status();

    //経過時間の処理 0.1sごと
    if(SDL_TICKS_PASSED(SDL_GetTicks(), next_time)){
      next_time=SDL_GetTicks()+100;
      passed_time+=100;
      //時間を送る
      Server_SndData_Command sndbuf;
      sndbuf.data_type=TIME_COUNT;
      sndbuf.data=passed_time;
      Server_Send_to_All_Clients(&sndbuf, sizeof(sndbuf));
    }

    Draw_Game_Window();
    
    //printf("Time : %d\n",SDL_GetTicks()-debug_time);
  }

  SDL_Delay(1000);

  Server_SndData_Command sndbuf;
  sndbuf.data_type=GAME_END;
  Server_Send_to_All_Clients(&sndbuf, sizeof(sndbuf));
  
}

static void get_unit_vector(double vx,double vy,double *ex,double *ey){
  if(vx==0 && vy==0){
    *ex=*ey=0;
    return ;
  }
  *ex=vx/sqrt(vx*vx+vy*vy);
  *ey=vy/sqrt(vx*vx+vy*vy);
}
static double get_dist(double x1,double y1,double x2,double y2){
  return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
}
void input_change(Server_Player *p){
  int action = p->next_action_command;
  double nxt_vx = p->vx;
  double nxt_vy = p->vy;
  
  if(action & MOVE_UP){
      nxt_vx += cos(p->angle * M_PI / 180.0) * p->ax;
      nxt_vy += sin(p->angle * M_PI / 180.0) * p->ay;
  }
  else if(action & MOVE_DOWN){
    nxt_vx -= cos(p->angle * M_PI / 180.0) * p->ax;
    nxt_vy -= sin(p->angle * M_PI / 180.0) * p->ay;
  }

  if(passed_time>0){
  
  if(action & MOVE_LEFT){
    nxt_vx = p->vx*cos(p->handling_arg * M_PI / 180.0)
             + p->vy*sin(p->handling_arg * M_PI / 180.0);
    nxt_vy = - p->vx*sin(p->handling_arg * M_PI / 180.0)
             + p->vy*cos(p->handling_arg * M_PI / 180.0);
    p->angle-=p->handling_arg;
  }
  if(action & MOVE_RIGHT){
    nxt_vx = p->vx*cos(p->handling_arg * M_PI / 180.0)
             - p->vy*sin(p->handling_arg * M_PI / 180.0);
    nxt_vy = p->vx*sin(p->handling_arg * M_PI / 180.0)
             + p->vy*cos(p->handling_arg * M_PI / 180.0);
    p->angle+=p->handling_arg;

  }
  
  if(action & USE_ITEM){
      Use_Item(p);
  }

  }

  if(!(action & (MOVE_UP|MOVE_DOWN))){

  //床摩擦　　床依存にしてもいいかもしれない
  if(nxt_vx>=0){
    nxt_vx-=p->friction_a;
    if(nxt_vx<0)nxt_vx=0;
  }
  else{
    nxt_vx+=p->friction_a;
    if(nxt_vx>0)nxt_vx=0;
  }
  if(nxt_vy>=0){
    nxt_vy-=p->friction_a;
    if(nxt_vy<0)nxt_vy=0;
  }
  else{
    nxt_vy+=p->friction_a;
    if(nxt_vy>0)nxt_vy=0;
  }
  
  }
  
  //速度制限
  if(nxt_vx*nxt_vx + nxt_vy*nxt_vy > p->v_max*p->v_max){
    get_unit_vector(nxt_vx, nxt_vy, &nxt_vx, &nxt_vy);
    nxt_vx*=p->v_max;
    nxt_vy*=p->v_max;
  }
  p->vx = nxt_vx;
  p->vy = nxt_vy;

  //printf("x %lf y %lf\n",nx,ny);
  p->next_action_command = NOMOVE;
  
}

//Dt対応
void Falling_check(Server_Player *p){
  if(p->falling_time>0){
    if(p->falling_time>8000)p->falling_time=8000;
    //p->vx=p->vy=0;
    p->falling_time-=ceil(SERVER_LOOP_MS/DT);
    if(p->falling_time<0)p->falling_time=0;
  }
}

//Dt対応
void Move_Player(Server_Player *p){
  double nx; //doubleに変更
  double ny;
  nx = p->x + p->vx/(double)DT;
  ny = p->y + p->vy/(double)DT;
  
  if(Is_In_Map(ny, nx)==0){
    if(Is_In_Map(ny, p->x)==1){
      nx=p->x;
    }
    else if(Is_In_Map(p->y, nx)==1){
      ny=p->y;
    }
    else{
      nx=p->x;
      ny=p->y;
    }
  }

  
  //追加
  //順位用パラメータ更新
  int progress_param = Get_Progress_Param(p->y, p->x,ny,nx);
  //スタートライン越え処理
  static char start_in_out_flg[PLAYER_NUM]={};
  if(abs(progress_param)==2){//スタートラインに入った
    progress_param/=2;
    start_in_out_flg[p->id] = progress_param;
  }
  if(abs(progress_param)==3){//スタートラインから出た
    progress_param/=3;
    //入った側と違う側から出たらラップ更新
    if(start_in_out_flg[p->id] == progress_param){
      p->lap+= progress_param;
    }

    start_in_out_flg[p->id]=0;
  }
  p->rank_count+=progress_param;
  p->x=nx;
  p->y=ny;

}

void Collect_Command(int ms){
  static Uint32 client_dead[PLAYER_NUM]={0};//クライアント死亡判定用
  if(client_dead[0]==0){//初期状態の時
    for(int i=0;i<PLAYER_NUM;i++)client_dead[i]=SDL_GetTicks();
  }
  
  Uint32 time=SDL_GetTicks()+ms;
  while(!SDL_TICKS_PASSED(SDL_GetTicks(), time)){
    Client_SndData_t rcvbuf;
    if(Server_Receive(&rcvbuf, 1)){
      if(rcvbuf.data_type==ACTION_COMMAND){
        int id=rcvbuf.client_id;
        if(0<=id && id<PLAYER_NUM){
          Client_SndData_Command *rcv_command=(Client_SndData_Command*)&rcvbuf;
          if(player_list[id].falling_time==0){
            player_list[id].next_action_command=rcv_command->data;
          }
          client_dead[id]=SDL_GetTicks();
        }
      }
    }
  }

  //クライアントの死亡判定
  for(int i=0;i<PLAYER_NUM;i++){
    if(SDL_TICKS_PASSED(SDL_GetTicks(), client_dead[i]+1000)){
      Show_Error("クライアントが応答しません");
      Interrupt_Exit();
    }
  }
  
}

void confrict_move(Server_Player *p1,Server_Player *p2){
  //メモ: めり込み回避のために同じだけ戻るように変えてみた
  double revice = ( p1->r + p2->r - get_dist(p1->x, p1->y, p2->x, p2->y) )/2;
  double p1_nx = p1->x + revice * (p1->x < p2->x ? -1 : 1 );
  double p1_ny = p1->y + revice * (p1->y < p2->y ? -1 : 1 );
  double p2_nx = p2->x + revice * (p1->x < p2->x ?  1 : -1);
  double p2_ny = p2->y + revice * (p1->y < p2->y ?  1 : -1);
  
  if(Is_In_Map(p1_ny,p1_nx)){
    p1->x=p1_nx;
    p1->y=p1_ny;
  }
  if(Is_In_Map(p2_ny,p2_nx)){
    p2->x=p2_nx; 
    p2->y=p2_ny;
  }
}

void confrict_check(Server_Player *p1,Server_Player *p2){
  double e=0.5;/*反発係数*/
  double dist;
  dist = sqrt((p1->x-p2->x)*(p1->x-p2->x)+(p1->y-p2->y)*(p1->y-p2->y));
  if(p1->is_finished || p2->is_finished)return; //追加
  if(dist<=p1->r+p2->r){/*衝突*/
    double p1_vx,p2_vx,p1_vy,p2_vy;
    p1_vx=(p2->m*e*(p2->vx-p1->vx)+(p1->m*p1->vx+p2->m*p2->vx))/(p1->m+p2->m);
    p1_vy=(p2->m*e*(p2->vy-p1->vy)+(p1->m*p1->vy+p2->m*p2->vy))/(p1->m+p2->m);
    p2_vx=(p1->m*e*(p1->vx-p2->vx)+(p1->m*p1->vx+p2->m*p2->vx))/(p1->m+p2->m);
    p2_vy=(p1->m*e*(p1->vy-p2->vy)+(p1->m*p1->vy+p2->m*p2->vy))/(p1->m+p2->m);
    p1->vx=p1_vx;
    p1->vy=p1_vy;
    p2->vx=p2_vx;
    p2->vy=p2_vy;
    confrict_move(p1, p2);
  }
}

void Update_Rank(){
  //ゴールしてないプレーヤーだけ更新
  for(int i=finished_num;i<PLAYER_NUM;i++)ranking[i]=-1;
  for(int j=finished_num;j<PLAYER_NUM;j++){
    int max_count=(1<<30) * -1;
    for(int i=0;i<PLAYER_NUM;i++){
      int flg=0;
      for(int k=0;k<j;k++){
        if(ranking[k]==i){
          flg=1;
        }
      }
      if(flg==0){
        if(player_list[i].rank_count>max_count){
          max_count=player_list[i].rank_count;
          ranking[j]=i;
        }
      }
    }
  }

  for(int i=0;i<PLAYER_NUM;i++){
    player_list[ranking[i]].rank=i+1;
  }
  
}

void Check_Finish(){
  int i;
  for(i=0;i<PLAYER_NUM;i++){
    if(player_list[i].lap>=GOAL_LAP && !player_list[i].is_finished){
      player_list[i].is_finished=true;
      player_list[i].vx=player_list[i].vy=0;//追加　ゴール後動かないように
      player_list[i].finish_time=passed_time;
      finished_num++;
    }
  }
}

void Advance_Turn(){
  int i,j;
  for(i=0;i<PLAYER_NUM;i++){
    input_change(&player_list[i]);
  }

  //ここからDT回繰り返す
  
  for(int dt=0;dt<=DT;dt++){
    Apply_Items_And_Objects();
    
    if(passed_time>=0){
      for(i=0;i<PLAYER_NUM;i++){
        if(player_list[i].is_finished)continue;
        Falling_check(&player_list[i]); //追加
        Move_Player(&player_list[i]);
        for(j=0;j<PLAYER_NUM;j++){
          if(i!=j){
            confrict_check(&player_list[i], &player_list[j]);
          }
        }
      }
    }
  }
  
  
  
  Update_Rank();
  Check_Finish();
}

void Encode_Player_Data(Server_SndData_Player *sndbuf,const Server_Player *player){
  sndbuf->data_type=PLAYER_DATA;
  sndbuf->id=player->id;
  sndbuf->cart_type=player->chara_type;
  sndbuf->item=player->item;
  sndbuf->is_finished=player->is_finished;
  sndbuf->x=player->x;
  sndbuf->y=player->y;
  sndbuf->angle=player->angle;
  sndbuf->rank=player->rank;
  sndbuf->lap=player->lap;
  sndbuf->falling_time=player->falling_time;
}

void Send_Game_Status(){
  int i;
  Server_SndData_Player sndbuf;
  //プレーヤー情報 送る
  for(i=0;i<PLAYER_NUM;i++){
    Encode_Player_Data(&sndbuf, &player_list[i]);
    
    Server_Send_to_All_Clients(&sndbuf,sizeof(sndbuf));
  }

  //オブジェクト情報 送る
  Send_Item_Data();

}


void Draw_Game_Window(){
  static Uint32 fps = 0;

  if(SDL_TICKS_PASSED(SDL_GetTicks(), fps) || fps==0){
    fps=SDL_GetTicks()+20;
    
    Window_Clear(255,255,255);
    Draw_MiniMap_Image(30, 0, SERVER_WINDOW_H-30, SERVER_WINDOW_W);
    for(int i=0;i<PLAYER_NUM;i++){
      Draw_MiniMap_Player(30,0, SERVER_WINDOW_H-30, SERVER_WINDOW_W,
                          player_list[i].y,player_list[i].x,(SDL_Color){255,0,0,255});
    }
    Text_Create_Draw_And_Destroy(renderer, "ゲームが行われています", 5, 5, (SDL_Color){0,0,0,255});
    Window_RenderPresent();    
  }
}
