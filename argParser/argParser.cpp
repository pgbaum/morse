
/*
 * (c) 2016 P.G. Baum peter@dr-baum.net
 */

#include "argParser.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <iomanip>

struct ArgParser::Arg
{
   std::string name;
   std::string descr;
   bool required;

   Arg( const char *n, const char *d, bool r ) :
      name( n ), descr( d ), required( r )
   {
   }

   virtual void parse( const std::string &val ) = 0;
   virtual void printDefault( std::ostream &ost ) = 0;

   void assertNothingTrailing( const std::string &val, int n )
   {
      if( (int)val.size() != n )
         throw std::invalid_argument(
               std::string( "Trailing characters \"" )
               + val.substr( n ) + "\" in " + val );
   }
   virtual ~Arg() = default;
};

struct IntArg : public ArgParser::Arg
{
   int *p;

   IntArg( int *v, const char *n, const char *d, bool r ) :
         Arg( n, d, r ), p( v )
   {
   }
   void parse( const std::string &val )
   {
      size_t n;
      *p = std::stoi( val, &n );
      assertNothingTrailing( val, n );
   }
   void printDefault( std::ostream &ost )
   {
      ost << *p;
   }
};

struct DoubleArg : public ArgParser::Arg
{
   double *p;

   DoubleArg( double *v, const char *n, const char *d, bool r ) :
         Arg( n, d, r ), p( v )
   {
   }
   void parse( const std::string &val )
   {
      size_t n;
      *p = std::stod( val, &n );
      assertNothingTrailing( val, n );
   }
   void printDefault( std::ostream &ost )
   {
      ost << *p;
   }
};

ArgParser::~ArgParser( )
{
   for( auto p : args )
      delete p;
}

void ArgParser::add( int *val, const char *name, const char *descr,
      bool required )
{
   args.push_back( new IntArg{val, name, descr, required} );
}

void ArgParser::add( double *val, const char *name, const char *descr,
      bool required )
{
   args.push_back( new DoubleArg{val, name, descr, required} );
}

void ArgParser::parse( int argc, char *argv[] )
{
   std::vector<char> used( args.size(), 0 );

   for( int k = 1; k < argc; k += 2 )
   {
      if( argv[k][0] != '-' || argv[k][1] != '-' )
         throw std::invalid_argument(
               std::string( "Options have to start with '--': " )
               + argv[k] );

      std::string par( argv[k] + 2 );
      int m = 0;
      for( ; m < (int)args.size(); ++m )
      {
         Arg *arg = args[m];
         if( par == arg->name )
         {
            if( used[m] )
               throw std::invalid_argument(
                     std::string( "Argument " )
                           + argv[k] + " given more than once" );

            if( k > argc - 2 )
               throw std::invalid_argument(
                     std::string( "Missing value for option " )
                           + argv[k] );

            arg->parse( std::string( argv[k+1] ) );
            used[m] = 1;
            break;
         }
      }
      if( m == (int)args.size() )
         throw std::invalid_argument(
               std::string( "Unknown option: \"" ) + argv[k] + "\"");
   }
   // check whether required options are set
   for( int k = 0; k < (int)args.size(); ++k )
   {
      Arg *arg = args[k];
      if( arg->required && !used[k] )
         throw std::invalid_argument(
               std::string( "Parameter --" ) + arg->name + " is required");
   }
}

void ArgParser::printUsage( const char *name, std::ostream &ost ) const
{
   int nameLen = 0;
   for( auto arg : args )
      if( (int)arg->name.size() > nameLen )
         nameLen = (int)arg->name.size();

   ost << "Usage:\n" << name << ' ';
   for( auto arg : args )
      if( arg->required )
         ost << "--" << arg->name << " VALUE ";
   ost << "[OPTION]...\n";

   for( auto arg : args )
   {
      ost << "--" << std::setw( nameLen ) << std::left << arg->name
            << ": " << arg->descr << " (default: ";
      arg->printDefault( ost );
      ost << ")";
      if( arg->required )
         ost << " (required)";
      ost << '\n';
   }
}

#ifdef TEST_ARG_PARSER

#include <iostream>
int main( int argc, char *argv[] )
{
   ArgParser parser;

   int length = 1024;
   parser.add( &length, "length", "Length of something", false );
   double factor = 3.1415;
   parser.add( &factor, "factor", "Factor for something", false );
   int required = 10;
   parser.add( &required, "required", "Something required", true );
   double required2 = 2.14;
   parser.add( &required2, "required2", "Something else required", true );

   try
   {
      parser.parse( argc, argv );
      // parser.wasSet( &length );
   }
   catch( const std::exception &e )
   {
      std::cout << "Error: " << e.what() << '\n';
      parser.printUsage( argv[0], std::cout );
      return 1;
   }

   std::cout << "Required: " << required << '\n';
   std::cout << "Length:   " << length << '\n';
   std::cout << "Factor:   " << factor << '\n';
}

#endif

