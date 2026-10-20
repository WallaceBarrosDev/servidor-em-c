#include "app.h"
#include "networkSocket.h"
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>

int serverListen(int port, void (*callback)(void)) { 
  NetworkSocket *networkSocket = networkSocketInit();

  if(!networkSocket->create()) return -1;
  networkSocket->clenPort();

  if(!networkSocket->bind(port)) goto cleanup;

  if(!networkSocket->listen(10)) goto cleanup;
  callback();

  while(true) {
    int clientSocket = -1;
    if(!networkSocket->getClient(&clientSocket)) goto cleanup;

    read(clientSocket, NULL, 0);
    // o write rodaria depois desse read

    if(!networkSocket->close(clientSocket)) goto cleanup;
    printf("foi eba\n");
  }

  return networkSocket->closeNetwork();

cleanup:
  networkSocket->closeNetwork();
  return -1;
}

void responseSend(const char *msg) {
  write(clientSocket, msg, strlen(msg));
}

Response response = {
  .status = 200,
  .send = responseSend
};


int appGet(const char *router, void (*callback)(Response *res)) {
  callback(&response);
  printf("GET %s\n", router);
  return 0;
}

static App app = {
  .listen = serverListen,
  .get = appGet
};

App *createApp(void) {
  return &app;
}
