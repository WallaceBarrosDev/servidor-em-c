#include "server.h"

int main(void) {
  App app = createApp();

  app.listen(8080);

  app.close();
  return 0;
} 
