#include "response.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static _Thread_local int client_fd = -1;

void responseUseFd(int fd) {
  client_fd = fd;
}

// lida com write parcial e EINTR (loop); retorna 0 ok, -1 falha
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

  char header[128];
  int len = snprintf(header, sizeof header,
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/plain\r\n"
                     "Content-Length: %zu\r\n"
                     "Connection: close\r\n"
                     "\r\n",
                     strlen(body));
  if (len < 0 || (size_t)len >= sizeof header) return;
  if (writeAll(client_fd, header, (size_t)len) == 0)
    writeAll(client_fd, body, strlen(body));
}

static void responseSendError(int status, const char *reason) {
  if (client_fd == -1) return;

  char header[128];
  int len = snprintf(header, sizeof header,
                     "HTTP/1.1 %d %s\r\n"
                     "Content-Type: text/plain\r\n"
                     "Content-Length: 0\r\n"
                     "Connection: close\r\n"
                     "\r\n",
                     status, reason);
  if (len < 0 || (size_t)len >= sizeof header) return;
  writeAll(client_fd, header, (size_t)len);
}

const Response response = { .send = responseSend, .send_error = responseSendError };
