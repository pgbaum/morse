#ifndef ARG_PARSER_H_INCLUDED
#define ARG_PARSER_H_INCLUDED

/*
 * (c) 2016 P.G. Baum peter@dr-baum.net
 */

#include <vector>
#include <iosfwd>

class ArgParser
{
public:
   struct Arg;

private:
   std::vector<Arg *> args;

public:
   ~ArgParser( );
   void add( int *val, const char *name, const char *descr, bool required );
   void add( double *val, const char *name, const char *descr, bool required );
   void parse( int argc, char *argv[] );
   void printUsage( const char *name, std::ostream &ost ) const;
};

#endif
