/*
 * Generate and send "follow" sting
 * J Mehlman 7/21/21
 */

#ifndef __UDP_EX
#define __UDP_EX

#include <stdio.h>

#include "../legos/Socket/SocketUdpClient.h"

// The packets
#include "../HlcOcuMsgs/PacketHlcOcu.h" // Supper
#include "../HlcOcuMsgs/HlcOcuStateVehicle.h" // one of many


class udpEx
{
    public:
        udpEx(const std::string &ipAddress, uint16_t port);

        // data slots to here
        void slot_vesselState_info(HlcOcuStateVehicle thisVehicle);

    private:
        SocketUdpClient m_loc_socket;
};

#endif
