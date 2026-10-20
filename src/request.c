#include "request.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LINE_BUF 8192

static bool requestReadPath(int fd, char *buf, size_t n) {
  char line[LINE_BUF];
  size_t len = 0;
  bool sawCrlfEnd = false;

  // consome TODOS os headers byte a byte, até \r\n\r\n, EOF ou estouro;
  // não pode deixar bytes para trás, senão o close() pode mandar RST
  while (len < LINE_BUF) {
    char c;
    ssize_t nr = read(fd, &c, 1);
    if (nr <= 0) break; // EOF ou erro: para com o que temos
    line[len++] = c;
    if (len >= 4 &&
        line[len - 4] == '\r' && line[len - 3] == '\n' &&
        line[len - 2] == '\r' && line[len - 1] == '\n') {
      sawCrlfEnd = true;
      break;
    }
  }
  (void)sawCrlfEnd;

  // primeira linha: METHOD SP path SP HTTP/x.y
  // 1º espaço → copia até o 2º espaço; malformado → "/"
  size_t pathStart = 0;
  while (pathStart < len && line[pathStart] != ' ') pathStart++;
  if (pathStart == len) goto fallback; // sem 1º espaço
  pathStart++;

  size_t pathEnd = pathStart;
  while (pathEnd < len && line[pathEnd] != ' ') pathEnd++;
  if (pathEnd == len) goto fallback; // sem 2º espaço

  size_t pathLen = pathEnd - pathStart;
  if (pathLen > n - 1) pathLen = n - 1;
  memcpy(buf, line + pathStart, pathLen);
  buf[pathLen] = '\0';
  return true;

fallback:
  if (n > 0) {
    buf[0] = '/';
    if (n - 1 > 0) buf[1] = '\0';
  }
  return true;
}

const Request request = { .readPath = requestReadPath };
