#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <stdbool.h>

bool wbs_listener(int port, int (*callback)(void));

#endif
