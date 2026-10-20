#include "response.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static _Thread_local int client_fd = -1;

void responseSetFd(int fd) {
  client_fd = fd;
}

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

static Response response = {
  .send = responseSend,
};

Response *response_load(void) {
  return &response;
}
