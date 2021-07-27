#ifndef __SOCKET_UDP_CLIENT_H
#define __SOCKET_UDP_CLIENT_H

#include <stdint.h>
#include <string>
using std::string;

class SocketUdpClient
{
   public:
      SocketUdpClient(
            const string &remoteIp, 
            uint16_t remotePort, 
            uint16_t localPort = 0);

      bool init(
            const string &remoteIp, 
            uint16_t remotePort, 
            uint16_t localPort = 0);
      int32_t send(const string &data, int32_t flags = 0) const;
      int32_t receive(string &data,    int32_t flags = 0) const;

      void close();

   private:
      // No copying allowed
      SocketUdpClient(const SocketUdpClient &);
      SocketUdpClient& operator=(const SocketUdpClient &);

      int m_sockfd;
      bool m_isOpen;
};

#endif
