#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct {
  void (*send)(const char *body);
  void (*send_error)(int status, const char *reason);
} Response;

extern const Response response;
void responseUseFd(int fd); // seta o fd thread-local (-1 limpa)

#endif
