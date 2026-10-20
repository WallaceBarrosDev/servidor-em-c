#ifndef SERVER_H
#define SERVER_H

typedef struct {
  int networkSocket;
} Server;

Server *createServer(void);

#endif
