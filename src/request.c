#include "request.h"

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Request {
  char method[METHOD_SIZE];
  char path[PATH_SIZE];
  int client_fd;
} Request;

Request *request_new(void) {
  Request *request = malloc(sizeof(Request));
  return request;
}

void request_free(Request *request) {
  free(request);
}

void request_getMethod(Request *request, char *method) {
  strncpy(method, request->method, METHOD_SIZE - 1);
}

void request_getPath(Request *request, char *path) {
  strncpy(path, request->path, PATH_SIZE - 1);
}

bool request_parse(int client_fd, Request *request) {
  char buffer[BUFFER_SIZE];

  int bytes = read(client_fd, buffer, BUFFER_SIZE);

  if (bytes < 0) {
    printf("error: %s\n", strerror(errno));
    return false;
  };

  strncpy(request->method, strtok(buffer, " "), METHOD_SIZE - 1);
  strncpy(request->path, strtok(NULL, " "), PATH_SIZE - 1);
  request->client_fd = client_fd;

  return true;
}
