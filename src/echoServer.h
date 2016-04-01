#ifndef ECHO_SERVER_H_INCLUDED
#define ECHO_SERVER_H_INCLUDED

#include "morseGdkReceiver.h"
#include "morseGdkTransmitter.h"

class EchoServer : public MorseGdkReceiver
{
public:
   EchoServer( GtkWidget *window );

protected:
   void decode( void );

private:
   MorseGdkTransmitter transmitter;
};

#endif
