#include "server.h"
#include "status.h"

typedef struct {
  int port;
  int socketd;
  bool initialized;
} ServerCore;

static ServerCore serverCore = {
  .port = 0,
  .socketd = -1,
  .initialized = false
};

bool limparSocket(int socketd) {
  if (socketd < 0)
    return true;

  return close(socketd) >= 0;
}

static void reportarErro(const char *message) {
  int error_code = errno;

  if (error_code != 0)
    fprintf(stderr, "%s: %s\n", message, strerror(error_code));
  else
    fprintf(stderr, "%s\n", message);
}

Status creatBufferResponse(
    char *buffer_res,
    size_t tamanho,
    const char *html
) {
  Status status = {
    .has_error = false,
    .error_code = 0,
    .message = "Buffer criado"
  };

  int res = snprintf(
    buffer_res,
    tamanho,
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=utf-8\r\n"
    "Connection: close\r\n"
    "\r\n"
    "%s",
    html
  );

  if (res < 0) {
    status.has_error = true;
    status.error_code = 0;
    status.message = "Erro ao criar o buffer de resposta";
  } else if ((size_t)res >= tamanho) {
    status.has_error = true;
    status.error_code = 0;
    status.message = "Buffer de resposta insuficiente";
  }

  return status;
}

void serverInit(int port) {
  serverCore.initialized = false;
  serverCore.port = port;

  serverCore.socketd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (serverCore.socketd < 0) {
    reportarErro("Falha ao criar socket");
    return;
  }

  struct sockaddr_in ip_porta = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr.s_addr = htonl(INADDR_LOOPBACK)
  };

  if (bind(serverCore.socketd, (struct sockaddr *)&ip_porta, sizeof(ip_porta)) < 0) {
    reportarErro("Falha ao fazer bind");
    goto cleanup;
  }

  if (listen(serverCore.socketd, 10) < 0) {
    reportarErro("Falha ao executar listen");
    goto cleanup;
  }

  serverCore.initialized = true;
  return;

cleanup:
  limparSocket(serverCore.socketd);
  serverCore.socketd = -1;
}

void serverListen(void) {
  if (!serverCore.initialized) {
    fprintf(stderr, "Não é possível aceitar conexões: servidor não inicializado\n");
    return;
  }

  printf("Servidor funcionado em http://localhost:%d\n", serverCore.port);

  while (true) {
    Status status = {
        .has_error = false,
        .error_code = 0,
        .message = "Requisição atendida"
    };

    char buffer_req[SIZE_BUFFER];
    char buffer_res[SIZE_BUFFER];
    const char *html = "<h1>Olá mundo !</h1>";

    int client_socketd = accept(serverCore.socketd, NULL, NULL);

    if (client_socketd < 0) {
        reportarErro("Falha ao aceitar conexão");
        continue;
    }

    ssize_t bytes_lidos = read(
        client_socketd,
        buffer_req,
        sizeof(buffer_req) - 1
    );

    if (bytes_lidos < 0) {
        criarErro(&status, "Falha ao ler requisição");
        goto cleanup;
    }

    if (bytes_lidos == 0) {
        status.has_error = true;
        status.error_code = 0;
        status.message = "Cliente encerrou a conexão sem enviar dados";
        goto cleanup;
    }

    buffer_req[bytes_lidos] = '\0';

    status = creatBufferResponse(
        buffer_res,
        sizeof(buffer_res),
        html
    );

    if (status.has_error)
        goto cleanup;

    size_t total = strlen(buffer_res);
    size_t enviados = 0;

    while (enviados < total) {
      ssize_t bytes = write(
        client_socketd,
        buffer_res + enviados,
        total - enviados
      );

      if (bytes < 0) {
        criarErro(&status, "Falha ao enviar resposta");
        goto cleanup;
      }

      if (bytes == 0) {
        status.has_error = true;
        status.error_code = 0;
        status.message = "Não foi possível enviar toda a resposta";
        goto cleanup;
      }

      enviados += (size_t)bytes;
    }

    status.message = "Requisição atendida";

cleanup:
    if (!limparSocket(client_socketd) && !status.has_error) {
      status.has_error = true;
      status.error_code = errno;
      status.message = "Falha ao fechar socket do cliente";
    }

    if (status.has_error) {
      fprintf(stderr, "%s", status.message);

      if (status.error_code != 0) fprintf(stderr, ": %s", strerror(status.error_code));

      fprintf(stderr, "\n");
    } 
    else printf("%s\n", status.message);
  }
}

void serverClose(void) {
  if (limparSocket(serverCore.socketd))
    serverCore.socketd = -1;

  serverCore.initialized = false;
}
