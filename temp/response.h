#ifndef RESPONSE_H
#define RESPONSE_H
#include "status.h"

typedef struct Response Response;

struct Response {
  int client_socketd;
  Status status;
  void (*send)(Response *self, const char* html);
};

Response initResponse(int client_socketd);
#endif
