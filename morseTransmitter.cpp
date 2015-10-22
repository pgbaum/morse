#include "morseTransmitter.h"

void MorseTransmitter::setTickTime( int ms )
{
   tickTime = ms;
}

int MorseTransmitter::getTickTime( ) const
{
   return tickTime;
}

void MorseTransmitter::send( const std::vector<MorseCodec::Signal> &sig )
{
   lastSignal = MorseCodec::NONE;
   // signal is reversed
   signal.assign( sig.rbegin(), sig.rend() );

   sendNextSignal();
}

void MorseTransmitter::send( const std::string &str )
{
   std::vector<MorseCodec::Signal> sig = MorseCodec::encode( str );
   send( sig );
}

bool MorseTransmitter::sendNextSignal( void )
{
   while( !signal.empty() && signal.back() == MorseCodec::NONE )
      signal.pop_back();

   if( signal.empty() )
      return false;

   MorseCodec::Signal s = signal.back();

   // insert DOT_SPACE if necessary
   if( (lastSignal == MorseCodec::DOT || lastSignal == MorseCodec::DASH)
      && (s == MorseCodec::DOT || s == MorseCodec::DASH) )
   {
      s = MorseCodec::DOT_SPACE;
   }
   else
      signal.pop_back();

   lastSignal = s;
   switch( s )
   {
      case MorseCodec::DOT:          setState( true, tickTime ); break;
      case MorseCodec::DASH:         setState( true, 3 * tickTime ); break;
      case MorseCodec::DOT_SPACE:    setState( false, tickTime ); break;
      case MorseCodec::LETTER_SPACE: setState( false, 3 * tickTime ); break;
      case MorseCodec::WORD_SPACE:   setState( false, 7 * tickTime ); break;
      case MorseCodec::NONE:         ; // ignore
   }
   return true;
}
