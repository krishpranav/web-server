//
//  socket.c
//  web-server
//
//  Created by krisna pranav on 24/01/23.
//

#include "socket.h"

#ifndef _WIN32
  #include <errno.h>
#endif

int socket_startup(void) {
#ifdef _WIN32
  WSADATA wsa;
  return WSAStartup(MAKEWORD(2, 2), &wsa);
#else
  return 0;
#endif
}

int socket_cleanup(void)
{
#ifdef _WIN32
  return WSACleanup();
#else
  return 0;
#endif
}

int socket_get_last_error(void)
{
#ifdef _WIN32
  return WSAGetLastError();
#else
  return errno;
#endif
}

int socket_close(socket_t sock)
{
  int status = 0;
  #ifdef _WIN32
    status = shutdown(sock, SD_BOTH);
    status = (!status) ? closesocket(sock) : status;
  #else
    status = shutdown(sock, SHUT_RDWR);
    status = (!status) ? close(sock) : status;
  #endif
  return status;
}

int socket_set_option(socket_t sock, int level, int opt, int value)
{
  int status = 0;
  #ifdef _WIN32
    status = setsockopt(sock, level, opt, (const char *) &value, sizeof(value));
  #else
    status = setsockopt(sock, level, opt, &value, sizeof(value));
  #endif
  return status;
}

int socket_send(socket_t sock, const void *buf, int len, int flags)
{
  char *ptr = (char *) buf;
  int total = 0;
  while (total < len)
  {
    int n;
#ifdef _WIN32
    n = send(sock, ptr, len - total, flags);
#else
    n = (int) send(sock, ptr, len - total, flags);
#endif
    if (!n)
      return total;
    if (n == SOCKET_ERROR)
      return SOCKET_ERROR;
    total += n;
    ptr += n;
  }
  return total;
}

int socket_recv(socket_t sock, void *buf, int len, int flags)
{
  int n;
#ifdef _WIN32
  n = recv(sock, (char *) buf, len, flags);
#else
  n = (int) recv(sock, buf, len, flags);
#endif
  return n;
}
