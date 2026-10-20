#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include <stdbool.h>

typedef struct {
  bool (*create)(void);           // cria socket TCP, guarda fd interno (static)
  void (*reuseAddr)(void);        // SO_REUSEADDR no fd interno
  bool (*bind)(int port);
  bool (*listen)(int backlog);
  bool (*accept)(int *client_fd); // retry automático em EINTR
  bool (*closeClient)(int fd);    // fecha fd do CLIENTE (recebido por parâmetro)
  bool (*close)(void);            // fecha fd interno do servidor e o reseta para -1
} NetworkSocket;

NetworkSocket *networkSocket_load(void);

#endif
