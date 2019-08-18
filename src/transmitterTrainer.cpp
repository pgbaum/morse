#include "transmitterTrainer.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

TransmitterTrainer::TransmitterTrainer( int sl, int nr,
      const std::string &tl, GtkWidget *window )
      : MorseGdkReceiver( window ), testLetters( tl), signalLength( sl ), numRepeat( nr ),
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
         --actRepeat;
         std::cout << "Correct";
      }
      else
      {
         actRepeat = numRepeat + 1;
         ++numWrong;
         std::cout << "Wrong";
      }
      if( actRepeat > 0 )
         std::cout << ", send it again " << actRepeat << " times";
      printStat();
      MorseTransmitter::setTickTime( receiver.getTickTime() );
      send( orig );
   }
}

void TransmitterTrainer::gotSignal(
      const std::pair<MorseCodec::Signal,float> &sig )
{
   cancelTransmission();

   std::cout << "Signal: "
         << (sig.first == MorseCodec::DOT_SPACE ? " "
               : MorseCodec::toText( sig.first ))
         << " "
         << std::fixed << std::fixed << std::setprecision(1)
         << sig.second << '\n';
}


void TransmitterTrainer::transmissionDone( )
{
   if( actRepeat == 0 )
      fillString();
   std::cout << "To send: " << orig << '\n';
}

void TransmitterTrainer::fillString( )
{
   const int numVal = static_cast<int>( testLetters.size() );
   orig.clear();
   for( int k = 0; k < signalLength; ++k )
   {
      const int rand = std::rand() % numVal;
      orig.push_back( testLetters[rand] );
   }
   ++num;
   actRepeat = numRepeat;
}
