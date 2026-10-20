#include "server.h"
#include <stdlib.h>

Server *createServer(void) {
  Server *server = malloc(sizeof(Server));
  server->networkSocket = -1;
  return server;
}
