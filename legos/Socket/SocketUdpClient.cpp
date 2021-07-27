#include "SocketUdpClient.h"

/*
** SocketUdpClient.c -- a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>

SocketUdpClient::SocketUdpClient(
      const string &remoteIp, 
      uint16_t remotePort, 
      uint16_t localPort)
{
   m_isOpen = false;
    printf("SocketUdpClient::SocketUdpClient init IP=%s remotPort=%i localPort=%i\n", remoteIp.c_str(), remotePort, localPort);
   init(remoteIp, remotePort, localPort);
}

bool SocketUdpClient::init(
      const string &remoteIp, 
      uint16_t remotePort, 
      uint16_t localPort)
{
   if (m_isOpen) close();

   char remotePortStr[10];
   sprintf(remotePortStr, "%d", remotePort);
   char localPortStr[10];
   sprintf(localPortStr, "%d", localPort);

   struct addrinfo remoteHints, localHints; 
   struct addrinfo *remoteInfo, *localInfo, *p, *p1;
   int rv;

   memset(&remoteHints, 0, sizeof remoteHints);
   remoteHints.ai_family = AF_UNSPEC;
   remoteHints.ai_socktype = SOCK_DGRAM;

   if ((rv = getaddrinfo(remoteIp.c_str(), remotePortStr, &remoteHints, &remoteInfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return false;
   }
   
   if (localPort != 0) {
      memset(&localHints, 0, sizeof localHints);
      localHints.ai_family = AF_UNSPEC;
      localHints.ai_socktype = SOCK_DGRAM;
      localHints.ai_flags = AI_PASSIVE;

      if ((rv = getaddrinfo(NULL, localPortStr, &localHints, &localInfo)) != 0) {
         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
         return false;
      }
   }

   // loop through all the results and make a socket
   for(p = remoteInfo; p != NULL; p = p->ai_next) {
      if ((m_sockfd = socket(p->ai_family, p->ai_socktype,
                  p->ai_protocol)) == -1) {
         perror("SocketUdpClient: socket");
         continue;
      }
      
        // Enable broadcast
        int yes = 1;
        if (setsockopt(m_sockfd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int)) == -1) 
        {
            perror("SocketUdpClient: setsockopt");
            exit(1);
        }

      if (localPort != 0) {
         for (p1 = localInfo; p1 != NULL; p1 = p1->ai_next) {
            if (bind(m_sockfd, p1->ai_addr, p1->ai_addrlen) == -1) {
               perror("SocketUdpClient: bind");
               continue;
            }

            break;
         }
      }

      if (connect(m_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
         ::close(m_sockfd);
         perror("SocketUdpClient: connect");
         continue;
      }

      break;
   }

   fcntl(m_sockfd, F_SETFL, O_NONBLOCK);

   if (p == NULL) {
      fprintf(stderr, "SocketUdpClient: failed to bind socket\n");
      return false;
   }

    freeaddrinfo(remoteInfo);

    m_isOpen = true;

    return true;
}

int32_t SocketUdpClient::send(const string &data, int32_t flags) const
{

//   if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
//               p->ai_addr, p->ai_addrlen)) == -1) {
//      perror("SocketUdpClient: sendto");
//      exit(1);
//   }
   return ::send(m_sockfd, data.data(), data.size(), flags);
}

int32_t SocketUdpClient::receive(string &data, int32_t flags) const
{
   data = "";

   char buf[1500];
   int nBytes = ::recv(m_sockfd, buf, 1500, flags);

   if (nBytes == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
         return 0;
      }
      else {
   //      perror("SocketUdpClient: recv");
         return 0;
      }
   }


   data = string(buf, nBytes);

   return nBytes;
}

void SocketUdpClient::close()
{
   ::close(m_sockfd);
   m_isOpen = false;
}

#ifdef UNIT_TEST_SOCKETUDPCLIENT
int main(int argc, char **argv) {
   SocketUdpClient client("127.0.0.1", 5004, 4444);

   for (int i=0; i<100; i++) {
      char buf[100];
      sprintf(buf, "is this working %d\n", i);
      client.send(string(buf));
   }

   while (1) {
      usleep(10000);
      string data;
      client.receive(data);
      if (data != "") {
         printf("got: %s\n", data.c_str());
      }
   }

   client.close();

   return 0;
}
#endif
