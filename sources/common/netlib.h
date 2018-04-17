#pragma once

#include<stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/time.h>


///バインドしたUDPソケットを返す
int UDP_Socket_and_Bind(int port);

///hostnameとportから sockaddrを作る
bool Get_Addr(const char *hostname, int port, struct sockaddr_in *p_addr);

//エラー処理を追加したsendto関数
int mySendto(int sock,const void *data,size_t size,int flg,const struct sockaddr *addr,socklen_t addrlen);

//エラー処理を追加したrecvfrom関数
int myRecvfrom(int sock,void *rcvbuf,size_t bufsize,int flg,struct sockaddr *addr,socklen_t *addrlen);


bool Is_Same_sockaddr_in(const struct sockaddr_in *a1,const struct sockaddr_in *a2);


bool check_ip_v4(const char *ip);
