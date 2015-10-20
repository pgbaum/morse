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

protected:
   // has to call after ms milliseconds sendNextSignal()
   void setState( bool on, int ms );

private:
   GstElement *pipeline;
};

#endif
