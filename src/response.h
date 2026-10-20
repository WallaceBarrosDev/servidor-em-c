#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct {
  void (*send)(const char *msg);
} Response;

Response *response_load(void);
void socketThread(int clientSocket);

#endif
