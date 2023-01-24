//
//  client.c
//  web-server
//
//  Created by krisna pranav on 24/01/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "utils.h"

#define DEFAULT_HOST    "127.0.0.1"
#define DEFAULT_PORT    9000
#define DEFAULT_MESSAGE "Hi!"
#define MAX_MESSAGE_LEN 1023

int main(int argc, char *argv[]) {
  char *host = argc > 1 ? argv[1] : DEFAULT_HOST;
  int port = argc > 2 ? atoi(argv[2]) : 0;
  port = port ? port : DEFAULT_PORT;
  char *message = argc > 3 ? argv[3] : DEFAULT_MESSAGE;

  if (socket_startup())
  {
    printf("Socket startup failed: %d\n", socket_get_last_error());
    return EXIT_FAILURE;
  }

  socket_t client;

  if ((client = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
  {
    printf("Could not create socket: %d\n", socket_get_last_error());
    socket_cleanup();
    return EXIT_FAILURE;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, host, &server_addr.sin_addr) != 1)
  {
    printf("Could not convert host to IP: %d\n", socket_get_last_error());
    socket_close(client);
    socket_cleanup();
    return EXIT_FAILURE;
  }

  if (connect(client, (struct sockaddr *) &server_addr, sizeof(server_addr)) == SOCKET_ERROR)
  {
    printf("Connect failed: %d\n", socket_get_last_error());
    socket_close(client);
    socket_cleanup();
    return EXIT_FAILURE;
  }

  printf("Connected to %s:%d\n", host, port);

  char buffer[MAX_MESSAGE_LEN + 1];
  int nbytes;

  string_copy(buffer, message, MAX_MESSAGE_LEN);
  nbytes = (int) strlen(buffer);

  if ((nbytes = socket_send(client, buffer, nbytes, 0)) == SOCKET_ERROR)
  {
    printf("Send failed: %d\n", socket_get_last_error());
    socket_close(client);
    socket_cleanup();
    return EXIT_FAILURE;
  }
  printf("Sent: %s\n", buffer);

  if ((nbytes = socket_recv(client, buffer, MAX_MESSAGE_LEN, 0)) == SOCKET_ERROR)
  {
    printf("Recv failed: %d\n", socket_get_last_error());
    socket_close(client);
    socket_cleanup();
    return EXIT_FAILURE;
  }
  buffer[nbytes] = '\0';
  printf("Received: %s\n", buffer);

  socket_close(client);
  socket_cleanup();

  printf("Done.\n");
  return EXIT_SUCCESS;
}
