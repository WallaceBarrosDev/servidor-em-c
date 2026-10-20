#ifndef APP_H
#define APP_H

typedef struct {
  int status;
  void (*send)(const char *msg);
} Response;

typedef struct {
  int (*listen)(int port, void (*callback)(void));
  int (*get)(const char *router, void (*callback)(Response *res));
} App;

App *createApp(void);

#endif
