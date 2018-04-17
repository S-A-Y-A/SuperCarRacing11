#include<stdbool.h>

#include"client_net.h"
#include"../client/client.h"

void Client_Net_Init(){
  sock=UDP_Socket_and_Bind(0);
  FD_ZERO(&fd);
  FD_SET(sock,&fd);  
}

void Client_Net_Quit(){
  close(sock);
}

bool Client_Receive(Server_SndData_t *rcvbuf,int wait_time_ms){
  struct timeval wait_time;
  wait_time.tv_sec=wait_time_ms/1000;
  wait_time.tv_usec=(wait_time_ms%1000)*1000;

  fd_set fd_cp;
  FD_COPY(&fd,&fd_cp);
  int ret=select(sock+1,&fd_cp,NULL,NULL,&wait_time);
  
  if(ret<0){
    Show_Error("受信待機中にエラーが発生しました");
    Error_exit();
  }
  
  if(ret>0 && FD_ISSET(sock,&fd_cp)){
    sockaddr_in addr;
    socklen_t addrlen=sizeof(addr);
    myRecvfrom(sock, rcvbuf, sizeof(Server_SndData_t), 0, (struct sockaddr*)&addr, &addrlen);

    //サーバー同一性チェック
    if(!Is_Same_sockaddr_in(&server_addr, &addr)){
      fprintf(stderr,"received from another server ...\n");
      rcvbuf->data_type=NODATA;
      return false;
    }

    if(rcvbuf->data_type==ERROR_EXIT){
      Show_Error("サーバーが異常終了しました");
      SDL_RaiseWindow(window);
      Error_exit();
    }
    return true;
  }
  else {
    rcvbuf->data_type=NODATA;
    return false;
  }

}

void Send_to_Server(void *snddata,size_t size){
  ((Client_SndData_t*)snddata)->client_id=id;
  mySendto(sock, snddata, size, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
  
}
