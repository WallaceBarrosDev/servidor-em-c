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
  
  fclose(file);
  return html;
}

static FileHelper fileHelper = {
  .readFile = readFile,
};

FileHelper *fileHelper_load(void) {
  return &fileHelper;
}
