#ifndef MORSEGDKTRANSMITTER_H_INCLUDED
#define MORSEGDKTRANSMITTER_H_INCLUDED

#include "morseTransmitter.h"
#include <gst/gst.h>

class MorseGdkTransmitter : public MorseTransmitter
{
public:
   MorseGdkTransmitter();
   ~MorseGdkTransmitter();

   void stop( void );
   void cancelTransmission( );

   void timeoutCalled();

protected:
   // has to call after ms milliseconds sendNextSignal()
   void setState( bool on, int ms );

private:
   GstElement *pipeline;
   guint timer = 0;

   void cancelTimeout();
};

#endif
