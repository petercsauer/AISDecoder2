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

std::string serialInterface::clockTask(std::string longString)
{
    AISDecoder *decoder = new AISDecoder();
    
    if(!m_Port.valid()) { return ""; }

    std::string s;
    size_t pos = 0;
    std::string delimiter = "\n";
    std::string token;
    AISData ais;
    while((s = m_Port.receive()) != "")
    {
        //Continually append to string so wraparound is avoided
        longString.append(s);

        //Split every \n until no more are left, and remove each section from the longString
        while ((pos = longString.find(delimiter)) != std::string::npos) {
            token = longString.substr(0, pos);
            //parse separated nmea string into AIS object
            ais = decoder->parseNMEA(token);
            // std::cout<<token<<std::endl;
            longString.erase(0, pos + delimiter.length());

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

                // send the data to the Udp Handeler
                signal_vesselState_info(vehicleState);
            }
        }
        
        
    }
    return longString;
}
