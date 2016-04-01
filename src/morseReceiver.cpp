#include "morseReceiver.h"

namespace
{
   using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

   std::pair<MorseCodec::Signal,float> decodeToSignal( int dt, bool toOff,
         int tickTime )
   {
      /*
       * a DOT is 1 tick
       * a DASH is 3 ticks
       * a space between DOTs and DASHs is 1 tick
       * a space between letters is 3 ticks
       * a space between words is 7 ticks
       */
      const float len = (float)dt / tickTime;
      if( toOff )
      {
         if( len > 2 )
            return std::make_pair( MorseCodec::DASH, len / 3 );
         else
            return std::make_pair( MorseCodec::DOT, len );
      }
      else
      {
         if( len < 2 )
            return std::make_pair( MorseCodec::DOT_SPACE, len );
         else if( len < 5 )
            return std::make_pair( MorseCodec::LETTER_SPACE, len / 3 );
         else
            return std::make_pair( MorseCodec::WORD_SPACE, len / 7 );
      }
      // will never be reached
      return std::make_pair( MorseCodec::NONE, .0f );
   }
}

bool MorseReceiver::isOn( void ) const
{
   return stateIsOn;
}

bool MorseReceiver::isCharReady( void ) const
{
   // either a char has been detected, or the current off-state is longer
   // than a LETTER_SPACE
   return charIsReady
      || (!isOn() && std::chrono::steady_clock::now()
            >= timeStateChanged
            + std::chrono::milliseconds( getCharSpaceTime() ) );
}

std::pair<MorseCodec::Signal,float> MorseReceiver::setState( bool on )
{
   std::pair<MorseCodec::Signal,float> sig;
   if( on != stateIsOn )
   {
      auto now = std::chrono::steady_clock::now();
      // calculate the difference not for the first one
      if( timeStateChanged != TimePoint( ) )
      {
         auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
               now - timeStateChanged );
         sig = decodeToSignal( (int)diff.count(), on == false,
               tickTime );
         if( !charIsReady )
         {
            charIsReady = (sig.first == MorseCodec::LETTER_SPACE
                  || sig.first == MorseCodec::WORD_SPACE);
         }
         signals.push_back( sig.first );
      }
      timeStateChanged = now;
      stateIsOn = on;
   }
   return sig;
}

void MorseReceiver::setTickTime( int ms )
{
   tickTime = ms;
}

int MorseReceiver::getTickTime( void ) const
{
   return tickTime;
}

int MorseReceiver::getCharSpaceTime( void ) const
{
   return 3 * tickTime;
}

std::vector<MorseCodec::Signal> MorseReceiver::getDecoded( )
{
   // return signal and set it to empty vector
   std::vector<MorseCodec::Signal> ret;
   swap( signals, ret );
   charIsReady = false;
   return ret;
}
