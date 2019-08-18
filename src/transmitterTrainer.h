#ifndef TRANSMITTER_TRAINER_H_INCLUDED
#define TRANSMITTER_TRAINER_H_INCLUDED

#include "morseGdkReceiver.h"
#include "morseGdkTransmitter.h"
#include <chrono>

class TransmitterTrainer : public MorseGdkReceiver, private MorseGdkTransmitter
{
public:
   TransmitterTrainer( int signalLength, int numRepeat,
         const std::string &testLetters, GtkWidget *window );

   // to avoid ambiguous call to setTickTime() in main program
   void setTickTime( int ms ) { MorseGdkReceiver::setTickTime( ms ); }

protected:
   // implementation of MorseGdkReceiver interface
   void decode( void );
   void gotSignal( const std::pair<MorseCodec::Signal,float> &sig );

   // implementation of MorseGdkTransmitter interface
   void transmissionDone( void );

private:
   std::string testLetters;
   std::string orig;
   int signalLength = 1;
   int numRepeat = 1;
   int actRepeat = 0;
   int num = 0;
   int numWrong = 0;
   std::chrono::time_point<std::chrono::steady_clock> start;

   void fillString( );
   void printStat( ) const;
};

#endif
