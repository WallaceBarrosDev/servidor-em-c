#include "request.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#define SIZE 4096

bool request(int client_fd) {
  char buffer[SIZE];

  if (read(client_fd, buffer, SIZE) < 0) {
    printf("error: %s\n", strerror(errno));
    return false;
  };
  
  return true;
}
