#include "receiverServer.h"
#include <iostream>
#include <iomanip>

void ReceiverServer::printSig( const std::pair<MorseCodec::Signal,float> &sig )
{
   std::cout << "Signal: " << MorseCodec::toText( sig.first )
         << " "
         << std::fixed << std::fixed << std::setprecision(1)
         << sig.second << '\n';
}

ReceiverServer::ReceiverServer( GtkWidget *window ) : EventServer( window )
{
}

void ReceiverServer::setTickTime( int ms )
{
   receiver.setTickTime( ms );
}

int ReceiverServer::getTickTime( void ) const
{
   return receiver.getTickTime( );
}

void ReceiverServer::timeOut(  )
{
   decode( );
   cancelTimeout( );
}

void ReceiverServer::keyPress( GdkEventKey *event )
{
   cancelTimeout( );

   auto sig = receiver.setState( 1 );
   printSig( sig );
}

void ReceiverServer::keyRelease( GdkEventKey *event )
{
   cancelTimeout( );

   auto sig = receiver.setState( 0 );

   startTimeout( receiver.getCharSpaceTime( ) * 15 / 10 );

   printSig( sig );
}

