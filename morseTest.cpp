
#include "morseCodec.h"
#include <iostream>

void testOK( char c )
{
   std::vector<MorseCodec::Signal> signal = MorseCodec::encode( c );
   std::cout << '\'' << c << "': ";
   if( signal.empty() )
   {
      std::cout << "Could not encode";
   }
   else
   {
      std::cout << '\'' << MorseCodec::toString( signal ) << "' ";
      const char dec = MorseCodec::decode( signal, NULL );
      if( c == dec )
      {
         std::cout << "OK";
      }
      else
      {
         std::cout << "could not decode  != '" << dec << "'";
      }
   }
   std::cout << '\n';
}

int main( void )
{
   testOK( ' ' );
   for( char c = 'A';  c <= 'Z'; ++c )
      testOK( c );
   return 0;
}
