#include <bitset>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "decoder.h"


//FUNCTION: calculates the checksum of a NMEA string
//INPUT: The full NMEA string excluding the $ and chars after *
//OUTPUT: The checksum
int checksum(const char *s) {
    int c = 0;

    while (*s)
        c ^= *s++;

    return c;
}


//FUNCTION: Verifies the checksum of a full NMEA string
//INPUT: The full NMEA string
//OUTPUT: a boolean value (true if correct, false if corrupted)
bool AISDecoder::verifyChecksum(std::string line){
    std::vector<std::string> parts {};
    size_t pos = 0;
    std::string delimiter = "*";
    std::string token;
    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        parts.push_back(token);
        line.erase(0, pos + delimiter.length());
    }
    parts[0].erase(0, 1);
    std::istringstream converter(line);
    unsigned int value;
    converter >> std::hex >> value;
    bool verify = true;
    if (value!=checksum(parts[0].c_str())){
        verify = false;
    }
    return verify;
}


//FUNCTION: Counts the number of digits in a number (useful for adding leading 0's in MMSI)
//INPUT: number to check
//OUTPUT: number of digits
int AISDecoder::numDigits(int number)
{
    int digits = 0;
    if (number < 0) digits = 1; 
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}


//FUNCTION: Finds the two's complement of a binary string
//INPUT: Binary string
//OUTPUT: Binary string that is a twos complement of the input
std::string AISDecoder::findTwosComplement(std::string str)
{
    int n = str.length();

    int i;
    for (i = n-1 ; i >= 0 ; i--)
        if (str[i] == '1')
            break;

    if (i == -1)
        return '1' + str;

    for (int k = i-1 ; k >= 0; k--)
    {
        if (str[k] == '1')
            str[k] = '0';
        else
            str[k] = '1';
    }

    return str;
}

//FUNCTION: converts binary string to decimal number
//INPUT: binary string
//OUTPUT: decimal number
int AISDecoder::binaryStringToDecimal(std::string a)
{
    printf("Length: %i\n", a.length());
    long long num = 0;
    long long counter = 1;
    int j = 0;
    for (int i = 1; i<a.length();i++){
        if(a[i]=='1'){
            num += pow(2,a.length()-2-i);
            std::cout<< num << std::endl;
        }
    }
    if (a[0]=='1'){
        num = -num;
    }
    return num;
}

//FUNCTION: converts decimal number to binary string
//INPUT: decimal number
//OUTPUT: binary string
std::string AISDecoder::toBinary(int n)
{
    std::string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    
    int diff = 6-r.length();
    for (int i=0; i<diff;i++){
        r.insert(0,"0");
    }
    return r;
}

//FUNCTION: converts a binary string to AIS message type
//INPUT: The portion of the full AIS payload that is the type section
//OUTPUT: AIS message type
int AISDecoder::toType(std::string binary){
    float type = std::stoi(binary,0,2);
    return type;
}

//FUNCTION: converts a binary string to latitude or longitude
//INPUT: The portion of the full AIS payload that is the latitude or longitude section
//OUTPUT: The signed latitude or longitude coordinate
float AISDecoder::toLatLong(std::string binary){
    int p_n = 1;
    if (binary[0]=='1'){
        binary = findTwosComplement(binary);
        p_n = -1;
    }
    float coords = (std::stol(binary,0,2));
    coords = coords / 600000 * p_n;
    return coords;
}

//FUNCTION: converts a binary string to the navigation status
//INPUT: The portion of the full AIS payload that is the navigation status section
//OUTPUT: The navigation status descriptor
std::string AISDecoder::toNavStatus(std::string binary){
    int status = std::stol(binary,0,2);
    std::vector<std::string> status_types {
        "Under way using engine",
        "At anchor",
        "Not under command",
        "Restricted maneuverability",
        "Constrained by her draught",
        "Moored",
        "Aground",
        "Engaged in Fishing",
        "Under way sailing",
        "Status not currently in use",
        "Status not currently in use",
        "Status not currently in use",
        "Status not currently in use",
        "Status not currently in use",
        "AIS-SART is active",
        "default"
    };
    return status_types[status];
}

//FUNCTION: converts a binary string to a float
//INPUT: A portion of the full AIS payload
//OUTPUT: The float value of the input binary string
float AISDecoder::toFloat(std::string binary){
    return std::stoi(binary,0,2);
}

//FUNCTION: converts a binary string to ROT (rotational speed)
//INPUT: The portion of the full AIS payload that is the ROT section
//OUTPUT: The rotational speed of the vessel
float AISDecoder::toROT(std::string binary){
    int p_n = 1;
    if (binary[0]=='1'){
        binary = findTwosComplement(binary);
        p_n = -1;
    }
    float rot = std::stoi(binary,0,2);
    rot = p_n*pow(rot/4.733,2);
    return rot;
}

//FUNCTION: converts a binary string to a long
//INPUT: A portion of the full AIS payload
//OUTPUT: The long value of the input binary string
long AISDecoder::toLong(std::string binary){
    return std::stol(binary,0,2);
}

//FUNCTION: converts the full AIS payload to AISData objects
//INFO: can handle messages of type 1, 2, 3, 4, 18 and 27
//INPUT: the full AIS payload
//OUTPUT: an AIS struct
AISData AISDecoder::aisPayloadHandler(std::string unparsed){

    AISData ais;
    int dec = 0;
    std::string binary = "";

    // if(!verifyChecksum(unparsed)){
    //     std::cout << ("CORRUPTED STRING") << std::endl;
    // }
    // else{
    //     std::cout <<"ALL GOOD"<<std::endl;
    // }

    for (int i = 0; i<unparsed.length(); i++){
        dec = int(unparsed[i]);
        dec = dec-48;
        if (dec > 40){
            dec = dec - 8;
        }
        binary.append(toBinary(dec));
    }

    ais.type = toFloat(binary.substr(0,6));

    //Vessel Type A Location
    if (ais.type == 1 || ais.type == 2 || ais.type == 3){ 

        ais.longitude = toLatLong(binary.substr(61,28));
        ais.latitude = toLatLong(binary.substr(89,27));
        ais.navStatus = toNavStatus(binary.substr(38,4));
        ais.cog = toFloat(binary.substr(116,12))/10;
        ais.sog = toFloat(binary.substr(50,10))/10;
        ais.hdg = toFloat(binary.substr(128,9));
        ais.rot = toROT(binary.substr(38,4));
        ais.mmsi = toLong(binary.substr(8,30));
        ais.timestamp = toLong(binary.substr(137,6));

    }

    //Base Station Report
    if (ais.type == 4){ 

        ais.longitude = toLatLong(binary.substr(79,28));
        ais.latitude = toLatLong(binary.substr(107,27));
        ais.mmsi = toLong(binary.substr(8,30));

    }

    //Vessel Type B Location
    if (ais.type == 18){ 

        ais.longitude = toLatLong(binary.substr(57,28));
        ais.latitude = toLatLong(binary.substr(85,27));
        ais.cog = toFloat(binary.substr(116,12))/10;
        ais.sog = toFloat(binary.substr(50,10))/10;
        ais.hdg = toFloat(binary.substr(128,9));
        ais.mmsi = toLong(binary.substr(8,30));
        ais.mmsi = toLong(binary.substr(8,30));

    }

    //Long Range AIS Broadcast
    if (ais.type == 27){ 

        ais.longitude = toLatLong(binary.substr(44,18));
        ais.latitude = toLatLong(binary.substr(62,17));
        ais.mmsi = toLong(binary.substr(8,30));

    }
    return ais;
}

//FUNCTION: Takes an AIS message string from serial and splits it into its sub parts
//INPUT: A line from serial
//OUTPUT: An AISData object
AISData AISDecoder::parseNMEA(std::string string){
    std::vector<std::string> parts {};
    size_t pos = 0;
    std::string delimiter = ",";
    std::string token;
    while ((pos = string.find(delimiter)) != std::string::npos) {
        token = string.substr(0, pos);
        parts.push_back(token);
        string.erase(0, pos + delimiter.length());
    }
    if (parts.size() > 0 && parts.at(0) == "!AIVDM"){
        return aisPayloadHandler(parts[5]);
    }
    else{
        AISData ais;
        return ais;
    }
}



//FUNCTION: reads a .txt file with AIS data and creates a CSV with the program output
void AISDecoder::readTXT(){
    std::vector<AISData> aisData {};
    std::string line;
    std::ifstream myfile ("aisExampleOut.txt");
    if (myfile.is_open())
    {

        while(getline(myfile, line)) {
            if (line.substr(0,6)=="!AIVDM"){
                if(!verifyChecksum(line)){
                    std::cout << ("CORRUPTED STRING") << std::endl;
                }
                else{
                    std::cout <<"ALL GOOD"<<std::endl;
                }
                std::vector<std::string> parts {};
                size_t pos = 0;
                std::string delimiter = ",";
                std::string token;
                while ((pos = line.find(delimiter)) != std::string::npos) {
                    token = line.substr(0, pos);
                    parts.push_back(token);
                    line.erase(0, pos + delimiter.length());
                }
                aisData.push_back(aisPayloadHandler(parts[5]));
            }
        }
        myfile.close();

        std::ofstream myfile;
        myfile.open ("AISOutput.csv");
        myfile << "AIS TYPE, TIMESTAMP, MMSI, LATITUDE, LONGITUDE, SOG, COG, HDG, ROT, NAV STATUS\n";
        for (auto ais : aisData){
            if (ais.mmsi !=-1){
                int len = numDigits(ais.mmsi);
                myfile << ais.type << "," << ais.timestamp << ",";
                for (int j = 0; j<9-len;j++){
                    myfile << "0";
                }
                myfile << ais.mmsi << "," << ais.latitude << "," << ais.longitude << "," << ais.sog << "," << ais.cog << "," << ais.hdg << "," << ais.rot << "," << ais.navStatus << "\n";
            }
        }
        myfile.close();
    }

    else std::cout << "Unable to open file"; 
}
