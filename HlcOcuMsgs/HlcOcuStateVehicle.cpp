#include "HlcOcuStateVehicle.h"

HlcOcuStateVehicle::HlcOcuStateVehicle()
   : latitude_decDeg      ( 0.0 )
   , longitude_decDeg     ( 0.0 )
   , heading_deg          ( 0.0 )
   , speedOverGround_knots( 0.0 )
   , gpsFix               ( false )
{
}

bool
HlcOcuStateVehicle::parse( const std::vector<uint8_t> &newData )
{
    if( !super::parse(newData) ) { return false; }

//    vehicleState_x  incommingData;

    latitude_decDeg       = get_double( dataOffset + 0  );
    longitude_decDeg      = get_double( dataOffset + 8  );
    heading_deg           = get_float ( dataOffset + 16 );
    speedOverGround_knots = get_float ( dataOffset + 20 );
    gpsFix                = get_uint8 ( dataOffset + 24 ) == 1 ? true : false;
    courseOverGround_deg  = get_float ( dataOffset + 25 );
    vehicleType          = get_uint8( dataOffset + 29);
    vehicleName          = get_string(dataOffset + 30, 20);
    vehicleID            = get_uint32( dataOffset + 50);
    
    if( heading_deg > 360.0 ) heading_deg = 360.0;
    if( heading_deg < 0.0 )   heading_deg = 0.0;

    if( courseOverGround_deg > 360.0 ) courseOverGround_deg = 360.0;
    if( courseOverGround_deg < 0.0 )   courseOverGround_deg = 0.0;

    //printf("HlcOcuStateVehicle::parse  our name %s, \n", vehicleName.c_str() );
    return true;
}

void
HlcOcuStateVehicle::buildData()
{

   if( heading_deg > 360.0 ) heading_deg = 360.0;
   if( heading_deg < 0.0 )   heading_deg = 0.0;

   if( courseOverGround_deg > 360.0 ) courseOverGround_deg = 360.0;
   if( courseOverGround_deg < 0.0 )   courseOverGround_deg = 0.0;

   add_double( latitude_decDeg );
   add_double( longitude_decDeg  );
   add_float ( heading_deg );
   add_float ( speedOverGround_knots );
   add_uint8 ( gpsFix ? 1 : 0 );
   add_float ( courseOverGround_deg );
   add_uint8 ( vehicleType);
    // Make sure it is 20 char, no less, no more
    while(vehicleName.length() < 20) { vehicleName += " "; }
    if(vehicleName.length() > 20) { vehicleName.resize(20); }
    add_string (vehicleName);
    add_uint32( vehicleID );

    /*
    printf("HlcOcuStateVehicle::buildData");
    printf(": lat, %f, long, %f", latitude_decDeg, longitude_decDeg);
    printf(", heading_deg, %f", heading_deg);
    printf(", speedOverGround_knots, %f", speedOverGround_knots);
    printf(", gpsFix, %i", gpsFix);
    printf(", courseOverGround_deg, %f", courseOverGround_deg);
    printf(", vehicleName = %s len = %i\n", vehicleName.c_str(), vehicleName.length());
     */
}
