#pragma once
#include <stdint.h>

typedef enum { no, basic, colorful } logging_type_t;

typedef struct _server_config {
  uint16_t backlog;
  logging_type_t logging_type;
} kind_server_config_t;

typedef struct _header {
  char key[255];
  char value[8192];
} kind_header_t;

typedef struct _response {
  char *body;
  uint16_t status;
  kind_header_t headers[4096];
} kind_response_t;

void add_route(char *route);
void listen_server(kind_server_config_t *server_config, uint16_t port);
