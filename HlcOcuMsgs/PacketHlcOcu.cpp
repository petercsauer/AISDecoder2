#include "PacketHlcOcu.h"

const uint8_t PacketHlcOcu::sync0 = 0x55;
const uint8_t PacketHlcOcu::sync1 = 0xAA;
const uint8_t PacketHlcOcu::sync2 = 0x00;
const uint8_t PacketHlcOcu::sync3 = 0xFF;

const size_t PacketHlcOcu::dataOffset = 8;

PacketHlcOcu::PacketHlcOcu()
{
}

void
PacketHlcOcu::build()
{
   super::build();

   add_uint8( sync0 );
   add_uint8( sync1 );
   add_uint8( sync2 );
   add_uint8( sync3 );

   add_uint16( length() );
   add_uint16( type() );
   
   buildData();
   
   add_uint8( calcChecksum() );
}

bool
PacketHlcOcu::parse( const std::vector<uint8_t> &newData )
{
   // save data without checksum since it is not part of the calculation
   std::vector<uint8_t> v(newData);
   uint8_t checksum = v.back();
   v.pop_back();
   data_replace(v);

   // min size is 
   //   four sync bytes
   // + two length bytes
   // + two type bytes
   // + one checksum byte
   // = nine bytes
   if( newData.size() < 9 ) {
    //printf("Too small\n");
      return false;
   }

   // check for sync
   if( get_uint8(0) != sync0
    || get_uint8(1) != sync1
    || get_uint8(2) != sync2
    || get_uint8(3) != sync3 ) {
    //printf("Bad sync\n");
      return false;
   }

   // check for correct length, which is for all bytes excluding
   //   four sync bytes
   // + two length bytes
   // + one checksum byte
   // = seven bytes
   if( getLengthField() != newData.size() - 7 ) {
    //printf("wrong size\n");
      return false;
   }

   // check for type-specific correct packet length
   // if we're calling from PacketHlcOcu (which has type 0), we're just
   // checking that the packet is sane in general, so don't check the size
   // field in that case
   if ( type() != 0 && getLengthField() != length() ) {
    //printf("wrong length\n");
      return false;
   }

   // verify type
   // if we're calling from PacketHlcOcu (which has type 0), we're just
   // checking that the packet is sane in general, so don't check the type in
   // that case
   if( type() != 0 && getTypeField() != type() ) {
    //printf("wrong type\n");
      return false;
   }

   // verify checksum
   if( newData.back() != calcChecksum() ) {
    //printf("bad checksum\n");
      return false;
   }

   // add checksum back in case we need it later
   add_uint8( checksum );
   
   return true;
}

uint16_t
PacketHlcOcu::getLengthField() const
{
   if( data().size() >= 6 ) {
      return get_uint16( 4 );
   }

   return 0;
}

uint16_t
PacketHlcOcu::getTypeField() const
{
   if( data().size() >= 8 ) {
      return get_uint16( 6 );
   }

   return 0;
}

/*
 *  Checksum is calculated on all bytes excluding the sync bytes and the
 *  checksum byte itself.
 *  
 *  calcChecksum() assumes that the sync bytes are present in data.
 *  calcChecksum() assumes that the checksum byte is not present.
 */
uint8_t
PacketHlcOcu::calcChecksum() const
{
   uint8_t checksum = 0x00;
   uint8_t loop = 0;
   for
   (
      std::vector<uint8_t>::const_iterator pByte = data().begin();
      pByte != data().end();
      pByte++
   )
   {
      if( loop < 4 ) {
         loop++;
         continue;
      }

      checksum = checksum + *pByte;
   }

   return checksum;
}
