#include "argParser/argParser.h"
#include "echoServer.h"
#include "transmitterTrainer.h"
#include <iostream>

int main( int argc, char *argv[] )
{
   gtk_init( &argc, &argv );
   gst_init( NULL, NULL );

   ArgParser parser;

   try
   {
      parser.parse( argc, argv );
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
   TransmitterTrainer server( 2, window );
   server.setTickTime( 100 );

   gtk_widget_show_all( window );

   gtk_main( );

   return 0;
}


