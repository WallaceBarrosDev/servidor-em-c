#ifndef STATUS_H
#define STATUS_H
#include "defines.h"

typedef struct {
  bool has_error;
  int error_code;
  char *message;
} Status;

void criarErro(Status *status, const char *message);

#endif
