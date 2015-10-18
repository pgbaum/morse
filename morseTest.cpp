
#include "morseCodec.h"
#include "morseReceiver.h"
#include <iostream>
#include <thread>

void testCodecOK( char c )
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

void testCodecKO( const std::string &s )
{
   std::cout << s << ": ";
   const auto signal = MorseCodec::stringToSignal( "_.___" );
   const auto dec = MorseCodec::decode( signal, NULL );
   if( dec == MorseCodec::invalidChar )
      std::cout << " failed OK\n";
   else
      std::cout << " ERROR\n";
   const std::string str = MorseCodec::decode( signal );
}

void testCodec( )
{
   std::cout << "Codec test\n";
   testCodecOK( ' ' );
   for( char c = 'A';  c <= 'Z'; ++c )
      testCodecOK( c );

   testCodecKO( "._._" );
   testCodecKO( "___." );
   testCodecKO( "____" );
   testCodecKO( "....." );
   testCodecKO( "_.___" );
}

void testReceiverOK( char c )
{
   std::cout << '\'' << c << "': ";
   MorseReceiver mr;
   const int ms = mr.getTickTime();

   for( auto s : MorseCodec::encode( c ) )
   {
      mr.setState( 1 );
      const int duration = (s == MorseCodec::DASH) ? 3 * ms : ms;
      std::this_thread::sleep_for( std::chrono::milliseconds( duration ) );
      mr.setState( 0 );
      std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
   }
   std::this_thread::sleep_for( std::chrono::milliseconds( 2 * ms ) );
   std::vector<MorseCodec::Signal> sig;
   for( auto dec : mr.getDecoded() )
      sig.push_back( dec.first );

   std::cout << '\'' << MorseCodec::toString( sig ) << "' ";

   const char cDec = MorseCodec::decode( sig, NULL );
   if( cDec == c )
      std::cout << "OK";
   else
      std::cout << "could not decode  != '" << cDec << "'";

   std::cout << '\n';
}
void testReceiver( )
{
   std::cout << "Receiver test\n";
   for( char c = 'A';  c <= 'Z'; ++c )
      testReceiverOK( c );
}

int main( void )
{
   testCodec();
   std::cout << '\n';
   testReceiver();

   return 0;
}
