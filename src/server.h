#ifndef SERVER_H
#define SERVER_H

#include "defines.h"
#include "status.h"

void serverInit(int port);
void serverListen();
void serverClose();

#endif
