#include "transmitterTrainer.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

TransmitterTrainer::TransmitterTrainer( int cc, GtkWidget *window ) 
      : ReceiverServer( window ), count( cc )
{
   std::srand( std::time( NULL ) );
   fillString();
}

void TransmitterTrainer::decode( )
{
   if( receiver.isCharReady() )
   {
      std::cout << "Decoded: ";
      std::vector<MorseCodec::Signal> sig( receiver.getDecoded() );
      for( auto dec : sig )
      {
         if( dec != MorseCodec::DOT_SPACE )
            std::cout << MorseCodec::toText( dec ) << " ";
      }
      auto dec = MorseCodec::decode( sig );
      std::cout << ": '" << dec << "'\n";
      dec.erase( 0, dec.find_first_not_of(' ') );
      transmitter.setTickTime( receiver.getTickTime() );
      transmitter.send( orig );
      if( dec == orig )
      {
         if( inRepeat )
         {
            fillString();
         }
         else
         {
            std::cout << "Correct, please send it one more time!\n";
            inRepeat = true;
         }
      }
      else
      {
         std::cout << "Should be: " << orig << '\n';
      }
   }
}

void TransmitterTrainer::fillString( )
{
   const int numVal = 'Z' - 'A' + 1;
   orig.clear();
   for( int k = 0; k < count; ++k )
   {
      const int rand = std::rand() % numVal;
      orig.push_back( 'A' + rand );
   }
   std::cout << "New string to send: " << orig << '\n';
   inRepeat = false;
}
