#ifndef __HLC_OCU_STATE_VEHICLE_H__
#define __HLC_OCU_STATE_VEHICLE_H__

#include "PacketHlcOcu.h"

enum VEHICLETYPE
{
    SELF      = 0,
    WAMV      = 1,
    USV       = 2,
    UUV       = 3,
    UAV       = 4,
    MSV       = 5,
    SONAR_POS = 6,
    SONAR_CMD = 7,
    AIS       = 8
};

class HlcOcuStateVehicle : public PacketHlcOcu
{
private:
   typedef PacketHlcOcu super;

public:
   HlcOcuStateVehicle();

   bool parse( const std::vector<uint8_t> &newData);

   uint16_t length() const { return 2 + 54; }  
   uint16_t type()   const { return 0x0301; }

    // In string 
//    std::vector<vehicleState_x>   m_vehicleList;

   double latitude_decDeg;
   double longitude_decDeg;
   float  heading_deg;
   float  speedOverGround_knots;
   bool   gpsFix;
   float  courseOverGround_deg;
    uint8_t vehicleType; //0 = MSV, 1 = WAM-V, 2 = USV, 3 = UUV, 4 = UAV
    std::string    vehicleName;
    uint32_t vehicleID;
    // end of string

private:
   void buildData();
};

#endif
