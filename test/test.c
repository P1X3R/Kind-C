#include <kind.h>

int main(void) {
  kind_server_config_t server_config;
  server_config.logging_type = colorful;
  server_config.backlog = 10;

  listen_server(&server_config, 3000);

  return 0;
}
