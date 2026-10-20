#include "app.h"
#include "networkSocket.h"
#include "request.h"
#include "response.h"
#include "router.h"

#include <stdio.h>
#include <signal.h>

static void clientLoop(NetworkSocket *networkSocket, Router *router, Response *res) {
  while(true) {
    int client_fd = -1;
    if (!networkSocket->accept(&client_fd)) continue;
    responseSetFd(client_fd);

    char path[256];
    Handler handler = request.readPath(client_fd, path, sizeof path)
                    ? router->lookup(path)
                    : NULL;

    if (handler) { 
      handler(res);
      printf("GET %s 200 ok\n", path);
    }
    else {
      res->send("Not Found");
      printf("GET %s 404 not found\n", path);
    }

    networkSocket->closeClient(client_fd);
    responseSetFd(-1);
  }
}

static int serverListen(int port, void (*callback)(void)) {
  signal(SIGPIPE, SIG_IGN);

  NetworkSocket *networkSocket = networkSocket_load();

  if (!networkSocket->create()) return -1;
  networkSocket->reuseAddr();
  
  if (!networkSocket->bind(port) || !networkSocket->listen(10)) {
    networkSocket->close();
    return -1;
  }

  if (callback) callback();

  clientLoop(networkSocket, router_load(), response_load());
  return 0;
}

static void appGet(const char *path, Handler handler) {
  router_load()->add(path, handler);
}

static App app = {
  .listen = serverListen,
  .get = appGet
};

App *app_load(void) {
  return &app;
}
