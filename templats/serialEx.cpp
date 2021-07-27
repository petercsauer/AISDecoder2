/*
 * Serial Read using MAR lib
 * J Mehlman 7/21/21
 */

#include "serialEx.h"
#include "decoder.h"

static std::string stringAsHex(const std::string & s)
{
   std::vector<std::string> strs;

   BOOST_FOREACH(uint8_t c, s) {
      strs.push_back(boost::str(boost::format("%02x") % (uint32_t)c));
   }

   return boost::algorithm::join(strs, " ");
}

serialInterface::serialInterface(const std::string &portName)
    : m_Port(portName, B38400)
{
}

void serialInterface::clockTask()
{
    AISDecoder *decoder = new AISDecoder();
    
    if(!m_Port.valid()) { return; }

    std::string s;
    std::string longString = "";
    std::vector<std::string> nmeaStrings;
    size_t pos = 0;
    std::string delimiter = "\n";
    std::string token;
    AISData ais;
    while((s = m_Port.receive()) != "")
    {
        longString.append(s);
        
        while ((pos = longString.find(delimiter)) != std::string::npos) {
            token = longString.substr(0, pos);
            ais = decoder->parseNMEA(token);
            longString.erase(0, pos + delimiter.length());
        }

        // printf("serial string: %s\n", s.c_str());
        // printf("serial string: %s\n", stringAsHex(s).c_str());
        
        // send to the parser
        if (ais.type != -1){
            // update the state
            vehicleState.latitude_decDeg = ais.latitude;
            vehicleState.longitude_decDeg = ais.longitude;
            vehicleState.heading_deg = ais.hdg;     // float
            vehicleState.speedOverGround_knots = ais.sog; // float
            vehicleState.gpsFix = false; // bool
            vehicleState.courseOverGround_deg = ais.cog;// float
            vehicleState.vehicleType = ais.type; // move to VEHICLETYPE
            vehicleState.vehicleName = "ais input"; // 20 char string, will pad
            vehicleState.vehicleID = ais.mmsi; // uint32_t, unique id (has name?)

            // something will be needed to make sure we don't flood our systems with too much

            // send the data to the Udp Handeler
            signal_vesselState_info(vehicleState);
        }
    }
}
