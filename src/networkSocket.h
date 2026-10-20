#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include <stdbool.h>

typedef struct {
  bool (*create)(void);
  void (*clenPort)(void);
  bool (*closeNetwork)(void);
  bool (*close)(int socketd);
  bool (*bind)(int port);
  bool (*listen)(int backlog);
  bool (*getClient)(int *clientSocket);
} NetworkSocket;

NetworkSocket *networkSocketInit(void);

#endif
