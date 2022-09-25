/*******************************************************************************
*                         fxwm - FOX Window Manager                            *
********************************************************************************
*        Copyright (C) 2002-2004 by Sander Jansen. All Rights Reserved         *
*                               ---                                            *
* This program is free software; you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by         *
* the Free Software Foundation; either version 2 of the License, or            *
* (at your option) any later version.                                          *
*                                                                              *
* This program is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of               *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
* GNU General Public License for more details.                                 *
* You should have received a copy of the GNU General Public License            *
* along with this program; if not, write to the Free Software                  *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    *
*******************************************************************************/
// standard library
#include<unistd.h>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<vector>
#include<new>

// Fox library
#include<xincs.h>
#include<fx.h>
#include<FXArray.h>
#include<FXGLVisual.h>

// FXWM
#include<FXWindowManager.h>
#include<FXClientWindow.h>
#include<FXWorkspace.h>
#include<WMStack.h>
#include<FXWindowProperty.h>
#include<FXAtom.h>
#include<FXAtomDict.h>

//#include<DWindowNode.h>

#define DISPLAY ((Display*)(getDisplay()))

static Window badwindow=None;

static int fxerrormanager( Display *display, XErrorEvent *e ){
   FXchar descr[1024];
    XGetErrorText( display, e->error_code, descr, 1024 );
    std::cerr << "[::fxerrorhandler]: \"" << descr << "\" for window id: " << e->resourceid << std::endl;
	std::cerr << "-  Major opcode of failed request (???): " << e->request_code << std::endl;
	std::cerr << "-  Serial number of failed request: " << e->serial << std::endl;
	std::cerr << "-  Current serial number in output stream: ?????"  << std::endl;
	return 0;
  }

static void Print_event( const FXRawEvent &ev, FXint id, FXClientWindow *win ) {
  FXString n, t = " " ;
  FXID   wid = 0;
  //if( win == NULL ) { return; }
  switch ( ev.xany.type ) {
    //case GraphicExpose: n = "GraphicExpose"; break;
    case Expose: n = "Expose"; wid = ev.xexpose.window; break;
    case NoExpose: n = "NoExpose"; break;
    case KeymapNotify: n = "KeymapNotify"; break;
    case KeyPress: n = "KeyPress"; break;
    case KeyRelease: n = "KeyRelease"; break;
    case MotionNotify: n = "MotionNotify"; break;
    case ButtonPress: n = "ButtonPress"; break;
    case ButtonRelease: n = "ButtonRelease"; break;
    case EnterNotify: n = "EnterNotify"; break;
    case LeaveNotify: n = "LeaveNotify"; break;
    case FocusIn: n = "FocusIn"; wid = ev.xfocus.window; break;
    case FocusOut: n = "FocusOut"; wid = ev.xfocus.window; break;
    case MapNotify: n = "MapNotify"; break;
    case MapRequest: n = "MapRequest"; break;
    case UnmapNotify: n = "UnmapNotify"; break;
    case CreateNotify: n = "CreateNotify"; break;
    case DestroyNotify: n = "DestroyNotify"; break;
    case ConfigureNotify: n = "ConfigureNotify"; break;
    case ConfigureRequest: n = "ConfigureRequest"; break;
    case CirculateNotify: n = "CirculateNotify"; break;
    case SelectionClear: n = "SelectionClear"; break;
    case SelectionRequest: n = "SelectionRequest"; break;
    case SelectionNotify: n = "SelectionNotify"; break;
    case ClientMessage: n = "ClientMessage"; break;
    case PropertyNotify: n = "PropertyNotify"; break;
    case MappingNotify: n = "MappingNotify"; break;
    case VisibilityNotify: n = "VisibilityNotify"; break;
    case GenericEvent: n = "GenericEvent"; break;
    case ReparentNotify: n = "ReparentNotify"; break;
 /*
  case Expose: n = ""; break;
  case Expose: n = ""; break;
  case Expose: n = ""; break;
  case Expose: n = ""; break;
  case Expose: n = ""; break;
  case Expose: n = ""; break;
  */
  //  default: n = "Unknown event type: "; n = n + FXString::value( ev.xany.type );
  }
  if( wid == 0 ) { wid = id; }
  if( win != NULL ) { t = win->Title( );}
  if( n != FXString::null ){ fxwarning( "[::Print_event] %s -> client( %d / %s) \n", n.text( ), (int)wid, t.text( )  ); }
}

FXchar* convert_str( const FXString &str );
void clear_string_buffer( FXArray<const FXchar*> *buffer );
FXbool parse_params( FXArray<const FXchar*> *buffer, const FXString &str, FXbool dump = false );

/*************************************************************************************************/
FXIMPLEMENT( FXWindowManager, FXApp, NULL, 0 );

//********************************************************************
//
// Constructor
//
//********************************************************************
FXWindowManager::FXWindowManager( )
               : FXApp( "Krypton", "FOX-DESKTOP" ), focusmodel( WM_FOCUS_SLOPPY ), current_workspace( 0 )
{

  // OpenGL Window support
  /*
  if( FXGLVisual::hasOpenGL( this ) ) {
    fxwarning("[FXWindowManager::FXWindowManager]: Test OpenGl \n" );
    FXVisual   *old_vis = getDefaultVisual( );
    FXGLVisual *gl_vis = new FXGLVisual( this, VISUAL_DOUBLE_BUFFER | VISUAL_SWAP_COPY | VISUAL_STEREO | VISUAL_BEST | VISUAL_FORCE );
    if( gl_vis == NULL ) { fxwarning( "[FXWindowManager::FXWindowManager]:   OpenGL visual none \n" ); }
    else {
      setDefaultVisual( gl_vis );
      if( old_vis == NULL ) { fxwarning( "[FXWindowManager::FXWindowManager]:   Old visual none \n" );}
      else {
        fxwarning( "[FXWindowManager::FXWindowManager]:   Destroy old visual \n " );
        delete old_vis;
        old_vis = NULL;
      }
    }
    fxwarning("[FXWindowManager::FXWindowManager]:   OpenGl %s \n", ( ( FXGLVisual::hasOpenGL( this ) ) ? "ITS supported" : "NOT supported" ) );
  }
  else { fxwarning("[FXWindowManager::FXWindowManager]:   OpenGl NOT supported \n" ); }
  */
  //cl_dispatch = new FXWindowDispatcher( this ); // Nefunkcni - zatim :-P

  netwindow = new FXMainWindow( ( FXApp* ) this, FXString::null, NULL, NULL, DECOR_NONE, 0, 0, 0, 0 );
  workspaces._make( this, 4 );

#ifdef DEBUG
  grabcount=0;
#endif
}


//********************************************************************
//
// Destructor
//
//********************************************************************
FXWindowManager::~FXWindowManager( )
{
  delete netwindow;

  for( FXint i = 0; i < workspaces.no( ); i++ ) { delete workspaces[ i ]; }
  workspaces.clear( );

  delete rootwindowproperty;
}


//********************************************************************
//
// Create
//
//********************************************************************
void FXWindowManager::create()
{
  /// EWMH: https://specifications.freedesktop.org/wm-spec/wm-spec-1.3.html
  Window netid, rootid;

  FXApp::create( );
  netid  = netwindow->id( );
  rootid = this->getRootWindow( )->id( );

  /// POUZE PRO LADICI UCELY !!!!
  XSynchronize( DISPLAY, true );

  XSelectInput( DISPLAY, rootid, SubstructureRedirectMask | SubstructureNotifyMask | EnterWindowMask | KeyPressMask | KeyReleaseMask | PropertyChangeMask | ColormapChangeMask | ButtonPressMask | ButtonReleaseMask );
  XSetErrorHandler( ( XErrorHandler ) fxerrormanager );
  XSync( DISPLAY, true );

  FXAtom::init( this );

  /// Setup NET Support
  FXAtomList net_support;
  net_support.append( FXAtom::_NET_SUPPORTING_WM_CHECK );
  net_support.append( FXAtom::_NET_CLIENT_LIST );
  net_support.append( FXAtom::_NET_CLIENT_LIST_STACKING );
  net_support.append( FXAtom::_NET_WM_NAME );
  net_support.append( FXAtom::_NET_WM_STATE );
  net_support.append( FXAtom::_NET_CLOSE_WINDOW );
  net_support.append( FXAtom::_NET_WM_STATE_MODAL );
  net_support.append( FXAtom::_NET_WM_STATE_STICKY );
  net_support.append( FXAtom::_NET_WM_STATE_MAXIMIZED_VERT );
  net_support.append( FXAtom::_NET_WM_STATE_MAXIMIZED_HORZ );
  net_support.append( FXAtom::_NET_WM_STATE_SHADED );
  net_support.append( FXAtom::_NET_WM_STATE_SKIP_TASKBAR );
  net_support.append( FXAtom::_NET_WM_STATE_SKIP_PAGER );
  net_support.append( FXAtom::_NET_WM_STATE_HIDDEN );
  net_support.append( FXAtom::_NET_WM_STATE_FULLSCREEN );
  net_support.append( FXAtom::_NET_WM_STATE_ABOVE );
  net_support.append( FXAtom::_NET_WM_STATE_BELOW );
  net_support.append( FXAtom::_NET_WM_WINDOW_TYPE );
  net_support.append( FXAtom::_NET_WM_WINDOW_TYPE_DESKTOP );
  net_support.append( FXAtom::_NET_WM_WINDOW_TYPE_DOCK );
  net_support.append( FXAtom::_NET_WM_WINDOW_TYPE_TOOLBAR );
  net_support.append( FXAtom::_NET_WM_WINDOW_TYPE_MENU );
  net_support.append( FXAtom::_NET_WM_WINDOW_TYPE_UTILITY );
  net_support.append( FXAtom::_NET_WM_WINDOW_TYPE_SPLASH );
  net_support.append( FXAtom::_NET_WM_WINDOW_TYPE_DIALOG );
  net_support.append( FXAtom::_NET_WM_WINDOW_TYPE_NORMAL );
  net_support.append( FXAtom::_NET_WM_STRUT );
  net_support.append( FXAtom::_NET_WM_STRUT_PARTIAL );
  net_support.append( FXAtom::_KDE_NET_WM_WINDOW_TYPE_OVERRIDE );
  //net_support.append( FXAtom::_NET_WM_USER_TIME_WINDOW );
  net_support.append( FXAtom::_NET_WM_USER_TIME );

  //Nastaveni indikace aktivniho wm a podporovanych vlastnosti, nekonec nazvu WM do NET okna dle specifikace EWMH
  rootwindowproperty = new FXWindowProperty( this, rootid );
  rootwindowproperty->setWindow( FXAtom::_NET_SUPPORTING_WM_CHECK, netid );
  rootwindowproperty->setAtomList( FXAtom::_NET_SUPPORTED, net_support );

  netwindowproperty = new FXWindowProperty( this, netid );
  netwindowproperty->setWindow( FXAtom::_NET_SUPPORTING_WM_CHECK, netid );
  netwindowproperty->setUTF8String(FXAtom::_NET_WM_NAME, "Krypton");


//   XChangeProperty(DISPLAY,getRootWindow()->id(),xa_atoms[_NET_SUPPORTING_WM_CHECK],XA_WINDOW,32,PropModeReplace,(unsigned char *)&netid,1);
//   XChangeProperty(DISPLAY,getRootWindow()->id(),xa_atoms[_NET_SUPPORTED],XA_ATOM,32,PropModeReplace,(unsigned char *)&net_support,ARRAYNUMBER(net_support));
//   XChangeProperty( DISPLAY, netid, xa_atoms[ FXAtom::_NET_SUPPORTING_WM_CHECK ], XA_WINDOW, 32, PropModeReplace, ( unsigned char * ) &netid, 1 );
//   XChangeProperty(DISPLAY,netid,xa_atoms[FXAtom::_NET_WM_NAME],xa_atoms[FXAtom::UTF8_STRING],8,PropModeReplace,(unsigned char *)wmfox.text(),wmfox.length());

/*
  // Settings the XMessagesEvent dispatcher
  cl_dispatch->message_register( FXAtom::_NET_ACTIVE_WINDOW,    FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE ) );
  cl_dispatch->message_register( FXAtom::_NET_CLOSE_WINDOW,     FXSEL( SEL_COMMAND, FXClientWindow::ID_CLOSE_WINDOW ) );
  cl_dispatch->message_register( FXAtom::_NET_WM_STATE,         FXSEL( SEL_COMMAND, FXClientWindow::ID_STATE ) );
  cl_dispatch->message_register( FXAtom::WM_CHANGE_STATE,       FXSEL( SEL_COMMAND, FXClientWindow::ID_MINIMIZE ) );
  cl_dispatch->message_register( FXAtom::_NET_DESKTOP_VIEWPORT, FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE ) );
*/
  // Show the window manager on XServer side
  netwindow->show( PLACEMENT_SCREEN );
  workspaces( )->show( );

  // Detekce a notifikce vlastnosti pracovni(ch) ploch(y)
  notifyWorkspaces( );

  std::cout << "[ICCCM Support] Root window ID: " << ( int ) rootid << std::endl;
  std::cout << "[EWMH Support ] Net window ID : " <<  ( int ) netid << std::endl;

  Autostart( );
}



//********************************************************************
//
// Given a Window ID, returns pointer to client window.
// Returns NULL if Window is not handled by us.
//
//********************************************************************
FXClientWindow* FXWindowManager::getClientWindow( FXID wnd )
{
  FXWindow *window = findWindowWithId( wnd );
  if( window == NULL ){ return NULL; }
  else if( window->isMemberOf( FXMETACLASS( FXClientWindow ) ) ){
    return dynamic_cast<FXClientWindow*>( window );
  }
  else if( window->getParent( ) && window->getParent( )->isMemberOf( FXMETACLASS( FXClientWindow ) ) ) {
    return dynamic_cast<FXClientWindow*>( window->getParent( ) );
  }
  else { return NULL; }
}

FXClientWindow* FXWindowManager::findClientWindow( FXID win_id )
{
   FXClientWindow *resh = NULL;
   FXID    currentid;
   FXID    rootid;
   FXID    parentid;
   FXID*   childsidlist;
   FXuint  childsnum;

   currentid = win_id;
   while( resh == NULL ) {
     XQueryTree( (Display*) getDisplay( ), currentid, &rootid, &parentid, &childsidlist, &childsnum );
     currentid = parentid;
     //std::cout << "[FXWindowManager::findClientWindow ] Find window ID : " <<  ( int ) parentid << std::endl;
     //std::cout << "[FXWindowManager::findClientWindow ] Find window RootID : " <<  ( int ) rootid << std::endl;
     //std::cout.flush( );

     if( ( currentid == rootid ) || ( currentid <= 0 ) ) { break; }
     else { resh = getClientWindow( currentid ); }
   }
   return resh;
}

FXClientWindow* FXWindowManager::getEventWindow( FXRawEvent &event )
{
  // Inspirated by KarmenWM
  FXID winid;
  FXClientWindow *resh = NULL;

  switch( event.type ) {
    case ConfigureRequest : winid = event.xconfigurerequest.window; break;
    case MapRequest       : winid = event.xmaprequest.window; break;
    default               : winid = event.xany.window;
  }

  return getClientWindow( winid );
}

bool FXWindowManager::dispatchEvent( FXRawEvent &ev )
{
  //FXClientWindow *window = getClientWindow( ev.xmaprequest.window );
  FXClientWindow *window = getEventWindow( ev );
  XWindowChanges  wc;

  workspaces( )->handle( this, FXSEL( SEL_COMMAND, WMStack::ID_REPAINT ), &ev );
  Print_event( ev, (int)ev.xmaprequest.window, window );
  switch(ev.xany.type) {
    case ConfigureRequest :
      if( window ) {
        if( !hasDestroyEvent( ev.xconfigurerequest.window ) && !hasUnmapEvent( ev.xconfigurerequest.window ) ) {
          window->doConfigure( ev );
        }
      }
      else {
        if( !hasDestroyEvent( ev.xconfigurerequest.window ) )
        {
          //fxwarning("[Configure SEND %d]\n",(int)ev.xconfigurerequest.window);
          wc.x            = ev.xconfigurerequest.x;
          wc.y            = ev.xconfigurerequest.y;
          wc.width        = ev.xconfigurerequest.width;
          wc.height       = ev.xconfigurerequest.height;
          wc.sibling      = ev.xconfigurerequest.above;
          wc.stack_mode   = ev.xconfigurerequest.detail;
          wc.border_width = ev.xconfigurerequest.border_width;
          XConfigureWindow( DISPLAY, ev.xconfigurerequest.window, ev.xconfigurerequest.value_mask, &wc );
        }
      }
      return true;
      break;

    case MapRequest: // Namapovat, nebo pridat okno
      if( window ) { window->handle( this, FXSEL( SEL_COMMAND, FXClientWindow::ID_RESTORE ), NULL ); }
      else { client_insert( ev.xmaprequest.window ); }
      return true;


    case UnmapNotify:
      // skryt a odmapovat okno
      if( ( ev.xunmap.window != ev.xunmap.event ) && ev.xunmap.send_event ) { return true; }
      /*
      if( window && window->isEventForClient( ev.xdestroywindow.window ) ) {
        client_remove( window, ev.xunmap.window );
        return true;
      } /*
      if( window->isHidden( ) == false ) {
        window->hide( );
        return true;
      }*/
      break;

    case EnterNotify:
      if( ev.xcrossing.mode == NotifyGrab ) { return true; }
      if( window ) {
        if( focusmodel == WM_FOCUS_SLOPPY ) { window->doEnter( ev ); }
      }
      else {
      	return FXApp::dispatchEvent(ev);
//      if (focusmodel==WM_FOCUS_SLOPPY)
//          XSetInputFocus((Display*)getDisplay(),getRootWindow()->id(),RevertToPointerRoot,CurrentTime);
      }
      return true;
      break;
/*
    case CreateNotify:
      return true;
      break;
*/
    case DestroyNotify: // Okno je zavreno
      if( window && window->isEventForClient( ev.xdestroywindow.window ) ) {
        client_remove( window, ev.xunmap.window );
        return true;
      }
      break;

    case ResizeRequest:
      if (window){ }
      return true;
      break;

    case PropertyNotify:
      fxwarning( "[FXWindowManager::dispatchEvent  ==  ]: Client Xproperty atom %s - %s \n", XGetAtomName( ( Display*) getDisplay( ), ev.xclient.message_type ), XGetAtomName( ( Display*) getDisplay( ), ev.xproperty.atom ) );
      if( ev.xproperty.state != PropertyDelete ) {
        if( window ) {
          //fxwarning( "[FXWindowManager::dispatchEvent  +  ]: Client Xproperty atom %s - %s \n", XGetAtomName( ( Display*) getDisplay( ), ev.xclient.message_type ), XGetAtomName( ( Display*) getDisplay( ), ev.xproperty.atom ) );
          window->updateProperty( &ev );
        }
        else { dispatchOf( ev.xproperty.atom, getClientWindow( ev.xproperty.window ), NULL  ); }  //dispatchClient( ev ); }
      }
      return true;
      break;

    case CirculateRequest :
      if( window != NULL ) {
        if( ev.xcirculate.place == PlaceOnTop ) { window->above( ); }
        else { window->below( ); }
      }
      return FXApp::dispatchEvent( ev );
      break;

    case ClientMessage :
      /// FIXME : FXWindowManager_01
      if( dispatchClient( ev ) == false ) { return FXApp::dispatchEvent( ev ); }
      //if( cl_dispatch->dispatch( ev ) == false ) { return FXApp::dispatchEvent( ev ); }
      break;

    default:
      FXbool resh = FXApp::dispatchEvent(ev);
      if( window ) { sync( ); }
      return resh;
      break;
  }

   sync( );
   return false;
}

FXbool FXWindowManager::dispatchClient( FXRawEvent &event )
{
  /// FIXME : FXWindowManager_01
  /// Obsluha prime zadosti klienta ( ClientMessage event handle )
  long resh = 0;
  fxwarning( "[FXWindowManager::dispatchClient]: Client Message %s for %d \n", XGetAtomName( ( Display*) getDisplay( ), event.xclient.message_type ), ( int ) event.xclient.window );
  FXClientWindow *window = getClientWindow( event.xclient.window );
  Atom a = event.xclient.message_type;
  FXSelector selector = 0;

  if( ( window != NULL ) || ( event.xclient.window == getRootWindow( )->id( ) ) ) {
    //fxwarning( "[FXWindowManager::dispatchClient]: Client Message %s for %d / %s \n", XGetAtomName( ( Display*) getDisplay( ), event.xclient.message_type ), ( int ) event.xclient.window , window->Title( ).text( ) );
    //fxwarning( "%s \n", window->Title( ).text( )  );
    if( a == FXAtom::_NET_ACTIVE_WINDOW )    { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE );     }
    if( a == FXAtom::_NET_CLOSE_WINDOW )     { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_CLOSE_WINDOW ); }
    if( a == FXAtom::_NET_WM_STATE )         { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_STATE );        }
    if( a == FXAtom::WM_CHANGE_STATE)        { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_CHANGESTATE );  }
    if( a == FXAtom::_NET_DESKTOP_VIEWPORT ) { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE );     }
    if( a == FXAtom::_NET_WM_USER_TIME )     { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE );     }

    if( a == FXAtom::_NET_CURRENT_DESKTOP )  { resh =  switchWorkspace( event.xclient.data.l[ 0 ] ); }

  } else { /*fxwarning( "BAD WINDOW on id %d \n", ( int ) event.xclient.window );*/
    fxwarning( "[FXWindowManager::dispatchClient]: Client Message %s for %d - not request \n", XGetAtomName( ( Display*) getDisplay( ), event.xclient.message_type ), ( int ) event.xclient.window );
  }

  if( selector != 0 ) { resh = window->handle( this, selector, ( void* ) &event ); }
  return ( ( resh == 1 ) ? true : false );
}

FXbool FXWindowManager::dispatchOf( Atom a, FXClientWindow *window, void *data )
{
  long resh = 0;
  FXSelector selector = 0;

  if( window != NULL ) {
    fxwarning( "[FXWindowManager::dispatchOf]: Client Message %s for %d / %s \n", XGetAtomName( ( Display*) getDisplay( ), a ), ( int ) window->id( ) , window->Title( ).text( ) );

    if( a == FXAtom::_NET_ACTIVE_WINDOW )    { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE );     }
    if( a == FXAtom::_NET_CLOSE_WINDOW )     { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_CLOSE_WINDOW ); }
    if( a == FXAtom::_NET_WM_STATE )         { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_STATE );        }
    if( a == FXAtom::WM_CHANGE_STATE)        { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_CHANGESTATE );  }
    if( a == FXAtom::_NET_DESKTOP_VIEWPORT ) { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE );     }
    if( a == FXAtom::_NET_WM_USER_TIME )     { selector = FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE );     }

    if( selector != 0 ) { resh = window->handle( this, selector, NULL ); }
  }

  return ( ( resh == 1 ) ? true : false );
}

void FXWindowManager::sync( ) {
  XSync( DISPLAY, false );
}

void FXWindowManager::grab( )
{
#ifdef DEBUG
  grabcount++;
  if (grabcount==1)
    XGrabServer((Display*)getDisplay());
  else
    fxwarning("Server was already Grabbed\n");
#else
  //(DISPLAY);
  XGrabServer( DISPLAY );
#endif
}

void FXWindowManager::ungrab( )
{
#ifdef DEBUG
  grabcount--;
  if (grabcount==0)
    XUngrabServer((Display*)getDisplay());
  else
    fxwarning("Server was not Ungrabbed\n");
#else
  XUngrabServer( DISPLAY );
#endif
}


FXbool FXWindowManager::hasDestroyEvent( FXID wid )
{
  XEvent xevent;
  if( XCheckTypedWindowEvent( DISPLAY, wid, DestroyNotify, &xevent ) ) {
    XPutBackEvent( DISPLAY, &xevent );
    return true;
  }
  return false;
}

FXbool FXWindowManager::hasUnmapEvent( FXID wid )
{
  XEvent xevent;
  if( XCheckTypedWindowEvent( DISPLAY, wid, UnmapNotify, &xevent ) ) {
    XPutBackEvent( DISPLAY, &xevent );
    return true;
  }
  return false;
}

FXbool FXWindowManager::hasLeaveEvent( FXID wid )
{
  XEvent xevent;
  if( XCheckTypedWindowEvent( DISPLAY, wid, LeaveNotify, &xevent ) ) { return true; }
  return false;
}

FXbool FXWindowManager::isBadWindow(FXID winid)
{
  if( badwindow == None ) { return false; }
  if( badwindow == winid ) {
    badwindow = None;
    return true;
  }
  return false;
}


void FXWindowManager::notifyWorkspaces( )
{ // Notifikace parametru pracovni plochy
  FXint         numdesktops = workspaces.no( );
  FXIntegerList geometry;
  FXint viewport[ numdesktops ][ 2 ];  /// FIXME : numdesktops x 16

  // tzv. geometrie plochy - rozmery
  geometry.no( 2 );
  geometry[ 0 ] = getRootWindow( )->getWidth( );  // sirka plochy
  geometry[ 1 ] = getRootWindow( )->getHeight( ); // vyska plochy

  // Nastaveni tzv viewportu pro kazdou existujici plochu - pocatecnich souradnic
  for( FXint i = 0; i < numdesktops; i++ ) {
    viewport[ i ][ 0 ] = 0;
    viewport[ i ][ 1 ] = 0;
  }

  XChangeProperty( DISPLAY, getRootWindow( )->id( ), FXAtom::_NET_DESKTOP_VIEWPORT, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &viewport, numdesktops * 2 );
  rootwindowproperty->setCardinalList( FXAtom::_NET_DESKTOP_GEOMETRY, geometry );      // velikost pracovni plochy
  rootwindowproperty->setCardinal( FXAtom::_NET_NUMBER_OF_DESKTOPS, numdesktops );     // Pocet pracovnich ploch
  rootwindowproperty->setCardinal( FXAtom::_NET_CURRENT_DESKTOP, workspaces._current( ) );  // Aktualni pracovni plocha
}


void FXWindowManager::notifyClientList( Atom a )
{
  rootwindowproperty->setWindowList( a, clientids);
}

//********************************************************************
//
// This function constructs the FXClientWindow depending on if
// the right conditions are met.
//
//********************************************************************
void FXWindowManager::client_insert( FXID wid )
{
  fxmessage( "[FXWindowManager::client_insert]: BEGIN \n" );
  grab( );
  // Sync the Server (synchronizace serveru)
  sync( );
  // Check for any destroy and unmap events in the event queue
  //( Kontrola, zda jsou ve fronte nejake udalosti pro odstraneni, nebo odmapovani)
  if( hasUnmapEvent( wid ) || hasDestroyEvent( wid ) ) {
    ungrab( );
    return;
  }
  // Make sure Window Attributes are OK.
  // (Test, zda jsou atributy noveho okna koser)
  XWindowAttributes attrib;
  if( ( !XGetWindowAttributes( DISPLAY, wid, &attrib ) ) || ( !attrib.screen ) || attrib.override_redirect ) {
    ungrab( );
    return;
  }
  // Check if the initial state is WithDrawn
  // ( kontrola, zda pocatecni stav je WithDraw)
  XWMHints *hints = XGetWMHints( DISPLAY, wid );
  if( hints ) {
    if( hints->flags & StateHint ) {
      if( hints->initial_state == WithdrawnState ) {
        XFree( hints );
        ungrab( );
        return;
      }
    }
    XFree( hints );
  }
  // Vytvoreni klientskeho okna
  //FXClientWindow *client = new FXClientWindow( getRootWindow( ), wid, current_workspace );
  WMTopWindow *_win = new WMTopWindow( getRootWindow( ), wid, workspaces._current( ), workspaces.no( ) ); //current_workspace );

  // Inicializace indikacniho pole vitualnich ploch
  //FXint wnum = workspaces.no( );
  //_win->_status.no( wnum );
  //for( FXint i = 0; i != wnum; i++ ) { _win->_status[ i ] = false; }

  //fxmessage( "[FXWindowManager::client_insert]:  New client = %d : %d  \n", ( int ) wid, ( int ) client->id( ) );
  /*
    The save-set of a client is a list of other clients' windows that, if they are inferiors of one of the
    client's windows at connection close, should not be destroyed and should be remapped if they are unmapped.
    The X server automatically removes windows from the save-set when they are destroyed.
  */
  XAddToSaveSet( DISPLAY, wid );
  _win->create( );
  _win->show( );

  // Pridani instance klientskeho okna do stacku
  workspaces( )->addClient( _win );
  workspaces( )->handle( this, FXSEL( SEL_COMMAND, WMStack::ID_UPDATE ), NULL );
  ungrab( );

  // pridani klientova ID do seznamu mapovanych klientu a notifikace
  clientids.append( wid );
  notifyClientList( FXAtom::_NET_CLIENT_LIST );
  fxmessage( "[FXWindowManager::client_insert]: Client id %d; Topwindow id %d; Window title %s  OK\n", ( int ) wid, (int ) _win->id( ), _win->Title( ).text( ) );
  fxmessage( "[FXWindowManager::client_insert]: END \n" );

}
void FXWindowManager::client_append( FXClientWindow *window, FXint ws )
{
   std::cout << "APPEND the window: " << window->id( ) << std::endl;
   if( ws >= 0 && ws < workspaces.no( ) ) { workspaces[ ws ]->addClient( (WMTopWindow*) window ); }
   else {
     FXint act = 0;
     if( ws == -1 ) {
       for( FXint i = 0; i != workspaces.no( ); i++ ) {
            std::cout << "In virtual desktop " << i << " " << std::endl;
            workspaces[ i ]->addClient( (WMTopWindow*) window );
       }
     }
   }
}

void FXWindowManager::client_remove( FXClientWindow *window, FXID id )
{
  grab( );
  FXString t = window->Title( );

  // Odebrani klientskeho okna ze staku
  FXint ws = window->getWorkspace( );
  workspaces[ ws ]->removeClient( static_cast<WMTopWindow*>( window ) );
  workspaces[ ws ]->handle( this, FXSEL( SEL_COMMAND, WMStack::ID_UPDATE ), NULL );

  // Nalezeni a smazani zaznamu v seznamu mapovanych klientu
  for( FXint i = 0; i != clientids.no( ); i++ ) {
    if( clientids[ i ] == id ) {
      clientids.erase( i );
      break;
    }
  }

  // Skryti a uvolneni klientskeho okna z pameti
  if( window != NULL ) {
    window->hide();
    //XKillClient( ( Display* ) getDisplay( ), id );
    delete window;
    window = NULL;
  }

  // synchronizace  s XServerem
  sync( );
  ungrab( );

  // notifikace
  notifyClientList( FXAtom::_NET_CLIENT_LIST );
  //notifyClientList( FXAtom::_NET_CLIENT_LIST_STACKING );
  fxwarning( "[FXWindowManager::client_remove]: Client: %d; %s \n", ( int ) id, t.text( ) );
}

int FXWindowManager::switchWorkspace( FXint wid ) {
  if( workspaces.no( ) <= 0 ) { return 0; }
  if( workspaces._current( ) == wid ) { return 0; }
  if( wid >= 0 && wid < workspaces.no( ) ) {
    grab( );
    workspaces( )->hide( );
    sync( );

    fxwarning( "[FXWindowManager::dispatchClient]:Switch step 1: Clients = %d, Current WS = %d, Next WS = %d \n", clientids.no( ), workspaces._current( ), wid );
    workspaces._set( wid );
    for( FXint i = 0; i != clientids.no( ); i++ ) {
      fxwarning( "step 1.1 reasociate window cycle %d \n", i );
      FXClientWindow *win = getClientWindow( clientids[ i ] );
      if( win != NULL ) {
        FXbool igst = ( ( win->isWindowClass( CLASS_DESKTOP ) || win->isWindowClass( CLASS_DOCK ) ) ? false : true );
        workspaces.ReasocWin( win, wid, igst ); }
    }
    fxwarning( "[FXWindowManager::dispatchClient]:Switch step 2 \n" );

    workspaces( )->show( );
    workspaces( )->handle( this, FXSEL( SEL_COMMAND, WMStack::ID_UPDATE ), NULL );

    notifyWorkspaces( );
    sync( );
    ungrab( );

    return 1;
  }

  return 0;
}


/*************************************************************************************************/
void FXWindowManager::Autostart( )
{
  FXArray<const FXchar*> acmd, args( getArgv( ), getArgc( ));
  FXint num = args.no( );
  FXString prm = "-s";
  FXString cmd; //= FXSystem::getHomeDirectory( ) + "/.config/tinywm/startup";

  std::cout << "Arguments: " << std::endl;

  for( FXint i = 0; i != num; i++ ) {
    std::cout << i << ". " << args[ i ] << std::endl;
    if( prm == args[ i ] ) {
      FXint aut_id = i + 1;
      std::cout << "Autostart running: " << aut_id << "/" << args[ aut_id ] << std::endl;
      acmd.append( args[ aut_id ] );
      acmd.append( NULL );
      Exec( acmd );
      break;
    }
  }
}

FXbool FXWindowManager::Exec( const FXArray<const FXchar*> &args, FXbool wait, FXbool ver )
{
  /// Cesta k spoustenemu souboru (args[0]) nesmi zacinat a koncit mezerou (" ")
  /// Cesta ke spoustenemu souboru musi byt absolutni (tedy od korenoveho adresare)
  /// Navratovy kod spousteneho procesu lze zatim ziskat pouze pri wait = true
  /// Hodnota true parametru wait zablokuje celou aplikaci launcheru!

  FXbool    resh   = false;
  FXint     pid    = 0;
  FXint     status = 0;
  FXProcess proc;

  if( ( resh = proc.start( args[ 0 ], args.data( ) ) ) == true ) {
    pid = proc.id( );
    if( wait == true ) {
      proc.wait( status );
      resh = ( ( status != 0 ) ? false : true );
    }
  }

  if( ver == true ) {
    std::cout << "Run the process:";
    FXint num = args.no( );
    for( FXint i = 0; i != ( num - 1 ); i++ ) { std::cout << " " << args[ i ]; }
    std::cout << "\n";
    if( resh == true ) {
      std::cout << "process running of " << pid << std::endl;
      std::cout << "Process exited with " << status << " exit status" << std::endl;
    }
    else { std::cout << "RUN FATAL ERROR: Process is not running!" << std::endl;}
    std::cout << "\n";
    std::cout.flush( );
  }

  return resh;
}



/*************************************************************************************************/
/* Pomocne funkce pro interni operace tridy                                                      */
/*************************************************************************************************/
FXchar* convert_str( const FXString &str ) { // Konverze retezce FXString na FXchar[] vcetne alokace pameti
  if( !str.empty( ) ) {
    FXchar *c_str = new FXchar[ str.length( ) + 1 ];
    return strcpy( c_str, str.text( ) );
  }
  return NULL;
}

void clear_string_buffer( FXArray<const FXchar*> *buffer ) { // Dealokace retezcu v bufferu, alokovanych pomoci convert_str( )
  FXint num;
  if( ( num = buffer->no( ) ) > 0 ) {
    for( FXint i = 0; i != num; i++ ) {
       const FXchar *data = buffer->at( i );
       if( data != NULL ) { delete[] data; }
    }
    buffer->clear( );
  }
}

FXbool parse_params( FXArray<const FXchar*> *buffer, const FXString &ar, FXbool dump ) { // Rozparsovani retezce argumentu
  /// Parametry se oddeluji znakem mezery (" ")
  /// Je-li to vyzdovano aplikaci musi byt dodreno i jejich poradi
  /// mezera za, nebo pred strednikem se predava jako soucast parametru
  /// Specialni znaky ( napr uvozovky) je nutno uvadet v zastupne notaci dle XML standardu
  FXint    start, nargs;  // Aktualni sekce, ktera predstavuje argument; Pocet parametru v retezci
  FXString section_str;   // Subsekce retezce

  if( !ar.empty( ) && ( ar != "" ) ) {
    nargs = ar.contains( " " );
    if( nargs > 0 ) {
      nargs++;
      start = 0;
      while( start < nargs ){
        section_str = ar.section( " ", start );
        if( !section_str.empty( ) ) {
          buffer->append( convert_str( section_str ) );
          start++;
        } else { break; }
      }
    }
    else {
      start = nargs = 1;
      buffer->append( convert_str( ar ) );
    }
  }

  if( dump == true ) {
    FXint num = buffer->no( );
    std::cout << "This parameter(s) setting of new process: ";
    for( FXint i = 0; i != num; i++  ) { std::cout << "\n" << i << ". " << buffer->at( i ) << " " ; }
    std::cout << "\n" << std::endl;
    std::cout.flush( );
  }

  return ( ( nargs == start ) ? true : false );
}



