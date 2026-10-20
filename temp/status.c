#include "status.h"

void criarErro(Status *status, const char *message) {
  status->has_error = true;
  status->message = (char *) message;
  status->error_code = errno;
}
