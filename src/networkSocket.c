#include "networkSocket.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static int server_fd = -1;

static bool networkSocketCreate(void) {
  server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_fd < 0) {
    perror("socket");
    return false;
  }
  return true;
}

static void networkSocketReuseAddr(void) {
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
}

static bool networkSocketBind(int port) {
  struct sockaddr_in address;
  memset(&address, 0, sizeof address);
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof address) < 0) {
    perror("bind");
    return false;
  }
  return true;
}

static bool networkSocketListen(int backlog) {
  if (listen(server_fd, backlog) < 0) {
    perror("listen");
    return false;
  }
  return true;
}

static bool networkSocketAccept(int *client_fd) {
  int fd;
  while (true) {
    fd = accept(server_fd, NULL, NULL);
    if (fd >= 0) break;
    if (errno == EINTR) continue;
    perror("accept");
    *client_fd = -1;
    return false;
  }
  *client_fd = fd;
  return true;
}

static bool networkSocketCloseClient(int fd) {
  if (close(fd) < 0) {
    perror("close");
    return false;
  }
  return true;
}

static bool networkSocketClose(void) {
  if (close(server_fd) < 0) {
    perror("close");
    return false;
  }
  server_fd = -1;
  return true;
}

const NetworkSocket networkSocket = {
  .create = networkSocketCreate,
  .reuseAddr = networkSocketReuseAddr,
  .bind = networkSocketBind,
  .listen = networkSocketListen,
  .accept = networkSocketAccept,
  .closeClient = networkSocketCloseClient,
  .close = networkSocketClose
};
