#ifndef __SERIAL_PORT_H
#define __SERIAL_PORT_H

/*
 * SerialPort.h
 *
 * Interface for a non-blocking serial port.
 *
 * see:
 * http://en.wikibooks.org/wiki/Serial_Programming/Serial_Linux
 * http://tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html
 * http://linux.die.net/man/3/termios
 */

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <string>


class SerialPort
{
   public:
      SerialPort(const std::string &portName, speed_t speed);
      ~SerialPort();

      bool valid() const;

      int send(const std::string &data) const;
      std::string receive() const;

   private:
      bool m_valid;

      int m_fd;
      std::string m_portName;

      termios m_oldtio;
};

#endif
