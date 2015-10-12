#ifndef MORSE_H_INCLUDED
#define MORSE_H_INCLUDED

#include <climits>
#include <vector>
#include <string>

class MorseCodec
{
public:
   /*
    * Each element is implicetely followed by a space of 1 unit.
    * That means that at the end of a character there have to be 2 SPACE
    * and at the end of word there have to be 6 SPACE
    */
   typedef enum { NONE, DASH, DOT, DOT_SPACE, LETTER_SPACE, WORD_SPACE } Signal;
   static const char invalidChar = CHAR_MAX;

   static char decode( const Signal *ps, int length, const Signal **next );
   static char decode( const std::vector<Signal> &s, const Signal **next )
   {
      return s.empty() ? 0 : decode( s.data(), (int)s.size(), next );
   }

   static std::vector<Signal> encode( char c );

   static std::string toString( const std::vector<Signal> &s )
   {
      return s.empty() ? std::string() : toString( s.data(), (int)s.size() );
   }
   static std::string toString( const Signal *ps, int length );

private:
   typedef char Encoded_type;
};

#endif
