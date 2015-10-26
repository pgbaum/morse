#ifndef RECEIVER_SERVER_H_INCLUDED
#define RECEIVER_SERVER_H_INCLUDED

#include "eventServer.h"
#include "morseReceiver.h"

class ReceiverServer : public EventServer
{
public:
   static void printSig( const std::pair<MorseCodec::Signal,float> &sig );

   ReceiverServer( GtkWidget *window );

   void setTickTime( int ms );
   int getTickTime( void ) const;

   void keyPress( GdkEventKey *event );
   void keyRelease( GdkEventKey *event );
   void timeOut( );

protected:
   virtual void decode( void ) = 0;
   MorseReceiver receiver;
};

#endif
