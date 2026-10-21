#include "response.h"
#include "fileHelper.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 200

static _Thread_local int client_fd = -1;

static void responseSetFd(int fd) {
  client_fd = fd;
}

static InternalResponse internalResponse = {
  .setFd = responseSetFd,
  .isStatic = false,
  .file = NULL,
};

static int writeAll(int fd, const char *buf, size_t len) {
  size_t sent = 0;
  while (sent < len) {
    ssize_t n = write(fd, buf + sent, len - sent);
    if (n < 0) {
      if (errno == EINTR) continue;
      perror("write");
      return -1;
    }
    sent += (size_t)n;
  }
  return 0;
}

static void responseSend(const char *body) {
  if (client_fd == -1) return;

  writeAll(client_fd, body, (size_t)strlen(body));
}

static void responseSendFile(const char *path) {
  if (client_fd == -1) return;
  
  if (!internalResponse.isStatic) {
    printf("arquivos staticos nao implementados\n");
    return;
  }

  char complitePath[SIZE];
  snprintf(complitePath, SIZE, "%s/%s", internalResponse.file, path);

  char *html = fileHelper_load()->readFile(complitePath);

  writeAll(client_fd, html, (size_t)strlen(html));
  printf("tamanho do arquivo em byts: %d\n", (int)strlen(html));

  free(html);
}

static Response response = {
  .send = responseSend,
  .sendFile = responseSendFile,
};


Response *response_load(void) {
  return &response;
}

InternalResponse *internalResponse_load(void) {
  return &internalResponse;
}
