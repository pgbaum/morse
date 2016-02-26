#include "echoServer.h"
#include <iostream>
#include <iomanip>

EchoServer::EchoServer( GtkWidget *window ) : MorseGdkReceiver( window )
{
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
      transmitter.setTickTime( receiver.getTickTime() );
      transmitter.send( dec );
   }
}

