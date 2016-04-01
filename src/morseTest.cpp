
#include "morseCodec.h"
#include "morseReceiver.h"
#include "morseTransmitter.h"
#include <iostream>
#include <thread>

class TestTransmitter : public MorseTransmitter
{
public:
   TestTransmitter( MorseReceiver &r ) : receiver( r ) {}
   void setState( bool on, int ms )
   {
      receiver.setState( on );
      std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
   }

private:
   MorseReceiver &receiver;
};

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
   const int ms = 30;
   MorseReceiver mr;
   mr.setTickTime( ms );

   for( auto s : MorseCodec::encode( c ) )
   {
      mr.setState( 1 );
      const int duration = (s == MorseCodec::DASH) ? 3 * ms : ms;
      std::this_thread::sleep_for( std::chrono::milliseconds( duration ) );
      mr.setState( 0 );
      std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
   }
   std::this_thread::sleep_for( std::chrono::milliseconds( 2 * ms ) );
   std::vector<MorseCodec::Signal> sig( mr.getDecoded() );

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

void testTransmitterOK( char c  )
{
   const int ms = 30;

   std::cout << '\'' << c << "': ";
   MorseReceiver receiver;
   TestTransmitter tt( receiver );
   receiver.setTickTime( ms );
   tt.setTickTime( ms );

   tt.send( std::string( { c } ) );

   while( tt.sendNextSignal( ) )
      ;

   std::vector<MorseCodec::Signal> sig( receiver.getDecoded() );

   std::cout << '\'' << MorseCodec::toString( sig ) << "' ";

   const char cDec = MorseCodec::decode( sig, NULL );
   if( cDec == c )
      std::cout << "OK";
   else
      std::cout << "could not decode  != '" << cDec << "'";

   std::cout << '\n';
}
void testTransmitter( )
{
   std::cout << "Transmitter test\n";
   for( char c = 'A';  c <= 'Z'; ++c )
      testTransmitterOK( c );
}

int main( void )
{

   testCodec();
   std::cout << '\n';
   testReceiver();
   std::cout << '\n';
   testTransmitter();

   return 0;
}
