#include "echoServer.h"
#include <gdk/gdkkeysyms.h>
#include <iostream>
#include <iomanip>

namespace
{
   void printSig( const std::pair<MorseCodec::Signal,float> &sig )
   {
      std::cout << "Signal: " << MorseCodec::toText( sig.first )
            << " "
            << std::fixed << std::fixed << std::setprecision(1)
            << sig.second << '\n';
   }
}

EchoServer::EchoServer( GtkWidget *window ) : EventServer( window )
{
}

void EchoServer::setTickTime( int ms )
{
   transmitter.setTickTime( ms );
   receiver.setTickTime( ms );
}

int EchoServer::getTickTime( void ) const
{
   return transmitter.getTickTime( );
}

void EchoServer::decode( )
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
      std::cout << ": '" << dec << "'\n";
      transmitter.send( dec );
   }
}

void EchoServer::timeOut(  )
{
   std::cout << "In timeoutFunction\n";
   decode( );
   cancelTimeout( );
}

void EchoServer::keyPress( GdkEventKey *event )
{
   cancelTimeout( );

   auto sig = receiver.setState( 1 );
   printSig( sig );
}

void EchoServer::keyRelease( GdkEventKey *event )
{
   cancelTimeout( );

   auto sig = receiver.setState( 0 );

   startTimeout( receiver.getCharSpaceTime( ) * 15 / 10 );

   printSig( sig );
}

