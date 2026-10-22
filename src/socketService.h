#ifndef SOCKET_SERVICE_H
#define SOCKET_SERVICE_H

#include <stdbool.h>

bool socketService_create(void);
void socketService_close(int *socket_fd);
bool socketService_bind(int port);
bool socketService_listen(int backlog);
bool socketService_accept(int *clientSocket);

#endif
