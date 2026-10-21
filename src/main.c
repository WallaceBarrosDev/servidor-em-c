#include <stdio.h>

#include "app.h"

#define PORT 8080

static void onListening(void) {
  printf("Server escutando em http://localhost:%d\n", PORT);
}

static void helloWorld(Response *res) {
  res->sendFile("index.html");
}

int main(void) {
  App *app = app_load();
  app->use(USE_STATIC, "public");

  app->get("/", helloWorld);

  app->listen(PORT, onListening);
}
