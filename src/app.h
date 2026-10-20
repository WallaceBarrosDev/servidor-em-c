#ifndef APP_H
#define APP_H

#include "response.h"

typedef void (*Handler)(Response *res);

typedef struct {
  int (*listen)(int port, void (*cb)(void));
  void (*get)(const char *path, Handler handler);
} App;

extern const App app;

#endif
