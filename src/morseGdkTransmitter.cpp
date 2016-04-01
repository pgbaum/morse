#include "morseGdkTransmitter.h"
#include <glib.h>

namespace
{
   gboolean timeoutFunction( gpointer data )
   {
      MorseGdkTransmitter *transmitter = (MorseGdkTransmitter *)data;
      transmitter->timeoutCalled();
      return false;
   }
}

MorseGdkTransmitter::MorseGdkTransmitter()
{
   pipeline = gst_parse_launch(
         "audiotestsrc freq=800 ! audioconvert ! audioresample ! autoaudiosink",
         NULL );
}

MorseGdkTransmitter::~MorseGdkTransmitter()
{
   cancelTransmission();
   gst_object_unref( pipeline );
}

void MorseGdkTransmitter::cancelTransmission( )
{
   stop();
   cancelTimeout();
}

void MorseGdkTransmitter::timeoutCalled()
{
   timer = 0;
   stop();
   if( sendNextSignal() == false )
      transmissionDone();
}

void MorseGdkTransmitter::cancelTimeout()
{
   if( timer )
   {
      g_source_remove( timer );
      timer = 0;
   }
}

void MorseGdkTransmitter::stop( )
{
   gst_element_set_state( pipeline, GST_STATE_NULL );
}

void MorseGdkTransmitter::setState( bool on, int ms )
{
   timer = g_timeout_add( ms, timeoutFunction, this );
   if( on )
      gst_element_set_state( pipeline, GST_STATE_PLAYING );
}
