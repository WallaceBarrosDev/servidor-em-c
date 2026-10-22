#include "webserver.h"
#include "request.h"
#include "response.h"
#include "socketService.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void serverLog(Request *req) {
  char method[METHOD_SIZE];
  char path[PATH_SIZE];

  request_getMethod(req, method);
  request_getPath(req, path);

  printf("%s %s 200 ok\n", method, path);
}

bool wbs_listener(int port, int (*callback)(void)) {
  if(!socketService_create()) return false;
  if(!socketService_bind(port) || !socketService_listen(5)) return false;

  if (callback) {
    if (callback() < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      return false;
    }
  }

  int client_fd = -1;
  
  while(true) {
    if(!socketService_accept(&client_fd)) continue;
    
    Request *req = request_new();

    if(!request_parse(client_fd, req)) {
      socketService_close(&client_fd);
      continue;
    };

    if(!response(client_fd)) {
      socketService_close(&client_fd);
      continue;
    };

    serverLog(req);

    request_free(req);
    socketService_close(&client_fd);
  }

  return 0;
}
