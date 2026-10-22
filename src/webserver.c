#include "webserver.h"
#include "request.h"
#include "response.h"
#include "socketService.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

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
    
    if(!request(client_fd)) {
      socketService_close(&client_fd);
      continue;
    };

    if(!response(client_fd)) {
      socketService_close(&client_fd);
      continue;
    };
    
    printf("GET / 200 ok\n");
    socketService_close(&client_fd);
  }

  return 0;
}
