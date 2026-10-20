#include "networkSocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

static int fileDescriptor = -1;

bool creatSocket(void) {
  fileDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(fileDescriptor < 0) {
    perror("socket");
    return false;
  }

  return true;
}

void clearPort(void) {
  int opt = 1;
  setsockopt(fileDescriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
}

bool closeSocket(int socketd) {
  if(close(socketd) < 0) {
    perror("close");
    return false;
  };
  
  return true;
}

bool closeNetwork(void) {
  return closeSocket(fileDescriptor);
}

bool bindSocket(int port) {
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if(bind(fileDescriptor, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind");
    return false;
  }
  
  return true;
}

bool listenSocket(int backlog) {
  if(listen(fileDescriptor, backlog) < 0) {
    perror("listen");
    return false;
  }
  
  return true;
}

bool getClientSocket(int *clientSocket) {
  *clientSocket = accept(fileDescriptor, NULL, NULL);

  if(*clientSocket < 0) {
    perror("accept");
    return false;
  }
  
  return true;
}

static NetworkSocket networkSocket = {
  .create = creatSocket,
  .closeNetwork = closeNetwork,
  .clenPort = clearPort,
  .close = closeSocket,
  .bind = bindSocket,
  .listen = listenSocket,
  .getClient = getClientSocket
};

NetworkSocket *networkSocketInit(void) {
  return &networkSocket;
}
