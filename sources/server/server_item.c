#include "server.h"
#include "server_net.h"
#include"server_item.h"

static double get_dist(int x1,int y1,int x2,int y2){
  return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}
static double get_norm(double vx,double vy){
  return sqrt(vx*vx+vy*vy);
}
static void get_unit_vector(double vx,double vy,double *ex,double *ey){
  if(vx==0 && vy==0){
    *ex=*ey=0;
    return ;
  }
  *ex=vx/sqrt(vx*vx+vy*vy);
  *ey=vy/sqrt(vx*vx+vy*vy);
}


bool Apply_Item_Box(Object_Item_Box *itembox){
  if(!itembox->is_enable)return false;
  //無効時
  if(itembox->disable_time>0)itembox->disable_time-=ceil(SERVER_LOOP_MS/DT);
  //有効時
  else {
    itembox->disable_time=0;
    for(int i=0;i<PLAYER_NUM;i++){
      if(get_dist(itembox->x, itembox->y, player_list[i].x, player_list[i].y) <= ITEM_BOX_R+player_list[i].r){  //ここ調節必要!
        //ボックスに当たった!
        itembox->disable_time+=ITEM_BOX_DISABLETIME; //ここ調整必要
        if(player_list[i].item==ITEM_NOTHING){
          player_list[i].item = rand()%6+1;
        }        //ここアイテム数に応じて変えるべし
      }
    }
  }
  return true;
}

bool Apply_Squid_Ink(Item_Squid_Ink* ink){
  if(!ink->is_enable)return false;
  if(ink->left_time>0)ink->left_time-=ceil(SERVER_LOOP_MS/DT);
  if(ink->left_time<=0)ink->is_enable=false;
  return true;
}

bool Apply_Thunder(Item_Thunder *thunder){
  if(!thunder->is_enable)return false;
  if(thunder->left_time>0){
    thunder->left_time-=ceil(SERVER_LOOP_MS/DT);
    for(int i=0;i<PLAYER_NUM;i++){
      if(i==thunder->user)continue;
      //最高速度制限をかける
      if(get_norm(player_list[i].vx, player_list[i].vy)
         > player_list[i].v_max * THUNDER_DECELERATION){ //ここ調整必要
        get_unit_vector(player_list[i].vx,
                        player_list[i].vy,
                        &player_list[i].vx,
                        &player_list[i].vy);
        player_list[i].vx*=(player_list[i].v_max * THUNDER_DECELERATION);
        player_list[i].vy*=(player_list[i].v_max * THUNDER_DECELERATION);
      }
    }
  }
  if(thunder->left_time<=0)thunder->is_enable=false;
  return true;
}

bool Apply_GreenShell(Item_Green_Shell *shell){
  if(!shell->is_enable)return false;
  if(shell->left_time>0){
    shell->left_time-=ceil(SERVER_LOOP_MS/DT);
    double nx = shell->x+shell->vx/DT;
    double ny = shell->y+shell->vy/DT;
    if(Is_In_Map(ny, nx)){
      shell->x=nx;
      shell->y=ny;
    }
    else if(Is_In_Map(ny, shell->x)){
      shell->vx*=-1;
      shell->y=ny;
    }
    else if(Is_In_Map(shell->y, nx)){
      shell->x=nx;
      shell->vy*=-1;
    }
    else{
      shell->vx*=-1;
      shell->vy*=-1;
    }

    //衝突判定
    for(int i=0;i<PLAYER_NUM;i++){
      if(shell->user==i)continue;
      if(get_dist(shell->x, shell->y, player_list[i].x, player_list[i].y) <= player_list[i].r+GREEN_SHELL_R){  //ここ調節必要!
        shell->left_time=0;
        player_list[i].falling_time+=GREEN_SHELL_FALLTIME;
      }
    }
  }
  if(shell->left_time<=0)shell->is_enable=false;
  
  return true;
}

bool Apply_RedShell(Item_Red_Shell *shell){
  if(!shell->is_enable)return false;
  if(shell->left_time>0){
    if(player_list[shell->target].is_finished){
      shell->left_time=0;
    }
    shell->left_time-=ceil(SERVER_LOOP_MS/DT);
    shell->vx=player_list[shell->target].x-shell->x;
    shell->vy=player_list[shell->target].y-shell->y;
    get_unit_vector(shell->vx, shell->vy, &shell->vx, &shell->vy);
    shell->vx*=RED_SHELL_SPEED;
    shell->vy*=RED_SHELL_SPEED;
    //もうちょっとスピード調整必要?
    shell->x+=shell->vx/DT;
    shell->y+=shell->vy/DT;

    //衝突判定
    for(int i=0;i<PLAYER_NUM;i++){
      if(i==shell->user)continue;
      if(get_dist(shell->x, shell->y, player_list[i].x, player_list[i].y) <= player_list[i].r+RED_SHELL_R){  //ここ調節必要!
        shell->left_time=0;
        player_list[i].falling_time+=RED_SHELL_FALLTIME;
      }
    }
    
  }
  if(shell->left_time<=0)shell->is_enable=false;
  return true;
}

bool Apply_BlueShell(Item_Blue_Shell *shell){
  if(!shell->is_enable)return false;
  if(shell->left_time>0){
    if(player_list[shell->target].is_finished){
      for(int i=0;i<PLAYER_NUM;i++){
        if(player_list[ranking[i]].is_finished==false){
          shell->target =ranking[i];
          break;
        }
      }
    }
    shell->left_time-=ceil(SERVER_LOOP_MS/DT);
    shell->vx=player_list[shell->target].x-shell->x;
    shell->vy=player_list[shell->target].y-shell->y;
    get_unit_vector(shell->vx, shell->vy, &shell->vx, &shell->vy);
    shell->vx*=BLUE_SHELL_SPEED;
    shell->vy*=BLUE_SHELL_SPEED;
    //もうちょっとスピード調整必要?
    shell->x+=shell->vx/DT;
    shell->y+=shell->vy/DT;

    //衝突判定
    for(int i=0;i<PLAYER_NUM;i++){
      if(i==shell->user)continue;
      if(get_dist(shell->x, shell->y, player_list[i].x, player_list[i].y) <= player_list[i].r+BLUE_SHELL_R){
        if(i==shell->target || shell->target ==shell->user)shell->left_time=0;
        player_list[i].falling_time+=BLUE_SHELL_FALLTIME;
      }
    }
    
  }
  if(shell->left_time<=0)shell->is_enable=false;
  return true;

}

bool Apply_MushRoom(Item_MushRoom *mush){
  if(!mush->is_enable)return false;
  if(mush->left_time>0){
    mush->left_time-=ceil(SERVER_LOOP_MS/DT);
    //速度を底上げする
    player_list[mush->target].vx=MUSHROOM_ACCELERATION
                                 *player_list[mush->target].v_max
                                 *cos(player_list[mush->target].angle/180.0 * M_PI);
    player_list[mush->target].vy=MUSHROOM_ACCELERATION
                                 *player_list[mush->target].v_max
                                 *sin(player_list[mush->target].angle/180.0 * M_PI);
  }
  if(mush->left_time<=0){
    mush->is_enable=false;
  }
  return true;
}

void Apply_Items_And_Objects(){
  for(int i=0;i<MAX_OBJECT_NUM;i++){
    if(object_list[i].is_enable){
      int code = object_list[i].code;
      void * item=&object_list[i];
      if(code == ITEM_BOX)Apply_Item_Box(item);
      else if(code == SQUID_INK)Apply_Squid_Ink(item);
      else if(code == THUNDER)Apply_Thunder(item);
      else if(code == GREEN_SHELL)Apply_GreenShell(item);
      else if(code == RED_SHELL)Apply_RedShell(item);
      else if(code == BLUE_SHELL)Apply_BlueShell(item);
      else if(code == MUSHROOM)Apply_MushRoom(item);
    }
  }
}

void Send_Item_Data(){
  for(int i=0;i<MAX_OBJECT_NUM;i++){
    Server_SndData_Object sndbuf;
    sndbuf.data_type=OBJECT_DATA;
    sndbuf.id=i;
    sndbuf.object=object_list[i];
    Server_Send_to_All_Clients(&sndbuf, sizeof(Server_SndData_Object));
  }
}

bool Use_Item(Server_Player *p){
  int space=-1;
  for(int i=0;i<MAX_OBJECT_NUM;i++){
    if(object_list[i].is_enable==false){
      space=i;
      break;
    }
  }
  if(space<0)return false;//容量いっぱいでダメでした..

  
  if(p->item == SQUID_INK){
    Item_Squid_Ink *item = (void*)&object_list[space];
    item->code = SQUID_INK;
    item->is_enable =true;
    item->user=p->id;
    item->left_time=SQUID_INK_TIME;
  }
  else if(p->item == THUNDER){
    Item_Thunder *item = (void*)&object_list[space];
    item->code = THUNDER;
    item->is_enable =true;
    item->user=p->id;
    item->left_time=THUNDER_TIME;
  }
  else if(p->item == GREEN_SHELL){
    Item_Green_Shell *item = (void*)&object_list[space];
    item->code = GREEN_SHELL;
    item->is_enable =true;
    item->user=p->id;
    item->left_time=GREENSHELL_TIME;
    item->x=p->x;
    item->y=p->y;
    item->vx = cos(p->angle * M_PI / 180.0) * GREEN_SHELL_SPEED;
    item->vy = sin(p->angle * M_PI / 180.0) * GREEN_SHELL_SPEED;
  }
  else if(p->item == RED_SHELL){
    Item_Red_Shell *item = (void*)&object_list[space];
    item->code = RED_SHELL;
    item->is_enable =true;
    item->user=p->id;
    item->left_time=REDSHELL_TIME;
    item->x=p->x;
    item->y=p->y;
    item->vx = cos(p->angle * M_PI / 180.0) * RED_SHELL_SPEED;
    item->vy = sin(p->angle * M_PI / 180.0) * RED_SHELL_SPEED;
    
    if(p->rank!=1){//１位でなければ前がターゲット
      item->target =ranking[p->rank-1-1];
    }
    else if(p->rank!=PLAYER_NUM){//１位なら後ろがターゲット
      item->target =ranking[p->rank-1+1];
    }
    else{
      item->target=p->id;
    }
    //printf("Target %d\n",item->target);
  }

  else if(p->item == BLUE_SHELL){
    Item_Blue_Shell *item = (void*)&object_list[space];
    item->code = BLUE_SHELL;
    item->is_enable =true;
    item->user=p->id;
    item->left_time=BLUESHELL_TIME;
    item->x=p->x;
    item->y=p->y;
    item->vx = cos(p->angle * M_PI / 180.0) * BLUE_SHELL_SPEED;
    item->vy = sin(p->angle * M_PI / 180.0) * BLUE_SHELL_SPEED;
    for(int i=0;i<PLAYER_NUM;i++){
      if(player_list[ranking[i]].is_finished==false){
        item->target =ranking[i];
        break;
      }
    }
  }
  else if(p->item == MUSHROOM){
    Item_MushRoom *item = (void*)&object_list[space];
    item->code=MUSHROOM;
    item->is_enable=true;
    item->target=p->id;
    item->left_time=MUSHROOM_TIME;
  }
  
  //残りも書くべき
  p->item=ITEM_NOTHING;
  return true;
}



