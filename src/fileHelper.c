#include "fileHelper.h"
#include <stdio.h>
#include <stdlib.h>

static char *readFile(const char *path) {
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo: %s\n", path);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *html = malloc((size + 1)*sizeof(char));
  if (html == NULL) {
    printf("Erro ao alocar memória para o arquivo: %s\n", path);
    fclose(file);
    return NULL;
  }

  size_t bytesRead = fread(html, sizeof(char), size, file);
  html[bytesRead] = '\0';
  
  if (bytesRead != size) {
    printf("Erro ao ler o arquivo: %s\n", path);
    free(html);
    return NULL;
  }
  
  char header[200];
  int hlen = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n\r\n", size);

  char *response_http = malloc(hlen + size + 1);
  if (response_http == NULL) {
    printf("Erro ao alocar memória para o arquivo: %s\n", path);
    fclose(file);
    return NULL;
  }


  snprintf(response_http, hlen + size, "%s%s", header, html);
  free(html);
  fclose(file);

  return response_http;
}

static FileHelper fileHelper = {
  .readFile = readFile,
};

FileHelper *fileHelper_load(void) {
  return &fileHelper;
}
