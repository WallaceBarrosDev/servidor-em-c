#include "server.h"

int main(void) {
  serverInit(8080);
  serverListen();
  serverClose();
  return 0;
} 
