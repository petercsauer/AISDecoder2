/*
 * Serial Read using MAR lib
 * J Mehlman 7/21/21
 */

#ifndef __SERIAL_EX
#define __SERIAL_EX

#include <stdio.h>
#include <string>
#include <vector>

// For the bin to hex conversion
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>

// for the signal handoff
#include <boost/signals2/signal.hpp>

#include "../legos/Serial/SerialPort.h"

// The packets
#include "../HlcOcuMsgs/PacketHlcOcu.h" // Supper
#include "../HlcOcuMsgs/HlcOcuStateVehicle.h" // one of many


class serialInterface
{
    public:
        serialInterface(const std::string &portName);
        std::string clockTask(std::string longString);

        // signals to send out, connections in main
        typedef boost::signals2::signal <void(HlcOcuStateVehicle)> stateVehicle_signal_t;
        stateVehicle_signal_t signal_vesselState_info;

    private:
        SerialPort m_Port;
        HlcOcuStateVehicle  vehicleState;
};

#endif
