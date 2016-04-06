#include "morseGdkReceiver.h"
#include <iostream>
#include <iomanip>

void MorseGdkReceiver::printSig(
      const std::pair<MorseCodec::Signal,float> &sig )
{
   std::cout << "Signal: "
         << (sig.first == MorseCodec::DOT_SPACE ? " "
               : MorseCodec::toText( sig.first ))
         << " "
         << std::fixed << std::fixed << std::setprecision(1)
         << sig.second << '\n';
}

MorseGdkReceiver::MorseGdkReceiver( GtkWidget *window )
      : GtkEventReceiver( window )
{
   pipeline = gst_parse_launch(
         "audiotestsrc freq=800 ! audioconvert ! audioresample ! autoaudiosink",
         NULL );
}

void MorseGdkReceiver::setTickTime( int ms )
{
   receiver.setTickTime( ms );
}

int MorseGdkReceiver::getTickTime( void ) const
{
   return receiver.getTickTime( );
}

void MorseGdkReceiver::timeOut(  )
{
   decode( );
   cancelTimeout( );
}

void MorseGdkReceiver::keyPress( GdkEventKey *event )
{
   cancelTimeout( );

   auto sig = receiver.setState( 1 );
   gst_element_set_state( pipeline, GST_STATE_PLAYING );
   printSig( sig );
}

void MorseGdkReceiver::keyRelease( GdkEventKey *event )
{
   cancelTimeout( );

   auto sig = receiver.setState( 0 );
   gst_element_set_state( pipeline, GST_STATE_NULL );

   startTimeout( receiver.getCharSpaceTime( ) * 15 / 10 );

   printSig( sig );
}

