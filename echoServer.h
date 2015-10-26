#ifndef ECHO_SERVER_H_INCLUDED
#define ECHO_SERVER_H_INCLUDED

#include "eventServer.h"
#include "morseReceiver.h"
#include "morseGdkTransmitter.h"


class EchoServer : public EventServer
{
public:
   EchoServer( GtkWidget *window );

   void setTickTime( int ms );
   int getTickTime( void ) const;

   void keyPress( GdkEventKey *event );
   void keyRelease( GdkEventKey *event );
   void timeOut( );

private:
   void decode( void );

   MorseGdkTransmitter transmitter;
   MorseReceiver receiver;
};

#endif
