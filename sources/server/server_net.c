#include"server_net.h"
#include"server.h"

void Server_Net_Init(int port)
{
  sock=UDP_Socket_and_Bind(port);
  FD_ZERO(&fd);
  FD_SET(sock,&fd);
}

void Server_Net_Quit(){
  close(sock);
}

bool Server_Receive_from(Client_SndData_t *rcvbuf,int wait_time_ms,sockaddr_in *addr){
  struct timeval wait_time;
  wait_time.tv_sec=wait_time_ms/1000;
  wait_time.tv_usec=(wait_time_ms%1000)*1000;
  
  fd_set fd_cp;
  FD_COPY(&fd,&fd_cp);
  int ret=select(sock+1,&fd_cp,NULL,NULL,&wait_time);
  
  if(ret<0){perror("Error: select.");exit(EXIT_FAILURE);}
  
  if(ret>0 && FD_ISSET(sock,&fd_cp)){
    socklen_t addrlen=sizeof(sockaddr_in);
    myRecvfrom(sock, rcvbuf, sizeof(Client_SndData_t), 0, (struct sockaddr*)addr, &addrlen);
    
    return true;
  }
  else {
    rcvbuf->data_type=NODATA;
    return false;
  }
}

bool Server_Receive(Client_SndData_t *rcvbuf,int wait_time_ms)
{
  struct timeval wait_time;
  wait_time.tv_sec=wait_time_ms/1000;
  wait_time.tv_usec=(wait_time_ms%1000)*1000;
  
  fd_set fd_cp;
  FD_COPY(&fd,&fd_cp);
  int ret=select(sock+1,&fd_cp,NULL,NULL,&wait_time);
  
  if(ret<0){perror("Error: select.");exit(EXIT_FAILURE);}
  
  if(ret>0 && FD_ISSET(sock,&fd_cp)){
    sockaddr_in addr;
    socklen_t addrlen=sizeof(sockaddr_in);
    myRecvfrom(sock, rcvbuf, sizeof(Server_SndData_t), 0, (struct sockaddr*)&addr, &addrlen);

    if(rcvbuf->data_type==ERROR_EXIT){
      Show_Error("クライアントが終了しました");
      Interrupt_Exit();
    }
    
    return true;
  }
  else {
    rcvbuf->data_type=NODATA;
    return false;
  }
  
}


void Server_Send_to(const Server_SndData_t *snddata,size_t size,const sockaddr_in *addr)
{
  mySendto(sock, snddata, size, 0, (struct sockaddr*)addr, sizeof(sockaddr_in));
}


void Server_Send_to_All_Clients(const void *snddata,size_t size)
{
  for(int i=0;i<PLAYER_NUM;i++){
    if(player_list[i].is_connected)
      Server_Send_to(snddata, size, &player_list[i].addr);
  }
  
}
