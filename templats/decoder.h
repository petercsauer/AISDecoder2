/*
 * Serial Read using MAR lib
 * J Mehlman 7/21/21
 */

#ifndef __DECODER
#define __DECODER

#include <bitset>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

//STRUCTURE: AIS Data Structure
struct AISData{
    int type = -1;
    int hdg = -1;
    long mmsi = -1;
    double rot = -1;
    double latitude = -1;
    double longitude = -1;
    double timestamp = -1;
    double cog = -1;
    double sog = -1;
    std::string navStatus = "";
};

//CLASS: AIS Decoder
//FUNCTION: Can read line by line from a serial string or text file and returns an AISData object.
class AISDecoder
{
    public:
        bool verifyChecksum(std::string line);
        int numDigits(int number);
        std::string findTwosComplement(std::string str);
        int binaryStringToDecimal(std::string a);
        std::string toBinary(int n);
        int toType(std::string binary);
        float toLatLong(std::string binary);
        std::string toNavStatus(std::string binary);
        float toFloat(std::string binary);
        float toROT(std::string binary);
        long toLong(std::string binary);
        AISData aisPayloadHandler(std::string unparsed);
        AISData parseNMEA(std::string string);
        void readTXT();
};

#endif
