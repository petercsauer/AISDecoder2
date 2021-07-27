/*
 * Generate and send "follow" sting
 * J Mehlman 7/21/21
 */

#include "udpEx.h"

udpEx::udpEx(const std::string &ipAddress, uint16_t port)
    : m_loc_socket(ipAddress, port, port) // ip, local port, remote port
{
}

void udpEx::slot_vesselState_info(HlcOcuStateVehicle thisVehicle)
{
    printf("udpEx::slot_vesselState_info");
    printf(", Type(int) = %i", thisVehicle.vehicleType);
    printf(", Name  %s", thisVehicle.vehicleName.c_str());
    printf(", ID(int) = %i", thisVehicle.vehicleID);
    printf(", gpsFix(t:f) = %i", thisVehicle.gpsFix);
    printf(", lat(deg) = %0.5f", thisVehicle.latitude_decDeg);
    printf(", lon(deg) = %0.5f", thisVehicle.longitude_decDeg);
    printf(", heading(deg) = %0.1f", thisVehicle.heading_deg);
    printf(", speed(kts) = %0.1f", thisVehicle.speedOverGround_knots);
    printf(", course(deg) = %0.1f", thisVehicle.courseOverGround_deg);

    printf("\n");

    thisVehicle.build();
    m_loc_socket.send(thisVehicle.string());
}
