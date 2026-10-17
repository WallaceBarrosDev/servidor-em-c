#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define SIZE_BUFFER 4096

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

Status creatBufferResponse(char *buffer_res, size_t tamanho, char *html) {
  Status status = {
    .has_error = false,
    .error_code = 0,
    .message = "buffer criado"
  };

  int res = snprintf(buffer_res, tamanho,
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=utf-8\r\n"
    "Connection: close\r\n"
    "\r\n"
    "%s",
    html);
    
  if (res < 0) criarErro(&status, "Erro ao criar o buffer de resposta");
  else if ((size_t)res >= tamanho) criarErro(&status, "Buffer de resposta maior que o size");

  return status;
}

Status lib_api(int port) {
  char buffer_req[SIZE_BUFFER];
  char buffer_res[SIZE_BUFFER];
  char *html = "<h1>Olá mundo !</h1>";

  Status status = {
    .has_error = false,
    .error_code = 0,
    .message = "socket criado, bind atribuido, resposta recebida e requisição enviada",
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

  ssize_t bytes_lidos = read(client_socketd, buffer_req, SIZE_BUFFER - 1);
  
  if(bytes_lidos < 0) {
    criarErro(&status, "falha ao ler a requisição do cliente");
    goto cleanup;
  }

  if(bytes_lidos == 0) {
    status.message = "cliente encerrou a conexão sem enviar dados";
    goto cleanup;
  }
  
  buffer_req[bytes_lidos] = '\0';

  Status s = creatBufferResponse(buffer_res, SIZE_BUFFER, html);
  if(s.has_error) {
    status = s;
    goto cleanup;
  }

  ssize_t bytes_escritos = write(client_socketd, buffer_res, strlen(buffer_res));
  
  if(bytes_escritos < 0) {
    criarErro(&status, "falha ao enviar a resposta ao cliente");
    goto cleanup;
  }

  if(bytes_escritos == 0) {
    status.message = "Aviso: respota enviada, mas nenhum dado foi escrito na resposta";
    goto cleanup;
  }

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
