#ifndef MORSE_H_INCLUDED
#define MORSE_H_INCLUDED

#include <climits>
#include <vector>
#include <string>

class MorseCodec
{
public:
   typedef enum { NONE, DASH, DOT, DOT_SPACE, LETTER_SPACE, WORD_SPACE } Signal;
   static const char invalidChar = CHAR_MAX;

   static char decode( const Signal *ps, int length, const Signal **next );
   static char decode( const std::vector<Signal> &s, const Signal **next )
   {
      return s.empty() ? 0 : decode( s.data(), (int)s.size(), next );
   }
   static std::string decode( const std::vector<Signal> &s );

   static std::vector<Signal> encode( char c );
   static std::vector<Signal> encode( const std::string &str );

   static const char *toText( Signal s );
   static std::string toString( const std::vector<Signal> &s )
   {
      return s.empty() ? std::string() : toString( s.data(), (int)s.size() );
   }
   static std::string toString( const Signal *ps, int length );
   static std::vector<Signal> stringToSignal( const std::string &s );

private:
   typedef char Encoded_type;
};

#endif
