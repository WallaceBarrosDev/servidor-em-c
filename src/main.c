#include <stdio.h>
#include <string.h>

#include "app.h"

#define PORT 8080

static void onListening(void) {
  printf("Server escutando em http://localhost:%d\n", PORT);
}

static void helloWorld(Response *res) {
  res->sendFile("index.html");
}

#define HTML_SIZE 200

static void api(Response *res) {
  
  struct { char *name; int age;} user[] = {
    {.name = "Wallace", .age = 23},
    {.name = "Ana Julia", .age = 17},
    {.name = "Lindiniese Julia", .age = 47},
    {.name = "William", .age = 43}
  };

  char html[HTML_SIZE];
  for (int i = 0; i < (int)(sizeof(user) / sizeof(user[0])); i++) { 
    int end_string = strlen(html);
    snprintf(html + end_string, HTML_SIZE, "<li>Nome: %s, idade: %d</li>\n\n\n", user[i].name, user[i].age);
  }

  res->send(html);
}

int main(void) {
  App *app = app_load();
  app->use(USE_STATIC, "public");

  app->get("/", helloWorld);
  app->get("/api", api); 

  app->listen(PORT, onListening);
}
