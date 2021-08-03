/*
 * Serial Read using MAR lib
 * J Mehlman 7/21/21
 */

#include <stdio.h>
#include <iostream>
#include "serialEx.h"
#include "udpEx.h"

int main(int argc, char **argv)
{
    if(argc != 1)
    {
        printf("This program takes no arguments\n");
        return 1;
    }

    // the connection to the AIS
    // the port is actualy in a config file
    serialInterface *AIS;
    AIS = new serialInterface("/dev/ttyUSB0");

    // the connection to our broadcast
    // we might want to make this direct to our operator station
    // but for now we assume that we use our swarm connection for AIS
    // the ip and port are actualy in a config file
    udpEx mapServer("192.168.168.255", 5016); 


    // Make connections via boost/signals2
    AIS->signal_vesselState_info.connect(
               boost::bind(&udpEx::slot_vesselState_info, &mapServer, _1));
    
    std::string longString = "";
    while(true)
    {
        // the scheduler goes here, but just sleep for now
        usleep(2000); // run at 20Hz

        longString = AIS->clockTask(longString);
        // std::cout<<"NEW ----------\n"<<longString<<std::endl;

        //mapServer.clockTask();
    }
    return 0;
}
