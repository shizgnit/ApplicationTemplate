#include "mylo.hpp"

#ifndef __NETWORK_HPP
#define __NETWORK_HPP

__MYLO_NAMESPACE_BEGIN

class network {
public:
  network();

  int connect(char *host, char *port);

  int listen(char *port);

  int send(char *buffer, int size);
  int receive(char *buffer, int size);

protected:
  //  addrinfo m_addrinfo;
  sockaddr m_sockaddr;
  int m_addrlen;

  int m_status;

  SOCKET m_send;
  SOCKET m_receive;
};

__MYLO_NAMESPACE_END

#endif

// Local Variables:
// mode:C++
// End:
