#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "morseReceiver.h"
#include <iostream>
#include <iomanip>

guint timer;

const char *sigToString( MorseCodec::Signal sig )
{
   switch( sig )
   {
      case MorseCodec::DOT:          return ".";
      case MorseCodec::DASH:         return "_";
      case MorseCodec::DOT_SPACE:    return "DotSpace";
      case MorseCodec::LETTER_SPACE: return "NewLetter";
      case MorseCodec::WORD_SPACE:   return "' '";
      case MorseCodec::NONE:         ; // break through
   }
   return "None";
}

void printSig( const std::pair<MorseCodec::Signal,float> &sig )
{
   std::cout << "Signal: " << sigToString( sig.first )
         << " "
         << std::fixed << std::fixed << std::setprecision(1)
         << sig.second << '\n';
}

void decode( MorseReceiver *receiver )
{
   if( receiver->isCharReady() )
   {
      std::vector<MorseCodec::Signal> sig;
      std::cout << "Decoded: ";
      for( auto dec : receiver->getDecoded() )
      {
         sig.push_back( dec.first );
         if( dec.first != MorseCodec::DOT_SPACE )
            std::cout << sigToString( dec.first ) << " ";
      }
      auto dec = MorseCodec::decode( sig );
      std::cout << ": '" << dec << "'\n";
   }
}

void removeTimeout( void )
{
   if( timer )
      g_source_remove( timer );
   timer = 0;
}

gboolean timeoutFunction( gpointer user_data )
{
   std::cout << "In timeoutFunction\n";
   MorseReceiver *receiver = (MorseReceiver *)user_data;
   decode( receiver );
   timer = 0;
   return FALSE;
}

gboolean on_key_press( GtkWidget *widget, GdkEventKey *event,
      gpointer data )
{
   removeTimeout();
   MorseReceiver *receiver = (MorseReceiver *)data;

   auto sig = receiver->setState( 1 );
   printSig( sig );

   return FALSE;
}

gboolean on_key_release( GtkWidget *widget, GdkEventKey *event, gpointer data )
{
   removeTimeout();
   MorseReceiver *receiver = (MorseReceiver *)data;

   auto sig = receiver->setState( 0 );

   timer = g_timeout_add( receiver->getCharSpaceTime( ) * 15 / 10,
         timeoutFunction, data );

   printSig( sig );


   return FALSE;
}

int main( int argc, char *argv[] )
{
   MorseReceiver receiver;

   receiver.setTickTime( 100 );

   gtk_init( &argc, &argv );

   GtkWidget *window = gtk_window_new( GTK_WINDOW_TOPLEVEL );

   g_signal_connect( G_OBJECT( window ), "destroy",
      G_CALLBACK( gtk_main_quit ), NULL );

   g_signal_connect( G_OBJECT( window ),
      "key_press_event", G_CALLBACK( on_key_press ), &receiver );
   g_signal_connect( G_OBJECT( window ),
      "key_release_event", G_CALLBACK( on_key_release ), &receiver );

   gtk_widget_show_all( window );

   gtk_main( );

   return 0;
}


