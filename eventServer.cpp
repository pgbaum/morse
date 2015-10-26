#include "eventServer.h"

namespace
{
   gboolean on_key_press( GtkWidget *widget, GdkEventKey *event,
         gpointer data )
   {
      EventServer *server = (EventServer *)data;
      server->keyPress( event );
      return FALSE;
   }

   gboolean on_key_release( GtkWidget *widget, GdkEventKey *event,
         gpointer data )
   {
      EventServer *server = (EventServer *)data;
      server->keyRelease( event );
      return FALSE;
   }

   gboolean on_time_out( gpointer data )
   {
      EventServer *server = (EventServer *)data;
      server->timeOut( );
      return FALSE;
   }
}

EventServer::EventServer( GtkWidget *window ) : widget( window )
{
   if( widget )
   {
      keyPressHandler = g_signal_connect( G_OBJECT( widget ),
         "key_press_event", G_CALLBACK( on_key_press ), this );
      keyReleaseHandler = g_signal_connect( G_OBJECT( widget ),
         "key_release_event", G_CALLBACK( on_key_release ), this );
   }
}

EventServer::~EventServer( )
{
   cancelTimeout();
   if( widget && keyPressHandler > 0 )
      g_signal_handler_disconnect( widget, keyPressHandler );
   if( widget && keyReleaseHandler > 0 )
      g_signal_handler_disconnect( widget, keyReleaseHandler );
}

void EventServer::startTimeout( int ms )
{
   cancelTimeout();
   if( ms >= 0 )
      timer = g_timeout_add( ms, on_time_out, this );
}

void EventServer::cancelTimeout( void )
{
   if( timer )
      g_source_remove( timer );
   timer = 0;
}

