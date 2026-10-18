#include "response.h"

Status creatBufferResponse(
    char *buffer,
    size_t tamanho,
    const char *html
) {
  Status status = {
    .has_error = false,
    .error_code = 0,
    .message = "Buffer criado"
  };

  int res = snprintf(
    buffer,
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

void responseSend(Response *self, const char *html) {
  char buffer[SIZE_BUFFER];

  self->status = creatBufferResponse(
      buffer,
      sizeof(buffer),
      html
  );

  if (self->status.has_error) return;

  size_t total = strlen(buffer);
  size_t enviados = 0;

  while (enviados < total) {
    ssize_t bytes = write(
      self->client_socketd,
      buffer + enviados,
      total - enviados
    );

    if (bytes < 0) {
      criarErro(&self->status, "Falha ao enviar resposta");
      return;
    }

    if (bytes == 0) {
      self->status.has_error = true;
      self->status.error_code = 0;
      self->status.message = "Não foi possível enviar toda a resposta";
      return;
    }

    enviados += (size_t)bytes;
  }
}

Response initResponse(int client_socketd) {
  return (Response) {
    .client_socketd = client_socketd,
    .send = responseSend
  };
}
