#include <stdio.h>
#include "app.h"
#define PORT 8080

void msg(void) {
  printf("Server escutando em http://localhost:%d\n", PORT);
}

void helloWorld(Response *res) {
  res->send("Hello World !\n");
} 

int main(void) {
  App *app = createApp();

  app->get("/", helloWorld);
  
  app->listen(PORT, msg);
  
  return 0;
} 
