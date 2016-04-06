#ifndef GTK_EVENT_RECEIVER_H_INCLUDED
#define GTK_EVENT_RECEIVER_H_INCLUDED

#include <gtk/gtk.h>

class GtkEventReceiver
{
public:
   GtkEventReceiver( GtkWidget *window );
   virtual ~GtkEventReceiver();

   virtual void keyPress( GdkEventKey *event ) = 0;
   virtual void keyRelease( GdkEventKey *event ) = 0;
   virtual void timeOut( ) { }

   void startTimeout( int ms );
   void cancelTimeout( );

private:
   GtkWidget *widget = 0;
   guint timer = 0;
   guint keyPressHandler = 0;
   guint keyReleaseHandler = 0;
};

#endif
