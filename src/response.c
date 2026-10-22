#include "response.h"

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

bool response(int client_fd) {
  char header[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 0\r\n"
    "Connection: close\r\n"
  ;

  if (write(client_fd, header, strlen(header)) < 0) {
    printf("error: %s\n", strerror(errno));
    return false;
  };

  return true;
}
