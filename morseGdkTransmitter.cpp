#include "morseGdkTransmitter.h"
#include <glib.h>

namespace
{
   gboolean timeoutFunction( gpointer data )
   {
      MorseGdkTransmitter *transmitter = (MorseGdkTransmitter *)data;
      transmitter->stop();
      transmitter->sendNextSignal();
      return false;
   }
}

MorseGdkTransmitter::MorseGdkTransmitter()
{
   gst_init( NULL, NULL );
   pipeline = gst_parse_launch(
         "audiotestsrc freq=800 ! audioconvert ! audioresample ! autoaudiosink",
         NULL );
}

MorseGdkTransmitter::~MorseGdkTransmitter()
{
   gst_object_unref( pipeline );
}

void MorseGdkTransmitter::stop( )
{
   gst_element_set_state( pipeline, GST_STATE_NULL );
}

void MorseGdkTransmitter::setState( bool on, int ms )
{
   g_timeout_add( ms, timeoutFunction, this );
   if( on )
      gst_element_set_state( pipeline, GST_STATE_PLAYING );
}
