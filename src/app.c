#include "app.h"
#include "networkSocket.h"
#include "request.h"
#include "router.h"

#include <signal.h>

static int serverListen(int port, void (*cb)(void)) {
  signal(SIGPIPE, SIG_IGN);
  if (!networkSocket.create()) return -1;
  networkSocket.reuseAddr();
  if (!networkSocket.bind(port) || !networkSocket.listen(10)) {
    networkSocket.close();
    return -1;
  }
  if (cb) cb();

  for (;;) {
    int client_fd = -1;
    if (!networkSocket.accept(&client_fd)) continue; // accept interno já faz perror; erro não derruba o servidor
    responseUseFd(client_fd);

    char path[256];
    Handler handler = request.readPath(client_fd, path, sizeof path)
                    ? router.lookup(path)
                    : NULL;

    Response res = response;            // cópia de 8 bytes na stack, por request
    if (handler) handler(&res);
    else res.send_error(404, "Not Found");

    networkSocket.closeClient(client_fd);
    responseUseFd(-1);
  }
  /* inalcançável; loop infinito do servidor */
}

static void appGet(const char *path, Handler handler) {
  router.add(path, handler);
}

const App app = { .listen = serverListen, .get = appGet };
