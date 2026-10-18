#ifndef SERVER_H
#define SERVER_H

#include "defines.h"

typedef struct {
  int (*listen)(int port);
  void (*close)(void);
} App;

App createApp(void);

#endif
