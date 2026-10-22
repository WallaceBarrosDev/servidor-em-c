#ifndef REQUEST_H
#define REQUEST_H

#include <stdbool.h>

#define BUFFER_SIZE 4096
#define METHOD_SIZE 100
#define PATH_SIZE 200

typedef struct Request Request;
Request *request_new(void);
void request_free(Request *request);

bool request_parse(int client_fd, Request *request);

void request_getMethod(Request *request, char *method);
void request_getPath(Request *request, char *path);

#endif
