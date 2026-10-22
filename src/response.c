#include "response.h"

#include <iso646.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static char *concatenar(char *str1, char *str2) {
  size_t len1 = strlen(str1);
  size_t len2 = strlen(str2);

  char *result = (char *) malloc(len1+len2+1);

  if(!result) {
    printf("error: %s\n", strerror(errno));
    return NULL;
  }

  sprintf(result, "%s%s", str1, str2);

  return result;
}

bool response(int client_fd) {
  char header[] = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 0\r\n"
    "Connection: close\r\n"
    "\r\n"
  ;

  char html[] = "<h1>Ola mundo !</h1>";
/*
  char *complit = concatenar(header, html);

  printf("test");
  if(complit) return false;
  printf("%s", complit);
*/
  if (write(client_fd, header, strlen(header)) < 0) {
    printf("error: %s\n", strerror(errno));
    return false;
  };

  return true;
}
