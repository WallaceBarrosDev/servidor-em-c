#ifndef REQUEST_H
#define REQUEST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  bool (*readPath)(int fd, char *buf, size_t n);
} Request;

extern const Request request;

#endif
