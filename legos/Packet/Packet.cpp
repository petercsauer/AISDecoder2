#include "Packet.h"
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/foreach.hpp>

Packet::Packet()
{
}

std::string
Packet::readable() const
{
   std::vector<std::string> strs;

   BOOST_FOREACH(uint8_t c, m_data) {
      strs.push_back(boost::str(boost::format("%02x") % (uint32_t)c));
   }

   return boost::algorithm::join(strs, ",");
}

std::string
Packet::string() const
{
    return std::string(m_data.begin(), m_data.end());
}

const std::vector<uint8_t> &
Packet::data() const
{
   return m_data;
}
   
bool
Packet::parseString( const std::string &str )
{
   return parse( std::vector<uint8_t>(str.begin(), str.end()) );
}

bool
Packet::parsePacket( const Packet &p )
{
   return parse( p.data() );
}


void
Packet::data_clear()
{
   m_data.clear();
}

void
Packet::data_replace( const std::vector<uint8_t> &newData )
{
   m_data = newData;
}

void
Packet::add_uint8( uint8_t  value )
{
   m_data.push_back( value );
}

uint8_t
Packet::get_uint8( size_t index ) const
{
   return m_data[index];
}
   
void 
Packet::add_vec_uint8( const std::vector<uint8_t> &value )
{
   m_data.insert(m_data.end(), value.begin(), value.end());
}

std::vector<uint8_t> 
Packet::get_vec_uint8( size_t index, uint32_t length ) const
{
    return std::vector<uint8_t>(m_data.begin()+index, 
          m_data.begin()+index+length);
}

void
Packet::add_string( const std::string &value )
{
   m_data.insert(m_data.end(), value.begin(), value.end());
}

std::string 
Packet::get_string( size_t index, uint32_t length ) const
{
   return std::string(m_data.begin()+index, 
         m_data.begin()+index+length);
}
