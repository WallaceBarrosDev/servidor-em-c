#include <stdio.h>
#include "app.h"
#define PORT 8080

void msg(void) {
  printf("Server escutando em http://localhost:%d\n", PORT);
}

int main(void) {
  App *app = createApp();
  
  app->listen(PORT, msg);
  
  return 0;
} 
