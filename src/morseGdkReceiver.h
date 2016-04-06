#ifndef MORSE_GDK_RECEIVER_H_INCLUDED
#define MORSE_GDK_RECEIVER_H_INCLUDED

#include "gtkEventReceiver.h"
#include "morseReceiver.h"
#include <gst/gst.h>

class MorseGdkReceiver : public GtkEventReceiver
{
public:
   MorseGdkReceiver( GtkWidget *window );

   void setTickTime( int ms );
   int getTickTime( void ) const;

   void keyPress( GdkEventKey *event );
   void keyRelease( GdkEventKey *event );
   void timeOut( );

protected:
   virtual void decode( void ) = 0;
   virtual void gotSignal( const std::pair<MorseCodec::Signal,float> &sig ) = 0;
   MorseReceiver receiver;

private:
   GstElement *pipeline;
};

#endif
