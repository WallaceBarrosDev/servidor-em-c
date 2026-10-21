#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdbool.h>

typedef enum {
  USE_STATIC,
} UseOptions;

typedef struct {
  void (*send)(const char *msg);
  void (*sendFile)(const char *path);
} Response;

typedef struct {
  void (*setFd)(int fd);
  bool isStatic;
  const char *file;
} InternalResponse;

Response *response_load(void);
InternalResponse *internalResponse_load(void);

#endif
