#pragma once

#include"../common/game_common.h"

void Server_Net_Init(int port);

void Server_Net_Quit();

bool Server_Receive_from(Client_SndData_t *rcvbuf,int wait_time_ms,sockaddr_in *addr);

bool Server_Receive(Client_SndData_t *rcvbuf,int wait_time_ms);

void Server_Send_to(const Server_SndData_t *snddata,size_t size,const sockaddr_in *addr);

void Server_Send_to_All_Clients(const void *snddata,size_t size);
