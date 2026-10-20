#include "router.h"

#include <stdio.h>
#include <string.h>

typedef struct {
  const char *path;
  Handler handler;
} List;

static List table[MAX_ROUTES];
static int count = 0;

static void routeAdd(const char *path, Handler handler) {
  if (count >= MAX_ROUTES) {
    fprintf(stderr, "router: tabela cheia\n");
    return;
  }
  if (path == NULL || handler == NULL) {
    fprintf(stderr, "router: path ou handler nulo\n");
    return;
  }
  table[count].path = path;
  table[count].handler = handler;
  count++;
}

static Handler routeLookup(const char *path) {
  for (int i = 0; i < count; i++) {
    if (strcmp(table[i].path, path) == 0) return table[i].handler;
  }
  return NULL;
}

const Router router = { .add = routeAdd, .lookup = routeLookup };
