#include "network.hpp"

//#include <winsock2.h>
//#include <ws2tcpip.h>

__MYLO_NAMESPACE_BEGIN

#if defined POSIX

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#define SOCKET int
#define INVALID_SOCKET 0

#define TCP_NODELAY 0x0001

#else

#include <winsock2.h>
#include <ws2tcpip.h>
#include <map>

#pragma comment (lib, "Ws2_32.lib")

#endif

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881"

#define MAX_PACKET_SIZE 86400

network::network() {
  m_send = INVALID_SOCKET;
  m_receive = INVALID_SOCKET;
  m_status = -1;
}

int network::listen(char *port) {

#if !defined POSIX
  WSADATA wsaData;

  m_status = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (m_status != 0) {
    printf("failed with error\n");
    exit(1);
  }
#endif

  m_receive = socket(AF_INET, SOCK_STREAM, 0);

  if (m_receive == INVALID_SOCKET) {
    printf("socket failed with error\n");
    //WSACleanup();
    exit(1);
  }

#if !defined POSIX
  u_long iMode = 1;
  m_status = ioctlsocket(m_receive, FIONBIO, &iMode);

  if (m_status == SOCKET_ERROR) {
    printf("ioctlsocket failed with error");
    closesocket(m_receive);
    //WSACleanup();
    exit(1);
  }
#endif

  struct sockaddr_in server;
  //struct sockaddr_in client;
  int address_length;

  char *end_ptr;
  short sport = strtol(port, &end_ptr, 10);

  server.sin_family = AF_INET;
  server.sin_port = htons(sport);
  server.sin_addr.s_addr = INADDR_ANY;
  memset(&(server.sin_zero), 8, 0);
  address_length = sizeof(struct sockaddr);

  m_status = ::bind(m_receive, (struct sockaddr *)&server, address_length);

  //  if (m_status == SOCKET_ERROR) {
  //printf("bind failed with error: %d\n", WSAGetLastError());
  //   closesocket(m_receive);
  //WSACleanup();
  //    exit(1);
  //  }

  m_status = ::listen(m_receive, SOMAXCONN); // BACKLOG

  //  if (m_status == SOCKET_ERROR) {
  //printf("listen failed with error: %d\n", WSAGetLastError());
  //    closesocket(m_receive);
  //WSACleanup();
  //    exit(1);
  //  }

  SOCKET client = accept(m_receive, (sockaddr *)NULL, (socklen_t *)NULL);

  if (client != INVALID_SOCKET) {
    char value = 1;
    setsockopt(client, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
    return true;
  }

  return false;
}

int network::connect(char *address, char *port) {
  m_send = INVALID_SOCKET;

  //  struct addrinfo *result = NULL;
  //  struct addrinfo *ptr = NULL;
  //  struct addrinfo hints;

#if !defined POSIX
  WSADATA wsaData;

  m_status = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (m_status != 0) {
    printf("WSAStartup failed with error: %d\n", m_status);
    exit(1);
  }
#endif

  struct sockaddr_in server_addr;

  //int socket_fd = 0;

  char *end_ptr;
  short sport = strtol(port, &end_ptr, 10);
  if (address[0] > '9') {
    char ip_addr[1024] = "";
    struct hostent *resolv;

    printf("  - Resolving IP address\n");
    fflush(stdout);

    resolv = gethostbyname(address);
    if (!resolv) {
      return(-1);
    }

    m_send = socket((*resolv).h_addrtype, SOCK_STREAM, 0);
    if (m_send < 0) {
      return(-1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = (*resolv).h_addrtype;
    server_addr.sin_port = htons(sport);

    if (inet_ntop((*resolv).h_addrtype, (*resolv).h_addr_list[0], ip_addr, 1024) <= 0) {
      return(-1);
    }

    printf("  - Host: %s", (*resolv).h_name);
    printf("(%s)", ip_addr);

    printf(" / ");
    switch ((*resolv).h_addrtype) {
    case(AF_INET) : printf("AF_INET");  break;
      //    case(AF_INET6): printf("AF_INET6"); break;
    default:        printf("UNKNOWN");  break;
    };
    printf("\n");

    if (inet_pton((*resolv).h_addrtype, ip_addr, &server_addr.sin_addr) <= 0) {
      return(-1);
    }
  }
  else {
    printf("  - IP provided, just converting to network byte order.\n");
    fflush(stdout);

    m_send = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(sport);

    if (inet_pton(AF_INET, address, &server_addr.sin_addr) <= 0) {
      return(-1);
    }
  }

  if (::connect(m_send, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    printf("  - failed to open the TCP socket.\n");
    return(-1);
  }
  //  if (m_send == INVALID_SOCKET) {
  //    printf("Unable to connect to server!\n");
  //    WSACleanup();
  //    exit(1);
  //  }

  u_long iMode = 1;

#if defined POSIX
  int flags;
  if (-1 == (flags = fcntl(m_send, F_GETFL, 0)))
    flags = 0;

  fcntl(m_send, F_SETFL, flags | O_NONBLOCK);
#else
  m_status = ioctlsocket(m_send, FIONBIO, &iMode);
  if (m_status == SOCKET_ERROR)
  {
    printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
    closesocket(m_send);
    WSACleanup();
    exit(1);
  }

  char value = 1;
  setsockopt(m_send, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
#endif
}

int network::send(char *buffer, int size)	{
  return ::send(m_send, buffer, size, 0);
}
int network::receive(char *buffer, int size) {
  return ::recv(m_send, buffer, size, 0);
}


__MYLO_NAMESPACE_END

