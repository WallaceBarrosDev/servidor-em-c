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

bool limparSocket(int socketd) {
  if(socketd < 0)
    return true;
  
  return close(socketd) >= 0;
}

Status lib_api(int port) {
  Status status = {
    .has_error = false,
    .error_code = 0,
    .message = "socket criado e bind atribuido",
  };

  int socketd = -1;
  int client_socketd = -1;

  socketd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(socketd < 0) {
    criarErro(&status, "falha ao criar socket");
    goto cleanup;
  }
  
  struct sockaddr_in ip_porta = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr.s_addr = htonl(INADDR_LOOPBACK)
  };

  int res = bind(socketd, (struct sockaddr *) &ip_porta, sizeof(ip_porta));
  if(res < 0) {
    criarErro(&status, "falha ao fazer o bind");
    goto cleanup;
  }

  res = listen(socketd, 10);
  if(res < 0) {
    criarErro(&status, "falha ao deixar o socket em listen");
    goto cleanup;
  }

  printf("esperando conexão em http://localhost:%d\n", port);
  client_socketd = accept(socketd, NULL, NULL);
  if(client_socketd < 0) {
    criarErro(&status, "falha ao aceitar o socket do cliente");
    goto cleanup;
  }

  status.message = "conexão foi feita";

cleanup:
  if(!limparSocket(socketd) && status.error_code == 0)
    criarErro(&status, "falha ao fechar o socket");

  if(!limparSocket(client_socketd) && status.error_code == 0)
    criarErro(&status, "falha ao fechar o socket do cliente");

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
