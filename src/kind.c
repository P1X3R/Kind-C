#include <kind.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

static volatile bool is_running = true;

void interrupt_handler(int signal);
int setup_server_socket(kind_server_config_t *server_config,
                        struct sockaddr *address, socklen_t address_size);

// TODO: Make this multithread with a thread pool
// TODO: Make this thread safe
// TODO: Make well unit testing for this
void listen_server(kind_server_config_t *server_config) {
  // Setup signal action
  // If the kyeboard interrupts the program, instead of just close the program,
  // clean the data and then end the program
  struct sigaction action;
  action.sa_handler =
      interrupt_handler; // When the keyboard interrupts, execute input_handler
  (void)sigemptyset(&action.sa_mask); // Empty the action mask
  action.sa_flags = 0;                // Not a special flag

  if (-1 == sigaction(SIGINT, &action, NULL)) {
    (void)perror("Error trying handle keyboard interrupting");
    return;
  }

  // Setup server address
  struct sockaddr_in address;
  address.sin_family = AF_INET;                  // Setup TCP address
  address.sin_port = htons(server_config->port); // Set the server port
  address.sin_addr.s_addr = INADDR_ANY;          // Set address to 0.0.0.0
  socklen_t address_size = sizeof(address);

  int server_file_descriptor = setup_server_socket(
      server_config, (struct sockaddr *)&address, address_size);

  // Print some logging
  switch (server_config->logging_type) {
  case no:
    break;
  case basic:
    (void)printf("Server running in port: %d\n", server_config->port);
    break;
  case colorful:
    (void)printf(ANSI_COLOR_MAGENTA "[Server]: ");
    (void)printf(ANSI_COLOR_GREEN "Running at port ");
    (void)printf(ANSI_COLOR_BLUE "%d\n", server_config->port);
    (void)printf(ANSI_COLOR_RESET);
    break;
  }

  int client_file_descriptor = -2;
  while (is_running) {
    // Client socket representation
    client_file_descriptor = accept(server_file_descriptor,
                                    (struct sockaddr *)&address, &address_size);

    if (-1 == client_file_descriptor) {
      (void)perror("Error accepting for connections");
      is_running = false;
    }

    (void)close(client_file_descriptor);
  }

  // Close client and server socket
  (void)close(server_file_descriptor);
  if (-2 != client_file_descriptor)
    (void)close(client_file_descriptor);

  (void)printf("\nBye bye\n");
}

void interrupt_handler(int signal) {
  (void)signal;
  is_running = false;
}

int setup_server_socket(kind_server_config_t *server_config,
                        struct sockaddr *address, socklen_t address_size) {
  // Create the server TCP socket
  int server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == server_file_descriptor) {
    (void)perror("Error creating the server socket");
    return -1;
  }

  if (-1 == bind(server_file_descriptor, address, address_size)) {
    (void)perror("Error binding the address to the server");
    (void)close(server_file_descriptor);
    return -1;
  }

  if (-1 == listen(server_file_descriptor, server_config->backlog)) {
    (void)perror("Error initializing the server listener");
    (void)close(server_file_descriptor);
    return -1;
  }

  return server_file_descriptor;
}
