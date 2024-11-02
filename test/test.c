#include <kind.h>

int main(void) {
  kind_server_config_t server_config;
  server_config.port = 3000;
  server_config.logging_type = colorful;

  listen_server(&server_config);

  return 0;
}
