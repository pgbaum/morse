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
            return std::make_pair( MorseCodec::WORD_SPACE, len / 3 );
      }
      // will never be reached
      return std::make_pair( MorseCodec::NONE, .0f );
   }
}


MorseReceiver::MorseReceiver( ) :
   stateIsOn( false ),
   charIsReady( false ),
   tickTime( 60 )
{
}

bool MorseReceiver::isOn( void ) const
{
   return stateIsOn;
}

void MorseReceiver::setState( bool on )
{
   if( on != stateIsOn )
   {
      auto now = std::chrono::steady_clock::now();
      // calculate the difference not for the first one
      if( timeStateChanged != TimePoint( ) )
      {
         auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
               now - timeStateChanged );
         auto sig = decodeToSignal( (int)diff.count(), on == false,
               tickTime );
         if( !charIsReady )
         {
            charIsReady = (sig.first == MorseCodec::LETTER_SPACE
                  || sig.first == MorseCodec::LETTER_SPACE);
         }
         signals.push_back( sig );
      }
      timeStateChanged = now;
      stateIsOn = on;
   }
}

void MorseReceiver::setTickTime( int ms )
{
   tickTime = ms;
}

int MorseReceiver::getTickTime( void ) const
{
   return tickTime;
}

std::vector<std::pair<MorseCodec::MorseCodec::Signal,float>>
      MorseReceiver::getDecoded( )
{
   // return signal and set it to empty vector
   std::vector<std::pair<MorseCodec::MorseCodec::Signal,float>> ret;
   swap( signals, ret );
   return ret;
}
