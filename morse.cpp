#include "echoServer.h"

int main( int argc, char *argv[] )
{
   gtk_init( &argc, &argv );

   GtkWidget *window = gtk_window_new( GTK_WINDOW_TOPLEVEL );

   g_signal_connect( G_OBJECT( window ), "destroy",
      G_CALLBACK( gtk_main_quit ), NULL );

   EchoServer server( window );
   server.setTickTime( 100 );

   gtk_widget_show_all( window );

   gtk_main( );

   return 0;
}


