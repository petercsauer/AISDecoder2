#ifndef __PACKET_LITTLE_ENDIAN_H__
#define __PACKET_LITTLE_ENDIAN_H__

#include "Packet.h"

class PacketLittleEndian : public Packet
{
private:
   typedef Packet super;

public:
   PacketLittleEndian();
   
   virtual void build() { super::build(); }

protected:
   void add_uint16( uint16_t value );
   void add_uint32( uint32_t value );
   void add_uint64( uint64_t value );
   void add_float ( float    value );
   void add_double( double   value );

   uint16_t get_uint16( size_t index ) const;
   uint32_t get_uint32( size_t index ) const;
   uint64_t get_uint64( size_t index ) const;
   float    get_float ( size_t index ) const;
   double   get_double( size_t index ) const;
};

#endif
