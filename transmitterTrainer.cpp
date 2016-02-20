#include "transmitterTrainer.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

TransmitterTrainer::TransmitterTrainer( int cc, GtkWidget *window )
      : ReceiverServer( window ), count( cc ),
      start( std::chrono::steady_clock::now() )
{
   std::srand( std::time( NULL ) );
   fillString();
   transmissionDone();
}

void TransmitterTrainer::printStat( ) const
{
   auto duration = std::chrono::duration_cast<std::chrono::seconds>(
         std::chrono::steady_clock::now() - start );
   std::cout << " - "
         << duration.count() / 60 << ":"
         << std::setfill('0') << std::setw( 2 ) << duration.count() % 60;
   std::cout << ": " << num << " messages, " << numWrong << " errors\n";
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
      std::cout << ": '" << dec << "'\nPlaying: " << orig << ": ";
      dec.erase( 0, dec.find_first_not_of(' ') );

      if( dec == orig )
      {
         if( state == NONE )
         {
            state = FOUND;
            std::cout << "Correct, send it one more time";
         }
         else
         {
            std::cout << "Correct";
            state = IN_REPEAT;
         }
      }
      else
      {
         ++numWrong;
         std::cout << "Wrong";
      }
      printStat();
      MorseTransmitter::setTickTime( receiver.getTickTime() );
      send( orig );
   }
}

void TransmitterTrainer::transmissionDone( )
{
   if( state == IN_REPEAT )
      fillString();
   std::cout << "To send: " << orig << '\n';
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
   ++num;
   state = NONE;
}
