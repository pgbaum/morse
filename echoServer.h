#ifndef ECHO_SERVER_H_INCLUDED
#define ECHO_SERVER_H_INCLUDED

#include "receiverServer.h"
#include "morseGdkTransmitter.h"

class EchoServer : public ReceiverServer
{
public:
   EchoServer( GtkWidget *window );

protected:
   void decode( void );

private:
   MorseGdkTransmitter transmitter;
};

#endif
