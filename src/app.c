#include "app.h"
#include "networkSocket.h"
#include "request.h"
#include "response.h"
#include "router.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void clientLoop(NetworkSocket *networkSocket, Router *router, Response *res) {
  while(true) {
    int client_fd = -1;
    if (!networkSocket->accept(&client_fd)) continue;
    socketThread(client_fd);

    char path[256];
    Handler handler = request.readPath(client_fd, path, sizeof path)
                    ? router->lookup(path)
                    : NULL;

    if (handler) { 
      handler(res);
      printf("GET %s 200 ok\n", path);
    }
    else {
      res->send("pagina não encontrada\n");
      printf("GET %s 404 not found\n", path);
    }

    networkSocket->closeClient(client_fd);
    socketThread(-1);
  }
}

static void serverListen(int port, void (*callback)(void)) {
  signal(SIGPIPE, SIG_IGN);

  NetworkSocket *networkSocket = networkSocket_load();

  if (!networkSocket->create()) return;
  networkSocket->reuseAddr();
  
  if (!networkSocket->bind(port) || !networkSocket->listen(10)) {
    networkSocket->close();
    return;
  }

  if (callback) callback();

  clientLoop(networkSocket, router_load(), response_load());
}

static void appGet(const char *path, Handler handler) {
  Router *router = router_load();
  router->add(path, handler);
}

App *app_load(void) {
  App *app = malloc(sizeof(App));
  app->listen = serverListen;
  app->get = appGet;
  return app;
}
