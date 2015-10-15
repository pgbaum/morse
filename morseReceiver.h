#ifndef MORSERECEIVER_H_INCLUDED
#define MORSERECEIVER_H_INCLUDED

#include "morseCodec.h"
#include <chrono>

class MorseReceiver
{
public:
   MorseReceiver();

   bool isOn( void ) const;
   void setState( bool on );

   void setTickTime( int ms );
   int getTickTime( void ) const;

   bool isCharReady( void ) const;
   std::vector<std::pair<MorseCodec::Signal,float>> getDecoded( );

private:
   bool stateIsOn;
   bool charIsReady;
   int tickTime;
   std::chrono::time_point<std::chrono::steady_clock> timeStateChanged;
   std::vector<std::pair<MorseCodec::Signal,float>> signals;
};

#endif
