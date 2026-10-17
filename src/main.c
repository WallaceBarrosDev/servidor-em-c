#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

typedef struct {
  bool has_error;
  int error_code;
  char *message;
} Status;

void criarErro(Status *status, const char *message) {
  status->has_error = true;
  status->message = (char *) message;
  status->error_code = errno;
}

Status lib_api(int port) {
  Status status = {
    .has_error = false,
    .error_code = 0,
    .message = "socket criado e bind atribuido",
  };

  int socket_desc = -1;

  socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(socket_desc < 0) {
    criarErro(&status, "falha ao criar socket");
    goto cleanup;
  }
  
  struct sockaddr_in ip_porta = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr.s_addr = htonl(INADDR_LOOPBACK)
  };

  int res = bind(socket_desc, (struct sockaddr *) &ip_porta, sizeof(ip_porta));
  if(res < 0) criarErro(&status, "falha ao fazer o bind"); 
  
cleanup:
  if(socket_desc >= 0)
    if(close(socket_desc) < 0 && status.error_code == 0)
      criarErro(&status, "falha ao fechar o socket");

  return status;
}

int main(void) {
  Status status = lib_api(8080);

  if(status.has_error) {
    printf("%s: %s\n", status.message, strerror(status.error_code));
    return 1;
  }

  printf("%s\n", status.message);
  return 0;
} 
