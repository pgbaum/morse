#ifndef MORSETRANSMITTER_H_INCLUDED
#define MORSETRANSMITTER_H_INCLUDED

#include "morseCodec.h"

class MorseTransmitter
{
public:

   MorseTransmitter();
   virtual ~MorseTransmitter();

   void setTickTime( int ms );

   void send( const std::vector<MorseCodec::Signal> &sig );
   void send( const std::string &sig );

   // to be called after each state has ended
   // returns 0 if no more signal is to be sent
   bool sendNextSignal( void );

protected:
   // has to call after ms milliseconds sendNextSignal()
   virtual void setState( bool on, int ms ) = 0;

private:
   std::vector<MorseCodec::Signal> signal;
   int tickTime;
   MorseCodec::Signal lastSignal;
};

#endif
