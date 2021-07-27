/*
 * SerialPort.cpp
 *
 * Implementation for the non-blocking serial port.
 *
 */

#include "SerialPort.h"
#include <stdio.h>
#include <errno.h>
#include <string.h> //memset

using std::string;

SerialPort::SerialPort(const string &portName, speed_t speed)
{
   m_valid = false;

    if(portName.length() > 0) // Don't open the port if it's null
    {
        printf("SerialPort::SerialPort init port=%s, speed=%i\n", portName.c_str(), speed);

        m_portName = portName;
        if ( (m_fd = open(portName.c_str(), O_RDWR | O_NONBLOCK)) == -1) {
            perror("SerialPort: open");
            m_valid = false;
        }
        else {
            m_valid = true;
        }

       if (valid()) {
          printf("setting up serial port now\n");
          termios tio;
          memset(&tio, 0, sizeof(tio));
          tio.c_iflag = IGNPAR;
          tio.c_oflag = 0;
          tio.c_cflag = CS8 | CREAD | CLOCAL;
          tio.c_lflag = 0;
          tio.c_cc[VMIN]  = 0;
          tio.c_cc[VTIME] = 0;
          cfsetospeed(&tio, speed);
          cfsetispeed(&tio, 0);
    
          tcsetattr(m_fd, TCSANOW, &tio);
          tcflush(m_fd, TCIOFLUSH);
       }
    }
}
      
SerialPort::~SerialPort()
{
   if (valid()) { close(m_fd); }
}

bool SerialPort::valid() const
{
   return m_valid;
}

int SerialPort::send(const string &data) const
{
   int status = write(m_fd, data.data(), data.size());

   if (status == -1) {
//      perror("SerialPort: write");
   }

   //printf("status: %d\n", status);
   return status;
}

string SerialPort::receive() const
{
   const size_t BUFSIZE = 255;
   char buf[BUFSIZE];

   ssize_t count = read(m_fd, buf, BUFSIZE);

   if (count == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
         return "";
      }
      else {
//         perror("SerialPort: read");
         return "";
      }
   }

   return string (buf, count);
}

