#include "socketService.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int socket_fd = -1;

void socketService_close(int *socket_fd) {
  if(*socket_fd >= 0) close(*socket_fd);
}

bool socketService_create(void) {
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    fprintf(stderr, "error: %s\n", strerror(errno));
    return false;
  }

  return true;
}

bool socketService_bind(int port) {
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    fprintf(stderr, "error: %s\n", strerror(errno));
    return false;
  }

  return true;
}

bool socketService_listen(int backlog) {
  if (listen(socket_fd, backlog) < 0) {
    fprintf(stderr, "error: %s\n", strerror(errno));
    return false;
  }
  return true;
}

bool socketService_accept(int *clientSocket) {
  *clientSocket = accept(socket_fd, NULL, NULL);
  
  if (*clientSocket < 0) {
    fprintf(stderr, "error: %s\n", strerror(errno));
    return false;
  }
  return true;
}
