//
//  server.c
//  web-server
//
//  Created by krisna pranav on 24/01/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket.h"

#define IPV4_MAX_LEN 16

#define DEFAULT_PORT    9000
#define MAX_MESSAGE_LEN 1023

int main(int argc, char *argv[])
{
  int port = argc > 1 ? atoi(argv[1]) : 0;
  port = port ? port : DEFAULT_PORT;

    if (socket_startup())
    {
        printf("Socket startup failed: %d\n", socket_get_last_error());
        return EXIT_FAILURE;
    }

  socket_t server;

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket: %d\n", socket_get_last_error());
    socket_cleanup();
    return EXIT_FAILURE;
    }

  if (socket_set_option(server, SOL_SOCKET, SO_REUSEADDR, 1) == SOCKET_ERROR)
  {
    printf("Could not set socket option: %d\n", socket_get_last_error());
    socket_close(server);
    socket_cleanup();
    return EXIT_FAILURE;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        printf("Bind failed: %d\n", socket_get_last_error());
    socket_close(server);
    socket_cleanup();
    return EXIT_FAILURE;
    }

  if (listen(server, SOMAXCONN) == SOCKET_ERROR)
  {
    printf("Listen failed: %d\n", socket_get_last_error());
    socket_close(server);
    socket_cleanup();
    return EXIT_FAILURE;
  }

  printf("Listening on port %d\n", port);

  for (;;)
  {
    socket_t client;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    printf("Waiting for connection...\n");

    if ((client = accept(server, (struct sockaddr *) &client_addr, &client_addr_len)) == INVALID_SOCKET)
    {
      printf("Accept failed: %d\n", socket_get_last_error());
      socket_close(server);
      socket_cleanup();
      return EXIT_FAILURE;
    }

    char ip[IPV4_MAX_LEN + 1];
    memset(ip, 0, sizeof(ip));
    inet_ntop(AF_INET, &client_addr.sin_addr, ip, IPV4_MAX_LEN);
    printf("Accepted connection from %s\n", ip);

    for (;;)
    {
      char buffer[MAX_MESSAGE_LEN + 1];
      int nbytes;

      if ((nbytes = socket_recv(client, buffer, MAX_MESSAGE_LEN, 0)) == SOCKET_ERROR)
      {
        printf("Recv failed: %d\n", socket_get_last_error());
        socket_close(client);
        socket_close(server);
        socket_cleanup();
        return EXIT_FAILURE;
      }

      if (!nbytes)
      {
        printf("Connection closed\n");
        break;
      }

      buffer[nbytes] = '\0';
      printf("Received: %s\n", buffer);

      if ((nbytes = socket_send(client, buffer, nbytes, 0)) == SOCKET_ERROR)
      {
        printf("Send failed: %d\n", socket_get_last_error());
        socket_close(client);
        socket_close(server);
        socket_cleanup();
        return EXIT_FAILURE;
      }
      printf("Sent: %s\n", buffer);
    }

    socket_close(client);
  }

  socket_close(server);
  socket_cleanup();

  return EXIT_SUCCESS;
}
