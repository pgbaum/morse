#ifndef TRANSMITTER_TRAINER_H_INCLUDED
#define TRANSMITTER_TRAINER_H_INCLUDED

#include "receiverServer.h"
#include "morseGdkTransmitter.h"
#include <chrono>

class TransmitterTrainer : public ReceiverServer, private MorseGdkTransmitter
{
public:
   TransmitterTrainer( int count, GtkWidget *window );

   // to avoid ambiguous call to setTickTime() in main program
   void setTickTime( int ms ) { ReceiverServer::setTickTime( ms ); }

protected:
   void decode( void );
   void transmissionDone( void );

private:
   std::string orig;
   int count = 1;
   int num = 0;
   int numWrong = 0;
   std::chrono::time_point<std::chrono::steady_clock> start;
   enum {NONE, FOUND, IN_REPEAT } state = NONE;

   void fillString( );
   void printStat( ) const;
};

#endif
