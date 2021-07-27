#include "PacketLittleEndian.h"
// Windows runs on x86 architecture which is assumed to always be native little endian
// Linux has the ability to convert values between host and little endian
// Floats and doubles are handled the same regardless
#ifndef _WIN32
#include <endian.h>
#endif

PacketLittleEndian::PacketLittleEndian()
{
}

void
PacketLittleEndian::add_uint16( uint16_t value )
{
#ifndef _WIN32
   value = htole16( value );
#endif

   uint8_t *pValue = (uint8_t*)&value;

   add_uint8( pValue[0] );
   add_uint8( pValue[1] );
}

void
PacketLittleEndian::add_uint32( uint32_t value )
{
#ifndef _WIN32
   value = htole32( value );
#endif

   uint8_t *pValue = (uint8_t*)&value;

   add_uint8( pValue[0] );
   add_uint8( pValue[1] );
   add_uint8( pValue[2] );
   add_uint8( pValue[3] );
}

void
PacketLittleEndian::add_uint64( uint64_t value )
{
#ifndef _WIN32
   value = htole64( value );
#endif

   uint8_t *pValue = (uint8_t*)&value;

   add_uint8( pValue[0] );
   add_uint8( pValue[1] );
   add_uint8( pValue[2] );
   add_uint8( pValue[3] );
   add_uint8( pValue[4] );
   add_uint8( pValue[5] );
   add_uint8( pValue[6] );
   add_uint8( pValue[7] );
}

void
PacketLittleEndian::add_float ( float value )
{
   uint8_t *pValue = (uint8_t*)&value;

   add_uint8( pValue[0] );
   add_uint8( pValue[1] );
   add_uint8( pValue[2] );
   add_uint8( pValue[3] );
}

void
PacketLittleEndian::add_double( double value )
{
   uint8_t *pValue = (uint8_t*)&value;

   add_uint8( pValue[0] );
   add_uint8( pValue[1] );
   add_uint8( pValue[2] );
   add_uint8( pValue[3] );
   add_uint8( pValue[4] );
   add_uint8( pValue[5] );
   add_uint8( pValue[6] );
   add_uint8( pValue[7] );
}

uint16_t
PacketLittleEndian::get_uint16( size_t index ) const
{
   uint16_t value;
   uint8_t *pValue = (uint8_t*)&value;

   pValue[0] = get_uint8( index + 0 );
   pValue[1] = get_uint8( index + 1 );

#ifndef _WIN32
   value = le16toh( value );
#endif

   return value;
}

uint32_t
PacketLittleEndian::get_uint32( size_t index ) const
{
   uint32_t value;
   uint8_t *pValue = (uint8_t*)&value;

   pValue[0] = get_uint8( index + 0 );
   pValue[1] = get_uint8( index + 1 );
   pValue[2] = get_uint8( index + 2 );
   pValue[3] = get_uint8( index + 3 );

#ifndef _WIN32
   value = le32toh( value );
#endif

   return value;
}

uint64_t
PacketLittleEndian::get_uint64( size_t index ) const
{
   uint64_t value;
   uint8_t *pValue = (uint8_t*)&value;

   pValue[0] = get_uint8( index + 0 );
   pValue[1] = get_uint8( index + 1 );
   pValue[2] = get_uint8( index + 2 );
   pValue[3] = get_uint8( index + 3 );
   pValue[4] = get_uint8( index + 4 );
   pValue[5] = get_uint8( index + 5 );
   pValue[6] = get_uint8( index + 6 );
   pValue[7] = get_uint8( index + 7 );

#ifndef _WIN32
   value = le64toh( value );
#endif

   return value;
}

float
PacketLittleEndian::get_float( size_t index ) const
{
   float value;
   uint8_t *pValue = (uint8_t*)&value;

   pValue[0] = get_uint8( index + 0 );
   pValue[1] = get_uint8( index + 1 );
   pValue[2] = get_uint8( index + 2 );
   pValue[3] = get_uint8( index + 3 );

   return value;
}

double
PacketLittleEndian::get_double( size_t index ) const
{
   double value;
   uint8_t *pValue = (uint8_t*)&value;

   pValue[0] = get_uint8( index + 0 );
   pValue[1] = get_uint8( index + 1 );
   pValue[2] = get_uint8( index + 2 );
   pValue[3] = get_uint8( index + 3 );
   pValue[4] = get_uint8( index + 4 );
   pValue[5] = get_uint8( index + 5 );
   pValue[6] = get_uint8( index + 6 );
   pValue[7] = get_uint8( index + 7 );

   return value;
}
