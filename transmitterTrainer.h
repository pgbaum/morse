#ifndef TRANSMITTER_TRAINER_H_INCLUDED
#define TRANSMITTER_TRAINER_H_INCLUDED

#include "morseGdkReceiver.h"
#include "morseGdkTransmitter.h"
#include <chrono>

class TransmitterTrainer : public MorseGdkReceiver, private MorseGdkTransmitter
{
public:
   TransmitterTrainer( int signalLength, GtkWidget *window );

   // to avoid ambiguous call to setTickTime() in main program
   void setTickTime( int ms ) { MorseGdkReceiver::setTickTime( ms ); }

protected:
   void decode( void );
   void transmissionDone( void );

private:
   std::string orig;
   int signalLength = 1;
   int num = 0;
   int numWrong = 0;
   std::chrono::time_point<std::chrono::steady_clock> start;
   enum {NONE, FOUND, IN_REPEAT } state = NONE;

   void fillString( );
   void printStat( ) const;
};

#endif
