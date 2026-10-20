#ifndef APP_H
#define APP_H

typedef struct {
  int (*listen)(int port, void (*callback)(void));
} App;

App *createApp(void);

#endif
