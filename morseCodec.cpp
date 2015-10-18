
#include "morseCodec.h"
#include <algorithm>
#include <stdexcept>

char MorseCodec::decode( const Signal *ps, int length, const Signal **next )
{
   const char encToChar[] =
   {
      'E',         // .        10  0x02
      'T',         // _        11  0x03
      'I',         // ..      100  0x04
      'A',         // .-      101  0x05
      'N',         // _.      110  0x06
      'M',         // __      111  0x07
      'S',         // ...    1000  0x08
      'U',         // .._    1001  0x09
      'R',         // ._.    1010  0x0A
      'W',         // .__    1011  0x0B
      'D',         // _..    1100  0x0C
      'K',         // _._    1101  0x0D
      'G',         // __.    1110  0x0E
      'O',         // ___    1110  0x0F
      'H',         // ....  10000  0x10
      'V',         // ..._  10001  0x11
      'F',         // .._.  10010  0x12
      invalidChar, // ..__  10011  0x13
      'L',         // ._..  10100  0x14
      invalidChar, // ._._  10101  0x15
      'P',         // .__.  10110  0x16
      'J',         // .___  10111  0x17
      'B',         // _...  11000  0x18
      'X',         // _.._  11001  0x19
      'C',         // _._.  11010  0x1A
      'Y',         // _.__  11011  0x1B
      'Z',         // __..  11100  0x1C
      'Q',         // __._  11101  0x1D
   };

   if( length <= 0 )
      return 0;

   Encoded_type enc( 1 );

   if( ps[0] == MorseCodec::WORD_SPACE )
   {
      if( next )
         *next = ps + 1;
      return ' ';
   }

   int read = 0;
   // skip white speace at the beginning
   while( read < length && (ps[read] == LETTER_SPACE || ps[read] == DOT_SPACE) )
      read++;

   for( ; read < length; ++read )
   {
      switch( ps[read] )
      {
         case NONE:
         case WORD_SPACE:
         case LETTER_SPACE:
                     goto exitLoop;
                     break;
         case DOT_SPACE:
                     // do nothing
                     break;
         case DOT:   enc <<= 1;
                     break;
         case DASH:
                     enc <<= 1;
                     enc += 1;
      }
      // too long sequence
      if( enc > 0x20 )
      {
         if( next )
         {
            *next = ps;
            // skip to next space
            for( ; read < length; ++read )
               switch( ps[read] )
               {
                  case NONE:
                  case WORD_SPACE:
                  case LETTER_SPACE:
                              break;
                  default:    ;
               }
            *next = ps + read;
         }
         return invalidChar;
      }
   }

exitLoop:
   if( next )
      *next = ps + read;
   const int idx = enc - 2;
   if( idx < 0 || idx >= (int)( sizeof( encToChar ) / sizeof( *encToChar ) ) )
      return invalidChar;
   return encToChar[idx];
}

std::string MorseCodec::decode( const std::vector<Signal> &s )
{
   std::string ret;
   if( !s.empty() )
   {
      const Signal *sp = s.data();
      int len = (int)s.size();
      while( len > 0 )
      {
         const Signal *next;
         ret.push_back( decode( sp, len, &next ) );
         len -= next - sp;
         sp = next;
      }
   }
   return ret;
}

std::vector<MorseCodec::Signal> MorseCodec::encode( char c )
{
   const Encoded_type charToEnc[] =
   {
      0x05,  // 'A'  .-      101
      0x18,  // 'B'  _...  11000
      0x1A,  // 'C'  _._.  11010
      0x0C,  // 'D'  _..    1100
      0x02,  // 'E'  .        10
      0x12,  // 'F'  .._.  10010
      0x0E,  // 'G'  __.    1110
      0x10,  // 'H'  ....  10000
      0x04,  // 'I'  ..      100
      0x17,  // 'J'  .___  10111
      0x0D,  // 'K'  _._    1101
      0x14,  // 'L'  ._..  10100
      0x07,  // 'M'  __      111
      0x06,  // 'N'  _.      110
      0x0F,  // 'O'  ___    1110
      0x16,  // 'P'  .__.  10110
      0x1D,  // 'Q'  __._  11101
      0x0A,  // 'R'  ._.    1010
      0x08,  // 'S'  ...    1000
      0x03,  // 'T'  _        11
      0x09,  // 'U'  .._    1001
      0x11,  // 'V'  ..._  10001
      0x0B,  // 'W'  .__    1011
      0x19,  // 'X'  _.._  11001
      0x1B,  // 'Y'  _.__  11011
      0x1C,  // 'Z'  __..  11100
   };

   if( c == ' ' )
   {
      return std::vector<Signal>( { WORD_SPACE } );
   }

   std::vector<Signal> ret;

   if( c < 'A' || c > 'Z' )
      return ret;

   Encoded_type val = charToEnc[(int)(c-'A')];
   for( ; val > 1; val >>= 1 )
   {
      if( val & 1 )
         ret.push_back( DASH );
      else
         ret.push_back( DOT );
   }
   // if we reverse the bits in the constants, we could avoid the reverse
   std::reverse( ret.begin(), ret.end() );

   return ret;
}

std::string MorseCodec::toString( const Signal *ps, int length )
{
   std::string ret;
   while( length > 0 )
   {
      switch( *ps )
      {
         case DASH:         ret.push_back( '_' ); break;
         case DOT:          ret.push_back( '.' ); break;
         case LETTER_SPACE: ret.push_back( ' ' ); break;
         case WORD_SPACE:   ret.push_back( ' ' );
                            ret.push_back( ' ' );
                            ret.push_back( ' ' ); break;
         case DOT_SPACE:    break;
         case NONE:         return ret;
      }
      --length;
      ++ps;
   }
   return ret;
}

std::vector<MorseCodec::Signal> MorseCodec::stringToSignal(
      const std::string &s )
{
   std::vector<Signal> sig;
   for( char c : s )
   {
      switch( c )
      {
         case '.': sig.push_back( DOT ); break;
         case '_': sig.push_back( DASH ); break;
         case ' ': sig.push_back( LETTER_SPACE ); break;
         default:  throw std::invalid_argument( "Invalid character." );
      }
   }
   return sig;
}

