#ifndef MORSERECEIVER_H_INCLUDED
#define MORSERECEIVER_H_INCLUDED

#include "morseCodec.h"
#include <chrono>

class MorseReceiver
{
public:
   bool isOn( void ) const;
   std::pair<MorseCodec::Signal,float> setState( bool on );

   void setTickTime( int ms );
   int getTickTime( void ) const;
   int getCharSpaceTime( void ) const;

   bool isCharReady( void ) const;
   std::vector<MorseCodec::Signal> getDecoded( );

private:
   bool stateIsOn = false;
   bool charIsReady = false;
   int tickTime = 60;
   std::chrono::time_point<std::chrono::steady_clock> timeStateChanged;
   std::vector<MorseCodec::Signal> signals;
};

#endif
