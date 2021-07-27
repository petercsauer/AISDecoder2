#ifndef __PACKET_H__
#define __PACKET_H__

#include <vector>
#include <string>
#include <stdint.h>

class Packet
{
public:
   Packet();

   // returns data in a human readable string
   std::string readable() const;

   // returns data in a byte stream string
   std::string string() const;
   
   // return a reference to the constructed or inserted data
   const std::vector<uint8_t> &data() const;

   virtual void build() { data_clear(); }
   virtual bool parse( const std::vector<uint8_t> &newData ) = 0;
   bool parseString( const std::string &str );
   bool parsePacket( const Packet &p );

protected:
   void data_clear();
   void data_replace( const std::vector<uint8_t> &newData );

   void add_uint8( uint8_t  value );
   uint8_t get_uint8( size_t index ) const;
   
   void add_int8( int8_t  value ) { add_uint8(uint8_t(value)); }
   int8_t get_int8( size_t index ) { return int8_t(get_uint8(index)); }

   void add_vec_uint8( const std::vector<uint8_t> &value );
   std::vector<uint8_t> get_vec_uint8( size_t index, uint32_t length ) const;

   void add_string( const std::string &value );
   std::string get_string( size_t index, uint32_t length ) const;

   virtual void add_uint16( uint16_t value ) = 0;
   virtual void add_uint32( uint32_t value ) = 0;
   virtual void add_uint64( uint64_t value ) = 0;
   
   void add_int16( int16_t value ) { add_uint16(uint16_t(value)); }
   void add_int32( int32_t value ) { add_uint32(uint32_t(value)); }
   void add_int64( int64_t value ) { add_uint64(uint64_t(value)); }

   virtual void add_float ( float    value ) = 0;
   virtual void add_double( double   value ) = 0;

   virtual uint16_t get_uint16( size_t index ) const = 0;
   virtual uint32_t get_uint32( size_t index ) const = 0;
   virtual uint64_t get_uint64( size_t index ) const = 0;
   
   int16_t get_int16( size_t index ) const { return int16_t(get_uint16(index)); }
   int32_t get_int32( size_t index ) const { return int32_t(get_uint32(index)); }
   int64_t get_int64( size_t index ) const { return int64_t(get_uint64(index)); }

   virtual float    get_float ( size_t index ) const = 0;
   virtual double   get_double( size_t index ) const = 0;

private:
   std::vector<uint8_t> m_data;
};

#endif
