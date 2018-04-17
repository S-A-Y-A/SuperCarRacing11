#include<stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<resolv.h>

#include "netlib.h"
#include "errorlib.h"

int UDP_Socket_and_Bind(int port)
{
  int mysocket;
  struct sockaddr_in myaddr;
  /*ソケット作成*/
  if((mysocket=socket(PF_INET,SOCK_DGRAM,0))<0){
    Show_Error("ネットワークソケット作成に失敗しました");
    exit(EXIT_FAILURE);
  }
  /*自身のアドレス構造体設定*/
  bzero((char *)&myaddr,sizeof(myaddr));
  myaddr.sin_family=AF_INET;
  myaddr.sin_addr.s_addr=htonl(INADDR_ANY);/*使用NIC制約なし*/
  myaddr.sin_port=htons(port);/*設定ポート番号*/
  /*アドレス構造体の情報を生成ソケットに関連付け*/
  if(bind(mysocket,(struct sockaddr *)&myaddr,sizeof(myaddr))<0){
    Show_Error("ソケットのバインドに失敗しました");
    exit(EXIT_FAILURE);
  }
  return mysocket;
}

bool Get_Addr(const char *hostname,int port,struct sockaddr_in *p_addr)
{
  struct hostent *phostent=gethostbyname(hostname);
  if(phostent==NULL){
    return false;
  }else{
    /*サーバーのアドレス構造体の設定*/
    bzero((char *)p_addr,sizeof(struct sockaddr_in));
    p_addr->sin_family=AF_INET;
    p_addr->sin_port=htons(port);
    bcopy(phostent->h_addr,(char*)&(p_addr->sin_addr.s_addr),phostent->h_length);
  }
  return true;
}

int mySendto(int sock,const void *data,size_t size,int flg,const struct sockaddr *addr,socklen_t addrlen)
{
  int sent_len = sendto(sock, data, size, flg, (struct sockaddr *)addr, addrlen);
  if(sent_len==-1){
    int eno=errno;
    Show_Error("データが送信できませんでした");
    Output_Error_Log(strerror(eno), NULL);
    exit(EXIT_FAILURE);
  } 
  else {
    return sent_len;
  }
}

int myRecvfrom(int sock,void *rcvbuf,size_t bufsize,int flg,struct sockaddr *addr,socklen_t *addrlen)
{
  int read_len=recvfrom(sock,rcvbuf,bufsize,flg,addr,addrlen);
  if(read_len<0){
    int eno=errno;
    Show_Error("データの受信に失敗しました");
    Output_Error_Log(strerror(eno), NULL);
    exit(EXIT_FAILURE);
  }else{
    return read_len;
  }
}


bool Is_Same_sockaddr_in(const struct sockaddr_in *a1,const struct sockaddr_in *a2){
  if(a1->sin_port==a2->sin_port){
    if(a1->sin_addr.s_addr==a1->sin_addr.s_addr)return true;
  }
  return false;
}

bool check_ip_v4(const char *ip){
  if(strcmp(ip, "localhost")==0)return true;

  int ipn[5];
  if(sscanf(ip,"%d.%d.%d.%d.%d",&ipn[0],&ipn[1],&ipn[2],&ipn[3],&ipn[4])!=4)return false;
  for(int i=0;i<4;i++){
    if(ipn[i]<0 || 255<ipn[i])return false;
  }
  return true;
}
