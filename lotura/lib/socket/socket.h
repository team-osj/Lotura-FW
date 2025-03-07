#ifndef _SOCKET_H
#define _SOCKET_H

#include "variable.h"

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
int SendStatus(int ch, bool status);
int SendLog(int ch, String log);

#endif