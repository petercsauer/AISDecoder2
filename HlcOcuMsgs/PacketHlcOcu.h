#ifndef __PACKET_HLC_OCU_H__
#define __PACKET_HLC_OCU_H__

#include "../legos/Packet/PacketLittleEndian.h"
#include <stdio.h>

class PacketHlcOcu : public PacketLittleEndian
{
private:
   typedef PacketLittleEndian super;

public:
   PacketHlcOcu();

   void build();
   virtual bool parse( const std::vector<uint8_t> &newData );

   virtual uint16_t length() const { return 2; }
   virtual uint16_t type()   const { return 0; }

   uint16_t getLengthField() const;
   uint16_t getTypeField()   const;

protected:
   virtual void buildData() {}

   const static size_t dataOffset;

private:
   uint8_t calcChecksum() const;

   const static uint8_t sync0;
   const static uint8_t sync1;
   const static uint8_t sync2;
   const static uint8_t sync3;
};


#endif
