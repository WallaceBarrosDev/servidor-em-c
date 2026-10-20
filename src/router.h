#ifndef ROUTER_H
#define ROUTER_H

#include "app.h"

#define MAX_ROUTES 64

typedef struct {
  void (*add)(const char *path, Handler handler);
  Handler (*lookup)(const char *path);
} Router;

extern const Router router;

#endif
