#include "../argParser/argParser.h"
#include "echoServer.h"
#include "transmitterTrainer.h"
#include <iostream>

int main( int argc, char *argv[] )
{
   gtk_init( &argc, &argv );
   gst_init( NULL, NULL );

   ArgParser parser;

   int signalLength = 2;
   parser.add( &signalLength, "signalLength", "Length of the test signal", false );
   int tickTime = 100;
   parser.add( &tickTime, "tickTime", "Length of a 'dot' in ms", false );
   int numRepeat = 2;
   parser.add( &numRepeat, "numRepeat", "Number of repetitons of the same signal",
         false );
   std::string testSet( "1234" );
   parser.add( &testSet, "testSet", "Test set (1,2,3,4 or a combination of these)",
         false );

   std::string testLetters;
   try
   {
      parser.parse( argc, argv );


std::cout << testSet << '\n';
      if( testSet.find( '1' ) != std::string::npos )
         testLetters.append( "ET" );
      if( testSet.find( '2' ) != std::string::npos )
         testLetters.append( "IANM" );
      if( testSet.find( '3' ) != std::string::npos )
         testLetters.append( "SURWDKGO" );
      if( testSet.find( '4' ) != std::string::npos )
         testLetters.append( "HVFLPJBXCYZQ" );
      if( testLetters.empty() )
         testLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   }
   catch( const std::exception &e )
   {
      std::cout << "Error: " << e.what() << '\n';
      parser.printUsage( argv[0], std::cout );
      return 1;
   }

   GtkWidget *window = gtk_window_new( GTK_WINDOW_TOPLEVEL );

   g_signal_connect( G_OBJECT( window ), "destroy",
      G_CALLBACK( gtk_main_quit ), NULL );

   // EchoServer server( window );
   TransmitterTrainer server( signalLength, numRepeat, testLetters, window );
   server.setTickTime( tickTime );

   gtk_widget_show_all( window );

   gtk_main( );

   return 0;
}


