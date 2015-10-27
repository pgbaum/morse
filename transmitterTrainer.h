#ifndef TRANSMITTER_TRAINER_H_INCLUDED
#define TRANSMITTER_TRAINER_H_INCLUDED

#include "receiverServer.h"
#include "morseGdkTransmitter.h"

class TransmitterTrainer : public ReceiverServer
{
public:
   TransmitterTrainer( int count, GtkWidget *window );

protected:
   void decode( void );

private:
   MorseGdkTransmitter transmitter;
   std::string orig;
   int count = 1;
   bool inRepeat = false;

   void fillString( );
};

#endif
