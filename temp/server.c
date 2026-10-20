#include "server.h"
#include <unistd.h>

/*
typedef struct {
  int socketd;
} ServerApp;

static ServerApp serverApp = {
  .socketd = -1
};

static void reportarErro(const char *message) {
  int error_code = errno;

  if (error_code != 0)
    fprintf(stderr, "%s: %s\n", message, strerror(error_code));
  else
    fprintf(stderr, "%s\n", message);
}

static void closeSocket(int socketd) {
  if (close(socketd) < 0)
    fprintf(stderr, "falha ao fechar socket\n");
  else
    printf("socket fechado\n");
}

static int serverListen(int port) {
  if (serverApp.socketd < 0) {
    fprintf(stderr, "socket server não inicializado\n");
    return -1;
  }

  struct sockaddr_in ip_porta = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr.s_addr = htonl(INADDR_LOOPBACK)
  };

  if (bind(serverApp.socketd, (struct sockaddr *)&ip_porta, sizeof(ip_porta)) < 0) {
    reportarErro("falha ao fazer bind");
    return -1;
  }

  if (listen(serverApp.socketd, 10) < 0) {
    reportarErro("falha ao escutar");
    return -1;
  }

  printf("Server escutando em http://localhost:%d\n", port);

  char buffer[SIZE_BUFFER] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<h1>Hello World</h1>"
      ;

  while (true) {
    int client_socketd = accept(serverApp.socketd, NULL, NULL);

    if (client_socketd < 0) {
      reportarErro("falha ao aceitar a conexão");
      continue;
    } 

    char req[SIZE_BUFFER];
    int bytes = read(client_socketd, req, sizeof(req));
    
    if (bytes < 0) {
      reportarErro("falha ao ler o request");
      closeSocket(client_socketd);
      continue;
    }

    printf("req do cliente:\n%s\n", req);

    size_t total = strlen(buffer);
    size_t enviados = 0;
    
    while (enviados < total) {
      ssize_t bytes = write(
        client_socketd,
        buffer + enviados,
        total - enviados
      );

      if (bytes < 0) {
        reportarErro("falha ao enviar resposta");
        break;
      }

      enviados += (size_t)bytes;
    }

    if (enviados == total)
      printf("resposta enviada\n");

    closeSocket(client_socketd);
  }

  return 0;
}

static void serverClose(void) {
  if (serverApp.socketd >= 0) {
    closeSocket(serverApp.socketd);
    serverApp.socketd = -1;
  } else
    fprintf(stderr, "socket server não inicializado\n");
}

App createApp(void) {
  serverApp.socketd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (serverApp.socketd < 0) {
    reportarErro("falha ao criar socket");
    serverApp.socketd = -1;
  } else
    printf("server inicializado\n");

  return (App) {
    .listen = serverListen,
    .close = serverClose
  };
}
*/
