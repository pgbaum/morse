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
      if( dec == orig )
      {
         if( state == NONE )
         {
            state = FOUND;
            std::cout << "Correct, send it one more time: "
                  << orig << '\n';
         }
         else
         {
            std::cout << "Correct, that was: " << orig << '\n';
            state = IN_REPEAT;
         }
      }
      else
         std::cout << "This was wrong. Listen to it again: " << orig << '\n';
      MorseTransmitter::setTickTime( receiver.getTickTime() );
      send( orig );
   }
}

void TransmitterTrainer::transmissionDone( )
{
   if( state == IN_REPEAT )
      fillString();
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
   std::cout << "String #" << ++num <<" to send: " << orig << '\n';
   state = NONE;
}
