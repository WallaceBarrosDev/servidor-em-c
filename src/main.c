#include "webserver.h"

#include <stdio.h>

#define PORT 8080

int callback(void) {
  return printf("servidor rodando em http://localhost:%d\n", PORT);
}

int main(void) {
    wbs_listener(PORT, callback);
    return 0;
}
