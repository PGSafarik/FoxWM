#include<fx.h>
#include<xincs.h>

#include<FXAtom.h>
#include<FXWindowManager.h>

class FXAtomDict : public FXHash //Of<Atom, FXSelector>
{
public :
  FXAtomDict( FXWindowManager *app ) : wm( app )
  { }
  virtual ~FXAtomDict( ) { }

  void message_register( Atom a, FXSelector sel ) {
    if( FXHash::insert( static_cast<FXptr>( &a ), static_cast<FXptr>( &sel ) ) == NULL ) {
      fxwarning( "[FXAtomDict::send] No addin XAtom event !!!" );
    }
  }
  void message_unregister( Atom a )               { FXHash::remove( static_cast<FXptr>( &a ) ); }
//  void message_register( Atom a, FXSelector sel ) { insert( a, sel ); }
//  void message_unregister( Atom a )               { remove( a ); }

  FXSelector find( Atom ky )
  {
     FXSelector sel  = 0;
     FXival resh = FXHash::find( static_cast<FXptr>( &ky ) );

     if( resh != -1 ) { sel = *( static_cast<FXSelector*>( FXHash::value( resh ) ) ); }
     //fxwarning( "[FXAtomDict::find] Resulth: %d; %d ", resh, sel );
     return sel;
  }

  Atom key( FXint pos )
  {
    Atom a = 0;
    void *resh = static_cast<void*>( FXHash::key( pos ) );

    if( resh != NULL ) { a = *( static_cast<Atom*>( resh ) ); }
    return a;
  }

  FXSelector value( FXint pos )
  {
     FXSelector sel = 0;
     void *resh = static_cast<void*>( FXHash::value( pos ) );

     if( resh != NULL ) { sel = *( static_cast<FXSelector*>( resh ) ); }
     return sel;
  }

  virtual FXbool send( FXRawEvent &event, FXObject *target )
  {
     long resh = 0;
     FXSelector sel = FXAtomDict::find( event.xclient.message_type );
     if( sel > 0 ) {
       resh = target->handle( wm, sel, ( void* ) &event );
       //fxwarning( "[FXAtomDict::send] sending the GUI event with resulth: %d \n", resh );
     } else { fxwarning( "[FXAtomDict::send] No finding event !!!" ); }

     return ( ( resh == 1 ) ? true : false );
  }
protected :
  FXWindowManager *wm;

};

class FXWindowDispatcher : public FXAtomDict {


public :
  FXWindowDispatcher( FXWindowManager *app ) : FXAtomDict( app )
  { }
  ~FXWindowDispatcher( ) { }

  FXbool dispatch( FXRawEvent &event )
  {
    FXClientWindow *win = wm->getClientWindow( event.xclient.window );
    if( win != NULL ) {
      dump_event( event, win );
      return send( event, win );
    } else { fxwarning( "[FXWindowDispatcher::dispatch] No finding event widow!!! \n" ); }
    return false;
  }

  void dump_event( FXRawEvent &event, FXClientWindow *win )
  {
    fxwarning("[ FXWindowDispatcher ] Message %s -> %s \n", XGetAtomName( static_cast<Display*>( wm->getDisplay( ) ), event.xclient.message_type ),
    	      win->Title( ).text( ) );
  }
};

/* USAGE :
  FXAtomDict dispatcher( this );
  dispatcher.message_register( FXAtom::_NET_ACTIVE_WINDOW, FXSEL( SEL_COMMAND, WM_Client::ID_ACTIVE ) );
  dispatcher.message_register( FXAtom::_NET_CLOSE_WINDOW,  FXSEL( SEL_COMMAND, WM_Client::ID_CLOSE  ) );
  ...
  dispatcher.dispatch( _xevent );

*/
