#pragma once
#include <stdint.h>

typedef enum { no, basic, colorful } logging_type_t;

typedef struct {
  uint16_t port;
  logging_type_t logging_type;
} kind_server_config_t;

void listen_server(kind_server_config_t server_config);
