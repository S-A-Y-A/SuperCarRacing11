#pragma once

#include"../common/game_common.h"

void Client_Net_Init();

void Client_Net_Quit();

bool Client_Receive(Server_SndData_t *rcvbuf,int wait_time_ms);

void Send_to_Server(void *snddata,size_t size);

