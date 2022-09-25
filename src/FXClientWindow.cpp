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
// Standard libs
#include<new>
#include<iostream>
#include<fstream>
#include<limits>

// Fox library
#include<xincs.h>
#include<fx.h>
//#include<FXArray.h>

// FXWM
#include<FXAtom.h>
#include<FXWindowManager.h>
#include<FXWorkspace.h>
#include<WMStack.h>
#include<FXWindowProperty.h>
#include<WMDecoration.h>
#include<WMFrameButton.h>
#include<FXClientWindow.h>

#define BASIC_EVENT_MASK   (StructureNotifyMask|ExposureMask|PropertyChangeMask|EnterWindowMask|LeaveWindowMask|KeyPressMask|KeyReleaseMask)
#define ENABLED_EVENT_MASK (ButtonPressMask|ButtonReleaseMask|PointerMotionMask)
#define MANAGED_EVENT_MASK (SubstructureNotifyMask|ResizeRedirectMask|SubstructureRedirectMask)
#define NOT_PROPAGATE_MASK (KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|ButtonMotionMask)


#define MWM_HINTS_FUNCTIONS	(1L << 0)       // Definitions for FXMotifHints.flags
#define MWM_HINTS_DECORATIONS	(1L << 1)
#define MWM_HINTS_INPUT_MODE	(1L << 2)
#define MWM_HINTS_ALL           (MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS|MWM_HINTS_INPUT_MODE)

#define MWM_FUNC_ALL		(1L << 0)       // Definitions for FXMotifHints.functions
#define MWM_FUNC_RESIZE		(1L << 1)
#define MWM_FUNC_MOVE		(1L << 2)
#define MWM_FUNC_MINIMIZE	(1L << 3)
#define MWM_FUNC_MAXIMIZE	(1L << 4)
#define MWM_FUNC_CLOSE		(1L << 5)

#define MWM_DECOR_ALL		(1L << 0)       // Definitions for FXMotifHints.decorations
#define MWM_DECOR_BORDER	(1L << 1)
#define MWM_DECOR_RESIZEH	(1L << 2)
#define MWM_DECOR_TITLE		(1L << 3)
#define MWM_DECOR_MENU		(1L << 4)
#define MWM_DECOR_MINIMIZE	(1L << 5)
#define MWM_DECOR_MAXIMIZE	(1L << 6)

#define MWM_INPUT_MODELESS		    0   // Values for FXMotifHints.inputmode
#define MWM_INPUT_PRIMARY_APPLICATION_MODAL 1
#define MWM_INPUT_SYSTEM_MODAL		    2
#define MWM_INPUT_FULL_APPLICATION_MODAL    3

#define DISPLAY ((Display*)((getApp())->getDisplay()))


FXDEFMAP(FXClientWindow) FXClientWindowMap[]={
  FXMAPFUNC( SEL_PAINT,             0, FXClientWindow::onPaint          ),
  FXMAPFUNC( SEL_LEFTBUTTONPRESS,   0, FXClientWindow::onLeftBtnPress   ),
  FXMAPFUNC( SEL_LEFTBUTTONRELEASE, 0, FXClientWindow::onLeftBtnRelease ),
  FXMAPFUNC( SEL_MOTION,            0, FXClientWindow::onMotion         ),
  FXMAPFUNC( SEL_ENTER,             0, FXClientWindow::onEnter          ),
  FXMAPFUNC( SEL_LEAVE,             0, FXClientWindow::onLeave          ),
  FXMAPFUNC( SEL_FOCUSIN,           0, FXClientWindow::onFocusIn        ),
  FXMAPFUNC( SEL_FOCUSOUT,          0, FXClientWindow::onFocusOut       ),

  FXMAPFUNC( SEL_COMMAND, FXClientWindow::ID_ACTIVATE,     FXClientWindow::onCmdActivate    ),
  FXMAPFUNC( SEL_COMMAND, FXClientWindow::ID_STATE,        FXClientWindow::onCmdState       ),
  FXMAPFUNC( SEL_COMMAND, FXClientWindow::ID_CHANGESTATE,  FXClientWindow::onCmdState       ),
  FXMAPFUNC( SEL_COMMAND, FXClientWindow::ID_MINIMIZE,     FXClientWindow::onCmdState       ),
  FXMAPFUNC( SEL_COMMAND, FXClientWindow::ID_MAXIMIZE,     FXClientWindow::onCmdState       ),
  FXMAPFUNC( SEL_COMMAND, FXClientWindow::ID_RESTORE,      FXClientWindow::onCmdState       ),
  FXMAPFUNC( SEL_COMMAND, FXWindow::ID_MDI_RESTORE,        FXClientWindow::onCmdState       ),
  FXMAPFUNC( SEL_COMMAND, FXWindow::ID_MDI_MINIMIZE,       FXClientWindow::onCmdState       ),
  FXMAPFUNC( SEL_COMMAND, FXWindow::ID_MDI_MAXIMIZE,       FXClientWindow::onCmdState       ),
  FXMAPFUNC( SEL_COMMAND, FXClientWindow::ID_CLOSE_WINDOW, FXClientWindow::onCmdCloseWindow ),
  FXMAPFUNC( SEL_COMMAND, FXWindow::ID_MDI_CLOSE,          FXClientWindow::onCmdCloseWindow ),

  FXMAPFUNC(SEL_UPDATE,FXWindow::ID_MDI_RESTORE,FXClientWindow::onUpdRestore),
  FXMAPFUNC(SEL_UPDATE,FXWindow::ID_MDI_MAXIMIZE,FXClientWindow::onUpdMaximize),
//  FXMAPFUNC(SEL_UPDATE,FXWindow::ID_MDI_CLOSE,FXClientWindow::onCmdCloseWindow),


  };


FXIMPLEMENT(FXClientWindow,FXComposite,FXClientWindowMap,ARRAYNUMBER(FXClientWindowMap));

/////////////////////////////////////////////////
/// Construct - Destruct Methods

FXClientWindow::FXClientWindow( )
              : FXComposite( )
{
  flags |= FLAG_ENABLED|FLAG_SHOWN;

  font              = NULL;
  cid               = 0;
  client            = NULL;
  xproperty         = NULL;
  desktop           = 0;

  wm_delete_window  = false;
  wm_take_focus     = false;
  dragmode          = DRAG_NONE;
  decor             = DECOR_NONE;
  decor_ref         = DECOR_NONE;

  titlesize         = 15;
  bordersize        = 3;
  group             = None;
  property          = 0;
  state             = 0;
  wpriority         = 0;

  transientFor      = NULL;
//  next_window       = NULL;
//  previous_window   = NULL;
//  transient_windows = NULL;
}

FXClientWindow::FXClientWindow( FXComposite *p, FXID clientid, FXint ws )
              : FXComposite( p, 0, 0, 0, 0, 0 )
{

  flags |= FLAG_ENABLED | FLAG_SHOWN;

  cid              = clientid;
  client           = NULL;
  workspace        = ws;
  desktop          = 0;

  titlesize        = 0;
  bordersize       = 0;
  decor            = DECOR_NONE;
  decor_ref        = DECOR_NONE;

  group            = None;
  wm_delete_window = false;
  wm_take_focus    = false;
  dragmode         = DRAG_NONE;

  transientFor      = NULL;
//  next_window       = NULL;
//  previous_window   = NULL;
//  transient_windows = NULL;

  disp = static_cast<Display*>( getApp( )->getDisplay( ) );
  xproperty = new FXWindowProperty( ( FXWindowManager* ) getApp( ), cid );


  leftstrut   = 0;
  rightstrut  = 0;
  topstrut    = 0;
  bottomstrut = 0;

  wpriority = 0;
  property  = 0;
  state     = 0;

  deco = new WMDecoration( getApp( ) );
  title = "???";
  font  = getApp( )->getNormalFont( );


  XWindowAttributes attr;
  XGetWindowAttributes( ( Display* ) getApp( )->getDisplay( ), clientid, &attr );
  carea.set( attr.x, attr.y, attr.width, attr.height );
  c_bw    = attr.border_width;
  gravity = attr.win_gravity;
  FXTRACE( ( 70, "Window Attributes: %d %d %d %d %d\n", carea.x, carea.y, carea.w, carea.h, c_bw ) );


  setX( carea.x );
  setY( carea.y );
  setWidth( carea.w );
  setHeight( carea.h );


  decocache = new FXIconCache( getApp( ), "/usr/share/themes/RedmondXP/xfwm4/" );
  decocache->insert( "close-active.xpm" );
  decocache->insert( "close-inactive.xpm" );
  decocache->insert( "close-pressed.xpm" );
  decocache->insert( "hide-active.xpm" );
  decocache->insert( "hide-inactive.xpm" );
  decocache->insert( "hide-pressed.xpm" );
  decocache->insert( "maximize-active.xpm" );
  decocache->insert( "maximize-inactive.xpm" );
  decocache->insert( "maximize-pressed.xpm" );
  decocache->insert( "maximize-toggled-active.xpm" );
  decocache->insert( "maximize-toggled-inactive.xpm" );
  decocache->insert( "maximize-toggled-pressed.xpm" );


  if( decocache->find( "close-active.xpm") ) {std::cout << "EXIST close-active" << std::endl; }
//  mdimenu=new FXMenuPane(this);

//  new FXMenuCommand(mdimenu,"Test",NULL);
  mdimenu = new FXMDIMenu( this, this );

  //closebutton    = new FXMDIDeleteButton(   this, this,    ID_CLOSE_WINDOW,FRAME_RAISED );
  closebutton    = new WMFrameButton( this, "close", decocache, this, ID_CLOSE_WINDOW, FRAME_NONE );
  //maximizebutton = new FXMDIMaximizeButton( this, this,    ID_MAXIMIZE,FRAME_RAISED );
  maximizebutton = new WMFrameButton( this, "maximize", decocache, this, ID_MAXIMIZE, FRAME_NONE );
  //minimizebutton = new FXMDIMinimizeButton( this, this,    ID_MINIMIZE, FRAME_RAISED );
  minimizebutton = new WMFrameButton( this, "hide", decocache, this, ID_MINIMIZE, FRAME_NONE );
  //( (WMFrameButton*) minimizebutton )->loadData( decocache );
  //restorebutton  = new FXMDIRestoreButton(  this, this,    ID_RESTORE,FRAME_RAISED );
  restorebutton  = new WMFrameButton( this, "maximize-toggled", decocache, this, ID_RESTORE, FRAME_NONE );
  menubutton     = new FXMDIWindowButton(   this, mdimenu, 0 );



}

FXClientWindow::~FXClientWindow( )
{
  if( client ) {
    /*
      The save-set of a client is a list of other clients' windows that, if they are inferiors of one of the
      client's windows at connection close, should not be destroyed and should be remapped if they are unmapped.
    */
    delete client;
    client = NULL;
  }
  delete xproperty;
  delete mdimenu;
}

void FXClientWindow::create( )
{
  FXComposite::create( );
  decocache->find( "close-active.xpm" )->create( );
  decocache->find( "close-inactive.xpm" )->create( );
  decocache->find( "close-pressed.xpm" )->create( );
  decocache->find( "hide-active.xpm" )->create( );
  decocache->find( "hide-inactive.xpm" )->create( );
  decocache->find( "hide-pressed.xpm" )->create( );
  decocache->find( "maximize-active.xpm" )->create( );
  decocache->find( "maximize-inactive.xpm" )->create( );
  decocache->find( "maximize-pressed.xpm" )->create( );
  decocache->find( "maximize-toggled-active.xpm" )->create( );
  decocache->find( "maximize-toggled-inactive.xpm" )->create( );
  decocache->find( "maximize-toggled-pressed.xpm" )->create( );


  font->create( );
  mdimenu->create( );

  FXint ts = FXMAX3( font->getFontHeight( ), closebutton->getDefaultHeight( ), menubutton->getDefaultHeight( ) ) + 6;

  XSelectInput( disp, xid, NoEventMask );

  //client = new FXWindow( ( FXComposite* ) this, 0, 0, 0, 0, 0 );
  client = new FXFrame( ( FXComposite* ) this, FRAME_NONE | LAYOUT_FILL, 0, 0, 0, 0,  0, 0, 0, 0 );
  client->attach( cid );

  /*
    Change the Input Mask for our window so we get events from our managed client.
    That way, we get a DestroyNotify/UnmapNotify.
  */
  XSelectInput( disp, xid, BASIC_EVENT_MASK | ENABLED_EVENT_MASK | MANAGED_EVENT_MASK );
  //XSelectInput(disp,cid,BASIC_EVENT_MASK|ENABLED_EVENT_MASK);
  XSelectInput( disp, cid, PropertyChangeMask | StructureNotifyMask | FocusChangeMask );


//  XSetWindowAttributes attrib_set;
//	attrib_set.event_mask=PropertyChangeMask | StructureNotifyMask | FocusChangeMask;
//	attrib_set.do_not_propagate_mask=ButtonPressMask | ButtonReleaseMask | ButtonMotionMask;
//	XChangeWindowAttributes(disp,cid,CWEventMask|CWDontPropagate,&attrib_set);

  transientFor = getTransientWindow();

  getDecorations( );
  getWMProtocols( );
  getWMHints( );
  getNETState( );
  getNETDesktop( );
  getNETIcon( );
  getNETStrut( );
  getWMNormalHints( );
  getTitle( );

  ( decor & DECOR_BORDER ) ? bordersize = 3 : bordersize = 0;
  ( decor & DECOR_TITLE  ) ? titlesize = ts : titlesize = 0;

  /// Make sure the size is within its bounds
  carea.w = FXCLAMP( wmin.w, carea.w, wmax.w );
  carea.h = FXCLAMP( wmin.h, carea.h, wmax.h );

  configure( );

  /*
  FXString LOGName = "/home/gabriel/tmp/Test/";
  LOGName += title;

  std::fstream fd( ( LOGName + ".win_log" ).text( ), std::fstream::in | std::fstream::out | std::fstream::trunc );
  fd << "CLIENT : WINDOW ID: " << ( int ) cid << " WINDOW TITLE: " << title.text( ) << "\n";
  fd << "==============================================================\n";
  fd << "[x, y] = " << carea.x << ", " << carea.y << "\n";
  fd << "[w, h] = " << carea.w << ", " << carea.h << "\n";
  FXString state_str = "";
  switch(state){
    case STATE_WITHDRAWN : state_str = "WITHDRAWN"; break;
    case STATE_SHADED    : state_str = "SHADED"; break;
    case STATE_MINIMIZED : state_str = "MINIMIZED"; break;
    case STATE_NORMAL    : state_str = "NORMAL"; break;
    case STATE_MAXIMIZED : state_str = "MAXIMIZED"; break;
    case STATE_FULLSCREEN: state_str = "FULLSCREEN"; break;
  }
  state_str = "State = " + state_str;
  FXString decor_str = ( ( ( decor&DECOR_BORDER ) == true ) ? "Yes" : "NO" );
  decor_str = "Decorations = " + decor_str;
  fd << state_str.text( ) << ", " << decor_str.text( ) << "\n";
  fd << "\n";
  fd << "==============================================================\n";
  fd.flush( );
  fd.close( );
  */
}

/*************************************************************************************************/
FXbool FXClientWindow::isEventForClient( FXID wnd )
{
   FXbool resulth = false;

   if( ( state != STATE_SHADED ) && ( state != STATE_MINIMIZED ) ) {
     resulth = ( cid == wnd );
   }
   return resulth;
}


void FXClientWindow::show( )
{
  fxwarning( "[ FXClientWindow::show( %d ) - BEGIN ] \n", ( int ) cid );
  if( shown( ) == false ) {
    if( state == STATE_WITHDRAWN ) { state = STATE_NORMAL; }
    FXComposite::show( );
    client->show( );
  }
  fxwarning( "[ FXClientWindow::show( %d ) - END ] \n", ( int ) cid );
}

void FXClientWindow::hide( FXuint _state )
{
  if( shown( ) == true ) {
    state = _state;
    FXComposite::hide( );
    //client->hide( );

    //notifyState( );
    //notifyDeactivate( );
  }
}

void FXClientWindow::sticky( )
{
  property |= PROPERTY_STICKY;
  //getWindowManager( )->client_append( this, -1 );
}

void FXClientWindow::skipPager( )
{
  property |= PROPERTY_SKIPPAGER;
  //getWindowManager( )->client_append( this, -1 );
}

void FXClientWindow::layout(){
  FXint x = c_bw; // Vychozi hodnoutou je sirak clientskeho ramu (odstup od okraje okna
  FXint y = c_bw;

  if( ( decor != DECOR_NONE ) && ( state != STATE_MINIMIZED && state != STATE_FULLSCREEN ) ) {
    // Vychozi sika tlacitka munu
    FXint mh = menubutton->getDefaultHeight( );
    FXint mw = menubutton->getDefaultWidth( );
    // Vychozi rozmery tlacitka uzavreni
    FXint bh = closebutton->getDefaultHeight( );
    FXint bw = closebutton->getDefaultWidth( );
    // Vypocet pozice pro umisteni prvniho tlacitka
    FXint bx = FXDrawable::width - bordersize - bw - 3;
    FXint by = bordersize + ( titlesize - bh ) / 2;

    if( decor & DECOR_CLOSE ) { // Tlacitko pro zavreni okna
      closebutton->position( bx, by, bw, bh ); // Nastveni pozice tlacitka
      bx -= ( bw + 2 );                        // Prepocet souradnic pro dalsi tlacitko
    }

    if ( decor & DECOR_MAXIMIZE ) { // Tlacitko pro maximalizaci (a obnovu) okna
      if( ( state == STATE_MAXIMIZED ) ) {         // Je-li okno maximalizovane
        maximizebutton->hide( );                   // Skryje se maximalizecni tlacitko
        restorebutton->position( bx, by, bw, bh ); // Nastavy se pozice tlacitka obnovy
        restorebutton->show( );                    // Zobrazi se
        bx -= ( bw + 2 );                          // Prepocet souradnic pro dalsi tlacitko
      }
      else {
        restorebutton->hide( );
        maximizebutton->position( bx, by, bw, bh );
        maximizebutton->show( );
        bx -= ( bw +2 );
      }
    }
    if( decor & DECOR_MINIMIZE ) { minimizebutton->position( bx, by, bw, bh ); } // Pozice minimalizacniho tlacitka

    if( decor & DECOR_MENU ) {
      menubutton->position( bordersize + 2, bordersize + ( titlesize - mh ) / 2, mw, mh );
    }
    /*
    /// FIXME : ODSTRANIT - Nevidim duvod proc to prave zde, kdyz tyto hodnoty se pouziji jen a pouze
    ///         tehdy, je-li viditelny klient
    // Prepocet pozice klienta
    x = x + bordersize;
    y = y + bordersize + titlesize;
    */
  }

  if( state != STATE_SHADED && state != STATE_MINIMIZED ) { // je-li zobrazen klient
    // Prepocet pozice klienta
    x = x + bordersize;
    y = y + bordersize + titlesize;
    client->position( x, y, carea.w, carea.h );
    notifyClient( );
  }
}

/*************************************************************************************************/
FXint FXClientWindow::getDefaultWidth(){
/*
  FXint bb=hasBorder() ? 0 : 3;
  FXint mw=menubutton->getDefaultWidth();
  FXint bw=closebutton->getDefaultWidth();
  FXint ww=mw+(bw*3)+bb+5;
  return ww;
*/
  return 50;
  }


FXint FXClientWindow::getDefaultHeight(){
/*
  FXint bb=hasBorder() ? 0 : 3;
  FXint mh=menubutton->getDefaultHeight();
  FXint bh=closebutton->getDefaultHeight();
  FXint hh=mh+(bh*3)+bb+5;
  return hh;
*/
  return 25;
}

/*************************************************************************************************/
void FXClientWindow::notifyClient( )
{
  //print_info( "notifyClient", FXString::null );
  XConfigureEvent ev;
  ev.type              = ConfigureNotify;
  ev.event             = cid;
  ev.window            = cid;
  ev.x                 = carea.x;
  ev.y                 = carea.y;
  ev.width             = carea.w;
  ev.height            = carea.h;
  ev.border_width      = c_bw;
  ev.above             = id( );
  ev.override_redirect = false;
  XSendEvent( disp /*( Display* )getApp( )->getDisplay( )*/, cid, false, StructureNotifyMask, ( XEvent* ) &ev );
}

void FXClientWindow::notifyActivate( )
{
  //print_info( "notifyActive", FXString::null );
  XChangeProperty( disp, getApp( )->getRootWindow()->id( ), FXAtom::_NET_ACTIVE_WINDOW, XA_WINDOW, 32, PropModeReplace, (unsigned char *) &cid, 1);

}

void FXClientWindow::notifyDeactivate( )
{
  //print_info( "notifyDeactive", FXString::null );
  FXID root_id  = getApp( )->getRootWindow()->id( );
  XChangeProperty( disp, root_id, FXAtom::_NET_ACTIVE_WINDOW, XA_WINDOW, 32, PropModeReplace, (unsigned char *) &root_id, 1);
}

void FXClientWindow::notifyFocus( )
{
  //print_info( "notifyFocus", FXString::null );
  XClientMessageEvent ev;
  ev.type             = ClientMessage;
  ev.send_event       = false;
  ev.display          = ( Display* ) getApp( )->getDisplay( );
  ev.window           = cid;
  ev.message_type     = FXAtom::WM_PROTOCOLS;
  ev.format           = 32;
  ev.data.l[ 0 ]      = FXAtom::WM_TAKE_FOCUS;
  ev.data.l[ 1 ]      = CurrentTime;
  XSendEvent( disp, cid, false, NoEventMask, ( XEvent* ) &ev );
}


void FXClientWindow::notifyClose( )
{
  //print_info( "notifyClose", FXString::null );
  if( wm_delete_window ) {
    XClientMessageEvent ev;
    ev.type             = ClientMessage;
    ev.display          = ( Display* ) getApp( )->getDisplay( );
    ev.window           = cid;
    ev.message_type     = FXAtom::WM_PROTOCOLS;
    ev.format           = 32;
    ev.data.l[ 0 ]      = FXAtom::WM_DELETE_WINDOW;
    ev.data.l[ 1 ]      = CurrentTime;
    XSendEvent( disp, cid, false, NoEventMask, ( XEvent* ) &ev );
  } else { XKillClient( ( Display* ) getApp( )->getDisplay( ), cid ); }
}


void FXClientWindow::notifyState(  )
{
  //print_info( "notifyState", FXString::null );
  FXAtomList    atomlist;
  FXIntegerList list;

  /// ICCCM state property
  list.no( 2 );
  list[ 0 ] = NormalState;
  list[ 1 ] = None;

  switch( state ){
    case STATE_WITHDRAWN  : list[ 0 ] = WithdrawnState; break;
    case STATE_SHADED     :
    case STATE_MINIMIZED  : list[ 0 ] = IconicState; break;
    case STATE_NORMAL     :
    case STATE_MAXIMIZED  :
    case STATE_FULLSCREEN : list[ 0 ] = NormalState; break;
  }
  xproperty->setValueList( FXAtom::WM_STATE, list );


  /// EWMH state property
  switch(state){
    case STATE_MAXIMIZED  : atomlist.append( FXAtom::_NET_WM_STATE_MAXIMIZED_VERT );
                            atomlist.append( FXAtom::_NET_WM_STATE_MAXIMIZED_HORZ );
                            break;
    case STATE_MINIMIZED  : atomlist.append( FXAtom::_NET_WM_STATE_HIDDEN ); break;
    case STATE_FULLSCREEN : atomlist.append( FXAtom::_NET_WM_STATE_FULLSCREEN ); break;
    case STATE_SHADED     : atomlist.append( FXAtom::_NET_WM_STATE_SHADED ); break;
  }
  if( property & PROPERTY_MODAL )     { atomlist.append( FXAtom::_NET_WM_STATE_MODAL) ;  }
  if( property & PROPERTY_STICKY )    { atomlist.append( FXAtom::_NET_WM_STATE_STICKY ); }
  if( property & PROPERTY_SKIPPAGER ) { atomlist.append( FXAtom::_NET_WM_STATE_SKIP_PAGER ); }

  xproperty->setAtomList( FXAtom::_NET_WM_STATE,atomlist );
}


//********************************************************************
//
// Paint Stuff
//
//********************************************************************
long FXClientWindow::onPaint( FXObject *, FXSelector, void *ptr){
  FXint xx, yy, titlespace, letters, dots, dotspace;
  if( state == STATE_CLOSE ) { return 0; }

  FXEvent *ev = ( FXEvent* ) ptr;
  FXDCWindow dc( this, ev );
  //  std::cout << "[FXClientWindow::onPaint( )] " << (int) cid << " - " << (int) id( ) <<  std::endl;
  FXRectangle myarea( ev->rect.x, ev->rect.y, ev->rect.w, ev->rect.h );
  FXbool focus = this->canFocus( );


  if( state == STATE_NORMAL || state == STATE_MAXIMIZED || state == STATE_SHADED )  {
    // Vykresleni oblasti okna
    deco->draw_content( &dc, myarea, focus );
    // Vykresleni ramu okna
    if( decor & DECOR_BORDER ) { deco->draw_frame( &dc, myarea, focus, bordersize );  }

    /// vykresleni titulkového pruhu a textu
    if ( decor & DECOR_TITLE ) {
      deco->draw_titlebar( &dc, FXRectangle( bordersize, bordersize, width - ( 2 * bordersize ), titlesize ), focus );

      FXint fh = font->getFontHeight( ) + 2;
      if( decor & DECOR_MENU ) { xx = bordersize + menubutton->getDefaultWidth( ) + 2 + 4; }
      else { xx = bordersize + 2 + 4; }

      yy = bordersize + font->getFontAscent( ) + ( titlesize - fh ) / 2;
      deco->draw_titletext( &dc, this->title, FXPoint( xx, yy ), focus );

    /*
    // Compute space for title
    titlespace = width - 3 * 12 - ( bordersize << 1 ) - 2 - 4 - 4 - 6 - 2;
    dots = 0;
    letters=title.length();

    // Title too large for space
    if( font->getTextWidth( title.text( ), letters ) > titlespace ) {
      dotspace = titlespace - font->getTextWidth( "...", 3 );
      while( letters > 0 && font->getTextWidth( title.text( ), letters ) > dotspace ) { letters--; }
      dots=3;
      if( letters == 0 ) {
        letters = 1;
        dots = 0;
      }
    }

    // Draw as much of the title as possible
    dc.setForeground( getApp( )->getSelforeColor( ) );
    dc.setFont( font );
    dc.drawText( xx, yy, title.text( ), letters );
    ///dc.drawText( xx + font->getTextWidth( title.text( ), letters ), yy, "...", dots );
    */
    }
  }
/*
    /// Pro testovaci ucely - odstranit!
    /// Vykresli cerveny ramecek kolem klienta
    FXint xt = bordersize + c_bw;
    FXint yt = bordersize + titlesize + c_bw;
    dc.setForeground( FXRGB( 255, 0, 0 ) );
    dc.drawRectangle( xt - 1, yt - 1, carea.w + 1, carea.h + 1 );
*/
  return 1;
}


void FXClientWindow::toClientCoordinates( FXint &x, FXint &y, FXint &w, FXint &h )
{
  x = x + bordersize;
  y = y + bordersize + titlesize;
  w = w - ( bordersize * 2 );
  h = h - ( bordersize * 2 ) - titlesize;
}


void FXClientWindow::toWindowCoordinates( FXint &x, FXint &y, FXint &w, FXint &h )
{
  x = x - bordersize;
  y = y - bordersize - titlesize;
  w = w + ( bordersize * 2 );
  h = h + ( bordersize * 2 ) + titlesize;
}

/* FIXME: DELETED 01
FXbool FXClientWindow::isDesktopWindow( )
{
  return ( ( property & PROPERTY_DESKTOP ) ? true : false );
}

FXbool FXClientWindow::isDockWindow( )
{
  return ( ( property & PROPERTY_DOCK ) ? true : false );
}

*/

void FXClientWindow::configure_class( /*FXint value*/ )
{



}


FXint FXClientWindow::getTopPriority( )
{
	FXint value = wpriority;

//	if( property & PROPERTY_DESKTOP ) { value = -1; }
//	if( property & PROPERTY_DOCK )    { value = 1;  }

	if( isWindowClass( CLASS_DESKTOP ) ) { value = -1; }
	if( isWindowClass( CLASS_DOCK ) )    { value += 1;  }
	if( state == STATE_FULLSCREEN )      { value += 2;  }

    FXString msg = "priority value = " + FXString::value( value );
    //print_info( "getTopPriority", msg );
    //fxwarning( "[FXClientWindow::getTopPriority] %d END \n", value );
	return value;
}

FXbool FXClientWindow::getNETHints( )
{
  FXAtomList list;
  /*
     The Client SHOULD specify window types in order of preference (the first being most preferable) but MUST include at least one of
     the basic window type atoms from the list below. This is to allow for extension of the list of types whilst providing default behavior
     for Window Managers that do not recognize the extensions.

     Start from the end of the list and work our way forward so we end up with the most desirable that we support.
  */
  //wclass = CLASS_NORMAL;
  if( xproperty->getAtomList( FXAtom::_NET_WM_WINDOW_TYPE, list ) ) {
    if(list.no( ) == 0 ) { return false; }
    for( FXint i = list.no( ) - 1; i >= 0; i-- ) {
      /*
      if      ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_DESKTOP )      { property |= PROPERTY_DESKTOP; decor = DECOR_NONE; type_text = "Desktop"; }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_TOOLBAR )      { property |= PROPERTY_TOOLBAR; decor = DECOR_NONE; type_text = "Toolbar"; }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_DOCK )         { property |= PROPERTY_DOCK;    decor = DECOR_NONE; type_text = "Dock"; }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_MENU )         { property |= PROPERTY_MENU;    decor = DECOR_NONE; type_text = "Menu"; }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_UTILITY )      { property |= PROPERTY_UTILITY; decor = DECOR_NONE; type_text = "Utility"; }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_SPLASH )       { property |= PROPERTY_SPLASH;  decor = DECOR_NONE; type_text = "Splash"; }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_DIALOG )       { property |= PROPERTY_DIALOG;  decor = ( DECOR_BORDER | DECOR_RESIZE | DECOR_TITLE | DECOR_CLOSE ); type_text = "Dialog"; }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_NORMAL )       { property |= PROPERTY_NORMAL;  decor = DECOR_ALL; type_text = "Normal"; }
      else if ( list[ i ] == FXAtom::_KDE_NET_WM_WINDOW_TYPE_OVERRIDE ) { decor = DECOR_NONE; type_text = "KDE type override"; } /// QT/KDE Invention... means no Decoration
      */
      if      ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_DESKTOP )      { wclass = CLASS_DESKTOP; decor = DECOR_NONE; type_text = "Desktop"; wpriority = -1; }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_TOOLBAR )      { wclass = CLASS_TOOLBAR; decor = DECOR_NONE; type_text = "Toolbar"; wpriority = 0;  }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_DOCK )         { wclass = CLASS_DOCK;    decor = DECOR_NONE; type_text = "Dock";    wpriority = 1;  }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_MENU )         { wclass = CLASS_MENU;    decor = DECOR_NONE; type_text = "Menu";    wpriority = 0;  }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_UTILITY )      { wclass = CLASS_UTILITY; decor = DECOR_NONE; type_text = "Utility"; wpriority = 0;  }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_SPLASH )       { wclass = CLASS_SPLASH;  decor = DECOR_NONE; type_text = "Splash";  wpriority = 0;  }
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_DIALOG )       { wclass = CLASS_DIALOG;  decor = ( DECOR_BORDER | DECOR_RESIZE | DECOR_TITLE | DECOR_CLOSE ); type_text = "Dialog"; wpriority = 0;}
      else if ( list[ i ] == FXAtom::_NET_WM_WINDOW_TYPE_NORMAL )       { wclass = CLASS_NORMAL;  decor = DECOR_ALL; type_text = "Normal"; wpriority = 0; }
      else if ( list[ i ] == FXAtom::_KDE_NET_WM_WINDOW_TYPE_OVERRIDE ) { decor = DECOR_NONE; type_text = "KDE type override"; wpriority = 0; } /// QT/KDE Invention... means no Decoration
      else { /*fxwarning( "FXClientWindow::getNETHints() - %d/%d] Please Report Bug - Unknown NET Hint: %s\n", ( int ) cid, ( int ) group, XGetAtomName( disp, list[ i ] ) ); */}

      //fxwarning( "[FXClientWindow::getNETHints( ) ] %d; %s - %s \n", ( int ) cid, title.text( ), XGetAtomName( disp, list[ i ]) );

      //print_info( "getNETHints", XGetAtomName( disp, list[ i ] ) , "WARNING" );
    }
    return true;
  }
  return false;
}

void FXClientWindow::getNETStrut( )
{
  FXIntegerList list;

  if( xproperty->getCardinalList( FXAtom::_NET_WM_STRUT_PARTIAL, list ) ) {
    if( list.no( ) == 12 ) {
      //fxwarning( "\tStrut:\n" );
      //fxwarning( "\tLeft:%d\n",           list[ 0 ] );
      //fxwarning( "\tRight:%d\n",          list[ 1 ] );
      //fxwarning( "\tTop:%d\n",            list[ 2 ] );
      //fxwarning( "\tBottom:%d\n",         list[ 3 ] );
      //fxwarning( "\tLeft start y:%d\n",   list[ 4 ] );
      //fxwarning( "\tLeft end y:%d\n",     list[ 5 ] );
      //fxwarning( "\tRight start y:%d\n",  list[ 6 ] );
      //fxwarning( "\tRight end y:%d\n",    list[ 7 ] );
      //fxwarning( "\tTop start x:%d\n",    list[ 8 ] );
      //fxwarning( "\tTop end x:%d\n",      list[ 9 ] );
      //fxwarning( "\tBottom start x:%d\n", list[ 10 ] );
      //fxwarning( "\tBottom end x:%d\n",   list[ 11 ] );

      leftstrut   = list[ 0 ];
      rightstrut  = list[ 1 ];
      topstrut    = list[ 2 ];
      bottomstrut = list[ 3 ];
      return;
    }
  }

  if( xproperty->getCardinalList( FXAtom::_NET_WM_STRUT, list ) ) {
    if( list.no( ) == 4 ) {
      //fxmessage( "\tStrut:\n");
      //fxwarning( "\t\tLeft:%d\n",  list[ 0 ] );
      //fxwarning( "\t\tRight:%d\n", list[ 1 ] );
      //fxwarning( "\t\tTop:%d\n",   list[ 2 ] );
      //fxwarning( "\t\tBottom:%d\n",list[ 3 ] );

      leftstrut   = list[ 0 ];
      rightstrut  = list[ 1 ];
      topstrut    = list[ 2 ];
      bottomstrut = list[ 3 ];
    }
  }
}


void FXClientWindow::getNETDesktop( )
{
  FXuint us, ws;

  if( xproperty->getCardinal( FXAtom::_NET_WM_DESKTOP, us ) ) {
    ws = us;
    if( ws == 0xFFFFFFFF ) { property |= PROPERTY_STICKY; }
    else { if ( ws != 0 ) { workspace = ws; } }
  }
}

FXbool FXClientWindow::getNETState( )
{
  FXAtomList list;
  FXint maxv = 0;
  FXint maxh = 0;

  if( xproperty->getAtomList( FXAtom::_NET_WM_STATE, list ) ) {
    for ( FXint i = 0; i < list.no( ); i++ ) {

      if      ( list[ i ] == FXAtom::_NET_WM_STATE_MODAL )             modal( ); //property |= PROPERTY_MODAL;
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_STICKY )            sticky( ); //property |= PROPERTY_STICKY;
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_MAXIMIZED_VERT )    maxv      = 1;
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_MAXIMIZED_HORZ )    maxh      = 1;
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_SHADED )            shade(); //state     = STATE_SHADED;
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_FULLSCREEN )        fullscreen( ); //{ state   = STATE_FULLSCREEN; decor = DECOR_NONE; }
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_HIDDEN )            iconify( ); //state     = STATE_MINIMIZED;
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_ABOVE )             above( );
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_BELOW )             below( );
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_SKIP_PAGER )        skipPager( );
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_SKIP_TASKBAR )      ;
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_DEMANDS_ATTENTION ) ;
      else if ( list[ i ] == FXAtom::_NET_WM_STATE_FOCUSED )           ;
      else {
        FXString warn = "Please Report Bug - Unknown NET State: ";
        warn += XGetAtomName( disp, list[ i ] );
        print_info( "getNETState", warn, "WARNING" );
        //fxwarning( "FXClientWindow::getNETState() - %d/%d] Please Report Bug - Unknown NET State: %s\n",( int ) cid, ( int ) group, XGetAtomName( disp, list[ i ] ) ) ;
      }
      /*
      switch( list[ i ] ) {
        case FXAtom::_NET_WM_STATE_MODAL             : property |= PROPERTY_MODAL; break;
        case FXAtom::_NET_WM_STATE_STICKY            : property |= PROPERTY_STICKY; break;
        case FXAtom::_NET_WM_STATE_MAXIMIZED_VERT    : maxv      = 1; break;
        case FXAtom::_NET_WM_STATE_MAXIMIZED_HORZ    : maxh      = 1; break;
        case FXAtom::_NET_WM_STATE_SHADED            : state     = STATE_SHADED; break;
        case FXAtom::_NET_WM_STATE_FULLSCREEN        : { state   = STATE_FULLSCREEN; decor = DECOR_NONE; break;}
        case FXAtom::_NET_WM_STATE_HIDDEN            : state     = STATE_MINIMIZED; break;
        case FXAtom::_NET_WM_STATE_ABOVE             : break;
        case FXAtom::_NET_WM_STATE_BELOW             : break;
        case FXAtom::_NET_WM_STATE_SKIP_PAGER        : break;
        case FXAtom::_NET_WM_STATE_SKIP_TASKBAR      : break;
        case FXAtom::_NET_WM_STATE_DEMANDS_ATTENTION : break;
        case FXAtom::_NET_WM_STATE_FOCUSED           : break;
        default : {
          FXString warn = "Please Report Bug - Unknown NET State: ";
          warn += XGetAtomName( disp, list[ i ] );
          print_info( "getNETState", warn, "WARNING" );
        }
      }*/
      //print_info( "getNETState", XGetAtomName( disp, list[ i ] ) );
      //fxwarning( "[FXClientWindow::getNETState() - %d/%d] %s \n", ( int ) cid, ( int ) group, XGetAtomName( disp, list[ i ]) );
    }
    if( maxv || maxh ) { maximize(); /*state = STATE_MAXIMIZED;*/ }
  }
  print_info( "getNETState", "OK" );
  return true;
}



//********************************************************************
//
// Get the _NET Icon
//
//********************************************************************
void FXClientWindow::getNETIcon( )
{
  FXbool hasIcon = false;
  FXIntegerList list;

  hasIcon = xproperty->getCardinalList( FXAtom::_NET_WM_ICON, list );
  if( hasIcon && list.no( ) ) {
	//fxwarning( "Size of first Icon: %d %d\n", list[ 0 ], list[ 1 ] );
	//fxwarning( "Predicted Size of Icon: %d\n", list[ 0 ] * list[ 1 ] );
	//fxwarning( "Items Left in List After first Icon: %d\n", list.no( ) - ( list[ 0 ] * list[ 1 ] ) -2 );

  	FXColor * pixeldata;
	FXMALLOC(&pixeldata,FXColor,list[0]*list[1]);

	for( int i = 0; i < ( int )( list[ 0 ] * list[ 1 ] ); i++ ) {
	  pixeldata[ i ] = FXRGBA( FXREDVAL(   list[ i + 2 ] ),
                               FXGREENVAL( list[ i + 2 ] ),
                               FXBLUEVAL(  list[ i + 2 ] ),
                               FXALPHAVAL( list[ i + 2 ] ) );
    }

	FXIcon *icon = new FXIcon( getApp( ), pixeldata, 0,
                               IMAGE_OWNED | IMAGE_ALPHAGUESS | IMAGE_SHMP | IMAGE_SHMI,
                               list[ 0 ], list [ 1 ] );
	icon->blend( getApp( )->getSelbackColor( ) );
	icon->scale( 16, 16 );
	icon->create( );

	menubutton->setIcon( icon );
  }
}



FXClientWindow* FXClientWindow::getTransientWindow( )
{
  FXID win;

  if( !XGetTransientForHint( ( Display* ) getApp( )->getDisplay( ), cid, &win ) ) { return NULL; }
  if( win == cid ) { return NULL; }
  if( win == getApp( )->getRootWindow( )->id( ) && win != 0 ) {
    //fxwarning("\tModal Window\n");
    property |= PROPERTY_MODAL;
    return NULL;
  }

  FXClientWindow *t = getWindowManager( )->getClientWindow( win );
  /*if( t ) { fxwarning("\tTransient for %d\n",(int)t->id()); }*/
  return t;
  }



//********************************************************************
//
// Get the Window Manager Hints. getWMProtocols should have been called first
//
//********************************************************************
void FXClientWindow::getWMHints()
{
  group=None;
  focusmode=FOCUSMODE_PASSIVE;

  XWMHints *hints = XGetWMHints((Display*)getApp()->getDisplay(),cid);
  if( hints ) {
    if ( hints->flags&WindowGroupHint){ group = hints->window_group; }
    if ( hints->flags&StateHint ) {
      switch( hints->initial_state ) {
        case WithdrawnState : state=STATE_WITHDRAWN;  /*fxwarning("\tInitial State: Withdrawn\n");*/ break;
        case NormalState    : state=STATE_NORMAL;     /*fxwarning("\tInitial State: Normal\n");*/ break;
        case IconicState    : state=STATE_MINIMIZED;  /*fxwarning("\tInitial State: Icon\n");*/ break;
        default             : /*fxwarning("\tInitial State: Unknown\n");*/ print_info( "getWMHints", "Initial State: Unknown", "WARNING" ); break;
        };
      }
    if (hints->flags&InputHint){
      if (wm_take_focus)
        (hints->input) ? focusmode=FOCUSMODE_PASSIVE : focusmode=FOCUSMODE_NONE;
      else
        (hints->input) ? focusmode=FOCUSMODE_LOCAL : focusmode=FOCUSMODE_GLOBAL;
      }
//#ifdef DEBUG
    switch(focusmode) {
      case FOCUSMODE_NONE   : /*fxwarning("\tFocus Mode: None\n");    */ break;
      case FOCUSMODE_PASSIVE: /*fxwarning("\tFocus Mode: Passive\n"); */ break;
      case FOCUSMODE_LOCAL  : /*fxwarning("\tFocus Mode: Local\n");   */ break;
      case FOCUSMODE_GLOBAL : /*fxwarning("\tFocus Mode: Globale\n"); */ break;
      };
//#endif
    XFree(hints);
    }
  //fxwarning("\tWindow Group: %d\n",(int)group);
  }


//********************************************************************
//
// Get the Window Manager Protocol Hints.
//
//********************************************************************
void FXClientWindow::getWMProtocols(){
  Atom *protocols;
  FXint n;

  if( XGetWMProtocols( ( Display* ) getApp( )->getDisplay( ), cid, &protocols, &n ) ) {
    for( FXint i = 0; i < n; i++ ) {
      if( protocols[ i ] == FXAtom::WM_DELETE_WINDOW )    { wm_delete_window = true; }
      else if ( protocols[ i ] == FXAtom::WM_TAKE_FOCUS ) { wm_take_focus    = true; }
    }
    XFree(protocols);
  }
}


//********************************************************************
//
// Get the Window Manager Normal Hints.
//
//********************************************************************
void FXClientWindow::getWMNormalHints(){
  long icccm_mask;
  XSizeHints hint;
  FXshort    __maxsizenum = std::numeric_limits<short>::max( );

  wmin.set( 0, 0 );
  wmax.set( __maxsizenum, __maxsizenum );
  wincr.set( 1, 1 );
  wbase.set( 0, 0 );
  amin.set( 1, 1 );
  amax.set( 1, 1 );
  gravity = NorthWestGravity;

  if( XGetWMNormalHints( ( Display* ) getApp( )->getDisplay( ), cid, &hint, &icccm_mask ) ) {
    if( hint.flags & PMinSize ) { wmin.set( FXMAX( 0, hint.min_width ),       FXMAX( 0,  hint.min_height ) ); }
    if( hint.flags & PMaxSize ) { wmax.set( FXMIN( __maxsizenum, hint.max_width ), FXMIN( __maxsizenum, hint.max_height ) ); }
    if( hint.flags & PResizeInc) { wincr.set( FXMAX( 1, hint.width_inc ),     FXMAX( 1, hint.height_inc ) ); }
    if( hint.flags & PAspect) {
      amin.set( FXMAX( 1, hint.min_aspect.x ), FXMAX( 1, hint.min_aspect.y ) );
      amax.set( FXMAX( 1, hint.max_aspect.x), FXMAX( 1, hint.max_aspect.y ) );
    }
    if( hint.flags & PBaseSize ) { wbase.set( hint.base_width, hint.base_height ); }
    if (hint.flags&PWinGravity) { gravity = hint.win_gravity; }
  }

  if( ( wmax.w == wmin.w ) && ( wmax.h == wmin.h ) ) { decor &=~DECOR_RESIZE; }
  if( ( wmax.w < getApp( )->getRootWindow( )->getWidth( ) ) || ( wmax.h < getApp( )->getRootWindow( )->getHeight( ) ) ) { decor &=~DECOR_MAXIMIZE; }
}


void FXClientWindow::calculateReferencePoint( )
 {
  FXString grw_name;

  if (state==STATE_MAXIMIZED){
    grw_name = "MAXIMG";
    ref_x=carea.x;
    ref_y=carea.y;
    return;
  }

  switch(gravity){
    case StaticGravity    : ref_x = carea.x;
                            ref_y = carea.y;
                            grw_name = "StaticGravity";
                            break;
    case NorthWestGravity : ref_x = carea.x - c_bw;
                            ref_y = carea.y - c_bw;
                            grw_name = "NorthWestGravity";
                            break;
    case NorthGravity     : ref_x = carea.x + ( carea.w / 2 );
                            ref_y = carea.y - c_bw;
                            grw_name = "NorthGravity";
                            break;
    case NorthEastGravity : ref_x = carea.x + carea.w + c_bw;
                            ref_y = carea.y - c_bw;
                            grw_name = "NorthEastGravity";
                            break;
    case EastGravity      : ref_x = carea.x + carea.w + c_bw;
                            ref_y = carea.y + ( carea.h / 2 );
                            grw_name = "EastGravity";
                            break;
    case SouthEastGravity : ref_x=carea.x+carea.w+c_bw;
                            ref_y=carea.y+carea.h+c_bw;
                            grw_name = "SouthEastGravity";
                            break;
    case SouthGravity     : ref_x=carea.x+(carea.w/2);
                            ref_y=carea.y+carea.h+c_bw;
                            grw_name = "SouthGravity";
                            break;
    case SouthWestGravity : ref_x=carea.x-c_bw;
                            ref_y=carea.y+carea.h+c_bw;
                            grw_name = "SouthWestGravity";
                            break;
    case WestGravity      : ref_x=carea.x-c_bw;
                            ref_y=carea.y+(carea.h/2);
                            grw_name = "WestGravity";
                            break;
    case CenterGravity    : ref_x=carea.x+(carea.w/2);
                            ref_y=carea.y+(carea.h/2);
                            grw_name = "CenterGravity";
                            break;
    };
    //fxwarning( "\tGravity: %s \n", grw_name.text( ) );
    //fxwarning( "\t  reference point [%d, %d] \n", ref_x, ref_y );
  }

void FXClientWindow::positionFrameWindow( )
{
  FXint x, y, w, h;

  x = carea.x;
  y = carea.y;
  w = carea.w + ( bordersize * 2 ) + ( c_bw * 2 );
  h = carea.h + ( bordersize * 2 ) + ( c_bw * 2 ) + titlesize;

  switch( gravity ) {
    case StaticGravity    :
    {
      x = ref_x - bordersize - c_bw;
      y = ref_y - bordersize - titlesize - c_bw;
      break;
    }
    case NorthWestGravity :
    {
      x = ref_x;
      y = ref_y;
      break;
    }
    case NorthGravity     :
    {
      x = ref_x - ( w / 2 );
      y = ref_y;
      break;
    }
    case NorthEastGravity :
    {
      x = ref_x - w;
      y = ref_y;
      break;
    }
    case EastGravity      :
    {
      x = ref_x - w;
      y = ref_y - ( h / 2 );
      break;
    }
    case SouthWestGravity :
    {
      x = ref_x;
      y = ref_y - h;
      break;
    }
    case SouthGravity     :
    {
      x = ref_x - ( w / 2 );
      y = ref_y - h;
      break;
    }
    case SouthEastGravity :
    {
      x = ref_x - w;
      y = ref_y - h;
      break;
    }
    case WestGravity      :
    {
      x = ref_x;
      y = ref_y - ( h / 2);
      break;
    }
    case CenterGravity    :
    {
      x = ref_x - ( w / 2 );
      y = ref_y - ( h / 2 );
      break;
    }
  };

  /// If we're maximized, use static gravity.
  if( state == STATE_MAXIMIZED ) {
    x = ref_x - bordersize - c_bw;
    y = ref_y - bordersize - titlesize - c_bw;
  }

  if( x < 0  ) {
    w = w + ( -1 * x );
    x = 0;
  }

  if( y < 0 ) {
    h = h + ( -1 * y );
    y = 0;
  }

  //fxwarning("\tFrame Coordinates - %d %d %d %d\n",x,y,w,h);
  position( x, y, w, h );
  }

void FXClientWindow::calculateGravity( )
{
  // Prevzato z KARMEN WM
  /*
  FXString grw_name;
  FXint x, y, w, h;
  FXbool north, south, east, west, stat, center;

  north = south = east = west = stat = center = false;
  switch(gravity){
    case StaticGravity    : stat     = true;
                            grw_name = "StaticGravity";
                            break;
    case NorthWestGravity : north    = true;
                            west     = true;
                            grw_name = "NorthWestGravity";
                            break;
    case NorthGravity     : north    = true;
                            grw_name = "NorthGravity";
                            break;
    case NorthEastGravity : north    = true;
                            east     = true;
                            grw_name = "NorthEastGravity";
                            break;
    case EastGravity      : east     = true;
                            grw_name = "EastGravity";
                            break;
    case SouthEastGravity : south    = true;
                            east     = true;
                            grw_name = "SouthEastGravity";
                            break;
    case SouthGravity     : south    = true;
                            grw_name = "SouthGravity";
                            break;
    case SouthWestGravity : south    = true;
                            west     = true;
                            grw_name = "SouthWestGravity";
                            break;
    case WestGravity      : west     = true;
                            grw_name = "WestGravity";
                            break;
    case CenterGravity    : center   = true;
                            grw_name = "CenterGravity";
                            break;
    default :               north    = true;
                            west     = true;
                            grw_name = "NorthWestGravity ( DEFAULT )";
  };
  //fxwarning( "\tGravity: %s \n", grw_name.text( ) );

  if      ( north  == true ) { y = carea.y; }
  else if ( south  == true ) { }
  else if ( center == true ) { }
  else if ( stat   == true ) { }
  else { y = carea.y; }

  if      ( west   == true ) { x = carea.x; }
  else if ( east   == true ) { }
  else if ( center == true ) { }
  else if ( stat   == true ) { }
  else { x = carea.x; }
  */
  //fxwarning( "\t  reference point [%d, %d] \n", x, y );
}

void FXClientWindow::getDecorations(){
  FXbool gothints=false;

  gothints = getNETHints();
  if (gothints) goto done;

  gothints = getMotifHints();
  if (gothints) goto done;

  /// We're so desperate... lets just do something
  decor = DECOR_ALL;

done:

  /// Remove certain hints when we're transient or modal
  if (transientFor || (property&PROPERTY_MODAL)){
    decor&=~(DECOR_MENU|DECOR_MAXIMIZE|DECOR_MINIMIZE);
    }
   decor_ref = decor;
  }

FXbool FXClientWindow::getMotifHints(){
  FXIntegerList list;

  if (xproperty->getValueList(FXAtom::_MOTIF_WM_HINTS,list)){
    FXuint flags = list[0];
    FXuint functions = list[1];
    FXuint decorations = list[2];

    /// Get the Decor Hints
    if (flags & MWM_HINTS_DECORATIONS) {
      if (decorations & MWM_DECOR_ALL){
        decor|=DECOR_ALL;
        }
      else {
        decor=0;
        if (decorations & MWM_DECOR_BORDER)
          decor|=DECOR_BORDER;
        if (decorations & MWM_DECOR_TITLE)
          decor|=DECOR_TITLE;
        if (decorations & MWM_DECOR_MENU)
          decor|=(DECOR_MENU|DECOR_TITLE);
        if (decorations & MWM_DECOR_MINIMIZE)
          decor|=(DECOR_MINIMIZE|DECOR_TITLE);
        if (decorations & MWM_DECOR_MAXIMIZE)
          decor|=(DECOR_MAXIMIZE|DECOR_TITLE);
        if (decorations & MWM_DECOR_RESIZEH)
          decor|=DECOR_RESIZE;
        }
      }

    /// Get the Function Hints
    if (flags & MWM_HINTS_FUNCTIONS) {
      if (functions & MWM_FUNC_ALL){
        decor|=(DECOR_CLOSE|DECOR_TITLE);
        }
      else {
        if (functions & MWM_FUNC_CLOSE)
          decor|=(DECOR_CLOSE|DECOR_TITLE);
        if (functions & MWM_FUNC_MAXIMIZE)
          decor|=(DECOR_MAXIMIZE|DECOR_TITLE);
        if (functions & MWM_FUNC_RESIZE)
          decor|=DECOR_RESIZE;
        }
      }
    decor_ref = decor;
    this->update( );
    return true;

  }

  return false;
}

void FXClientWindow::configure( )
{
  //FXString nfo = "Client Coordinates: ";
  //nfo += FXString::value( carea.x ) + " " + FXString::value( carea.y ) + " " + FXString::value( carea.w ) + " " + FXString::value( carea.h );
  //print_info( "configure", nfo, "WARNING" );

  calculateReferencePoint( );
  notifyClient( );
  positionFrameWindow( );
  update( );
}

void FXClientWindow::active( )
{
  if( state != STATE_MINIMIZED ) {
    if( shown( ) == false ) { show( ); }
    raise( );
    setFocus( );
    notifyActivate( );
    XSetInputFocus( (Display*) getApp( )->getDisplay( ), cid, RevertToNone, CurrentTime );
    update( );
  }
}


void FXClientWindow::raise( )
{
  //print_info( "raise", FXString::null );
  getWindowManager( )->getWorkspace( workspace )->handle( this, FXSEL( SEL_COMMAND, WMStack::ID_RAISE ), this );
  //fxwarning( "[FXClientWindow::raise]: END \n" );
}

void FXClientWindow::maximize( )
{
  //print_info( "maximize", FXString::null );

  FXint oldstate = state;
  FXint xx, yy, ww, hh;

  state = STATE_MAXIMIZED;
  getWindowManager( )->getWorkspace( workspace )->getMaximizeRange( xx, yy, ww, hh );

  fxwarning("Maximize Range: %d %d %d %d\n",xx,yy,ww,hh);
  toClientCoordinates( xx, yy, ww, hh );

  //fxwarning("Maximize Range (client coordinates): %d %d %d %d\n",xx,yy,ww,hh);
  if( oldstate == STATE_SHADED ) { carea.h = ncarea.h; }
  /*
  ncarea.x = carea.x;
  nc_y = carea.y;
  nc_w = carea.w;
  nc_h = carea.h;
  */
  ncarea = carea;
  carea.x  = xx;
  carea.y  = yy;
  carea.w  = ww;
  carea.h  = hh;

  /// Unshade if we're shaded
  if( oldstate == STATE_SHADED ) { client->show(); }


  configure( );
  raise( );
  notifyState( );

  //fxwarning( "FXClientWindow::maximize( ) END" );
}

void FXClientWindow::iconify( )
{
  //print_info( "iconyfi", FXString::null );
  // state = STATE_MINIMIZED;
  /// FIXME : Tato funkce odesila CM WM_CHANGE_STATE !!!
  //XIconifyWindow( disp, cid, XDefaultScreen( disp ) );
  hide( STATE_MINIMIZED );

  //raise( );
  //notifyState( );
  //notifyDeactivate( );
}


void FXClientWindow::deiconify( )
{
  if( state == STATE_MINIMIZED ) {
    //print_info( "deiconyfy", FXString::null );
    state = STATE_NORMAL;

    configure( );
    active( );
    notifyState( );
   }
}

void FXClientWindow::shade( )
{
  //print_info( "shade", FXString::null );
  state = STATE_SHADED;
  ncarea = carea;
  carea.h = 0;
  client->hide( );

  configure( );
  raise( );
  notifyState( );
}

void FXClientWindow::unshade( )
{
  //print_info( "unshade", FXString::null );
  state = STATE_NORMAL;
  carea.h = ncarea.h;
  client->show( );

  configure( );
  active( );
  notifyState( );
}

void FXClientWindow::restore( )
{
  switch( state ) {
    case STATE_MINIMIZED  : deiconify( ); break;
    case STATE_SHADED     : unshade( ); break;
    case STATE_WITHDRAWN  :
    {
      state = STATE_NORMAL;
      show( );
      //configure( );
      //notifyClient( );
      //notifyState( );
      //update( );
      break;
    }
    case STATE_FULLSCREEN : decor = decor_ref;
    default : {
      print_info( "restore", FXString::null );
      state = STATE_NORMAL;
      show( );
      carea = ncarea;
      configure( );
      raise( );
      notifyState( );
    }
  }
}

void FXClientWindow::fullscreen( )
{
  //print_info( "fullscreen", FXString::null );
  state = STATE_FULLSCREEN;
  decor = DECOR_NONE;
  wpriority = 2;

  configure( );
  raise( );
  notifyState( );
}

void FXClientWindow::doEnter( FXRawEvent &ev ){
  FXbool leave    = false;
  FXbool inferior = false;
  XEvent e;

  while( XCheckTypedWindowEvent( disp, ev.xcrossing.window, LeaveNotify, &e ) ) {
    if( e.type == LeaveNotify && e.xcrossing.mode == NotifyNormal ) {
      leave = true;
      inferior = ( e.xcrossing.detail == NotifyInferior );
    }
  }

  if( ( !leave || inferior ) ) { setInputFocus( ); }
}



void FXClientWindow::doConfigure(FXRawEvent & ev){
  FXint old_x = carea.x;
  FXint old_y = carea.y;
  FXint old_h = carea.h;
  FXint old_w = carea.w;


  /// See if we had any changes
  if (ev.xconfigurerequest.value_mask & CWX)      carea.x = ev.xconfigurerequest.x;
  if (ev.xconfigurerequest.value_mask & CWY)      carea.y = ev.xconfigurerequest.y;
  if (ev.xconfigurerequest.value_mask & CWWidth)  carea.w = ev.xconfigurerequest.width;
  if (ev.xconfigurerequest.value_mask & CWHeight) carea.h = ev.xconfigurerequest.height;

  /// FIXME Handle CWStack

  /// FIXME: CLIENT_01
  /*
  /// Make sure the size is within its bounds
  carea.w = FXCLAMP(min_width,carea.w,max_width);
  carea.h = FXCLAMP(min_height,carea.h,max_height);
  //fxwarning("MAx Size Window %d, %d \n", max_width, max_height );
  //fxwarning("Min Size Window %d, %d \n", min_width, min_height );
  //fxwarning("CLAMP Size:  width = %d, height = %d \n", carea.w, carea.h );
  */

  /// Configure the Window if something has changed
  if ( old_x != carea.x || old_y != carea.y || old_w != carea.w || old_h != carea.h ) {
    if (state==STATE_MAXIMIZED) { state=STATE_NORMAL; }
    configure( );
  }
  update( );
  //fxwarning("Resulth Size: width = %d, height = %d \n", carea.w, carea.h );
}

void FXClientWindow::updateProperty( FXRawEvent *ev /*Atom property*/ ){
    Atom property = ev->xclient.message_type;
    //print_info( "updateProperty", XGetAtomName( disp, property ) );
    fxwarning( "[FXClientWindow::updateProperty]: Client Xproperty atom %s - %s \n", XGetAtomName( ( Display*) getDisplay( ), ev->xclient.message_type ), XGetAtomName( ( Display*) getDisplay( ), ev->xproperty.atom ) );
	if( property == XA_WM_NAME || property == FXAtom::_NET_WM_NAME ) {
      getTitle( );
      update( );
      return;
    }
	if( property == FXAtom::WM_CHANGE_STATE || property == FXAtom::_NET_DESKTOP_VIEWPORT /*|| FXAtom::_NET_WM_USER_TIME*/ ){
      getNETState( );
      update();
      return;
    }

    if( property == FXAtom::_NET_WM_STATE) {
       property = ev->xclient.data.l[ 1 ];
       if( property == FXAtom::_NET_WM_STATE_FULLSCREEN ) {
        fullscreen( );
        return;
	   }
	   if( property == FXAtom::_NET_WM_STATE_MAXIMIZED_VERT || property == FXAtom::_NET_WM_STATE_MAXIMIZED_HORZ  ) {
         maximize( );
         return;
	   }
	   if( property == FXAtom::_NET_WM_STATE_SHADED ) {
         shade( );
         return;
	   }
    }
	else {
	  //fxwarning("\tUnhandled Property Update: %s\n",XGetAtomName(disp,property));
      FXString text = "Unhandled Property Update ( ";
      text += XGetAtomName( disp, property );
      text += " )";
      print_info( "updateProperty", text, "WARNING" );
    }
  return;
  }



long FXClientWindow::onCmdCloseWindow(FXObject*,FXSelector,void* ptr)
{
  state = STATE_CLOSE;
  notifyClose( );

  return 1;
}

FXuchar FXClientWindow::where( FXint x, FXint y )
{
  FXuchar code = DRAG_NONE;
//  FXint fh,mh,bh,th;
//  fh=font->getFontHeight();
//  mh=windowbtn->getDefaultHeight();
//  bh=deletebtn->getDefaultHeight();
  FXint th = font->getFontHeight( );
  if( ( decor & DECOR_RESIZE ) && ( state != STATE_SHADED ) ) {
    if(x<4)         { code |= DRAG_LEFT;  }
    if(width-4<=x)  { code |= DRAG_RIGHT;  }
    if(y<4)         { code |= DRAG_TOP;    }
    if(height-4<=y) { code |= DRAG_BOTTOM; }
  }
  if( ( decor & DECOR_TITLE ) && ( state != STATE_MAXIMIZED ) && (4 <= x && x <= width - 4 && 4 <= y && y < 4 + th) ) { code = DRAG_TITLE; }

  return code;
}

// Pressed LEFT button
long FXClientWindow::onLeftBtnPress( FXObject*, FXSelector, void* ptr )
{
  register FXEvent *event = static_cast<FXEvent*>( ptr );
  flags &= ~FLAG_TIP;
  setInputFocus( );
  raise( );
  if( isEnabled( ) ){
    grab( );
    if( event->click_count == 1 ){
      dragmode = where( event->win_x, event->win_y );
      if( dragmode != DRAG_NONE ){
        if( dragmode & ( DRAG_TOP | DRAG_TITLE ) ){ spot.y = event->win_y; }
        else{ if( dragmode & DRAG_BOTTOM ) { spot.y = event->win_y - height; } }
        if( dragmode & ( DRAG_LEFT | DRAG_TITLE ) ){ spot.x = event->win_x; }
        else{ if( dragmode & DRAG_RIGHT ){ spot.x = event->win_x - width; } }
        dragarea.set( getX( ), getY( ), getWidth( ), getHeight( ) );
      }
    }
    return 1;
  }
  return 0;
}

// Released LEFT button
long FXClientWindow::onLeftBtnRelease( FXObject*, FXSelector, void* ptr ){
  register FXEvent *event = static_cast<FXEvent*>( ptr );
  if( isEnabled( ) ) {
    ungrab( );
    if( target && target->handle( this, FXSEL( SEL_LEFTBUTTONRELEASE, message ), ptr ) ) { return 1; }
    if( event->click_count ==1 ) {
      if( dragmode != DRAG_NONE ) { position( dragarea.x, dragarea.y, dragarea.w, dragarea.h ); }
      dragmode = DRAG_NONE;
      recalc( );
    }
    else if( ( event->click_count == 2 ) && ( !event->moved ) ) {
      //FXint d=where(event->win_x,event->win_y);
      if( state == STATE_SHADED ) { unshade( ); } else { shade( ); }
    }
  }
  return 0;
}


// Moved
long FXClientWindow::onMotion(FXObject*,FXSelector,void* ptr){
  register FXEvent *event = static_cast<FXEvent*>( ptr );
  register FXint tmp, mousex, mousey;
  register FXint dummy;

  FXint mw = wmin.w;
  FXint mh = wmin.h;
  toWindowCoordinates( dummy, dummy, mw, mh );
  FXint MINHEIGHT = FXMAX( 50, mh );
  FXint MINWIDTH = FXMAX( 50, mw );

  if( dragmode != DRAG_NONE ) {
    mousex = event->root_x;
    mousey = event->root_y;

    // Keep inside RootWindow
    if( mousex < 0 ) { mousex = 0; }
    if( mousey < 0 ) { mousey = 0; }
    if(mousex >= getApp( )->getRootWindow( )->getWidth( ) ) { mousex = getApp( )->getRootWindow( )->getWidth( ) - 1; }
    if(mousey >= getApp( )->getRootWindow( )->getHeight( ) ) { mousey = getApp( )->getRootWindow( )->getHeight( ) - 1; }

    // Dragging title
    if( dragmode & DRAG_TITLE ) {
      if( !event->moved ) { return 1; }
      carea.x = dragarea.x = mousex - spot.x;
      carea.y = dragarea.y = mousey - spot.y;
      toClientCoordinates( (FXint&)carea.x, (FXint&)carea.y, dummy, dummy );
    }

    // Dragging sides
    else{
      // Vertical
      if( dragmode & DRAG_TOP ) {
        tmp=dragarea.h+dragarea.y-mousey+spot.y;
        if(tmp>=MINHEIGHT){ dragarea.h=tmp; dragarea.y=mousey-spot.y; }
        }
      else if(dragmode&DRAG_BOTTOM){
        tmp=mousey-spot.y-dragarea.y;
        if(tmp>=MINHEIGHT){ dragarea.h=tmp; }
        }

      // Horizontal
      if(dragmode&DRAG_LEFT){
        tmp=dragarea.w+dragarea.x-mousex+spot.x;
        if(tmp>=MINWIDTH){ dragarea.w=tmp; dragarea.x=mousex-spot.x; }
        }
      else if(dragmode&DRAG_RIGHT){
        tmp=mousex-spot.x-dragarea.x;
        if(tmp>=MINWIDTH){ dragarea.w=tmp; }
        }
      /*
      carea.x = dragarea.x;
      carea.y = dragarea.y;
      carea.w = neww;
      carea.h = newh;
      */
      carea = dragarea;

      state=STATE_NORMAL;
      toClientCoordinates( (FXint&)carea.x, (FXint&)carea.y, (FXint&)carea.w, (FXint&)carea.h );
      }
    position( dragarea.x, dragarea.y, dragarea.w, dragarea.h);
    }
  else {
    // Othersize just change cursor based on location
    changeCursor(event->win_x,event->win_y);
    }
  return 0;
  }


long FXClientWindow::onEnter(FXObject*sender,FXSelector sel,void* ptr){
  FXComposite::onEnter(sender,sel,ptr);
  //fxwarning("ENTER WINDOW %d\n",(int)cid);

  setFocus();
  update();
  XSetInputFocus((Display*)getApp()->getDisplay(),cid, RevertToNone, CurrentTime);
  return 1;
  }

long FXClientWindow::onLeave(FXObject*sender,FXSelector sel,void* ptr){
  FXComposite::onLeave(sender,sel,ptr);
 // killFocus();
  update();
  //fxwarning("LEAVE WINDOW %d\n",(int)cid);
//  XSetInputFocus((Display*)getApp()->getDisplay(),cid, RevertToNone, CurrentTime);
  return 1;
  }

long FXClientWindow::onFocusIn(FXObject*sender,FXSelector sel,void* ptr){
  FXComposite::onFocusIn(sender,sel,ptr);
  update();
  return 1;
  }

long FXClientWindow::onFocusOut(FXObject*sender,FXSelector sel,void* ptr){
  FXComposite::onFocusOut(sender,sel,ptr);
  update();
  return 1;
}
/*
long FXClientWindow::onCmdMinimize( FXObject*, FXSelector, void* )
{
  if( state == STATE_MINIMIZED ) { deiconify( ); } else { iconify( ); }
  return 1;
}
*/
long FXClientWindow::onUpdMinimize( FXObject* sender, FXSelector, void* )
{
  if( state == STATE_MINIMIZED ) { sender->handle( this, FXSEL( SEL_COMMAND, ID_DISABLE ), NULL ); }
  else { sender->handle( this, FXSEL( SEL_COMMAND, ID_ENABLE ), NULL ); }
  return 1;
}
/*
long FXClientWindow::onCmdMaximize( FXObject*, FXSelector, void* )
{
  if ( state != STATE_MAXIMIZED ) { maximize( ); }
  return 1;
}
*/
long FXClientWindow::onUpdMaximize(FXObject* sender,FXSelector,void*){
  if (state==STATE_MAXIMIZED)
  	sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),NULL);
  else
  	sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),NULL);
  return 1;
  }


/*
long FXClientWindow::onCmdRestore(FXObject*,FXSelector,void*){
  restore();
  return 1;
}
*/

long FXClientWindow::onCmdActivate( FXObject*, FXSelector, void*){
  FXString nfo = "Activate client top-level window \n";

  switch( state ) {
    case STATE_MINIMIZED : deiconify( );  nfo += "Window change state: DEICONIFY"; break;
    case STATE_SHADED    : unshade( );    nfo += "Window change state: UNSHADE"; break;
    default : active( );  nfo += "Window change state: ACTIVATE";
  }

  print_info( "onCmdActivate", nfo );
  return 1;
}

long FXClientWindow::onCmdState( FXObject* sender, FXSelector sel, void* data )
{
  FXString nfo = "Change state for client window";
  FXint n_state = state;

  nfo += "\nSENDER : ";
  nfo += sender->getClassName( );
  nfo += "\nPrevious STATE ID: ";
  nfo += FXString::value( state );

  switch( FXSELID( sel ) ) {
    case FXClientWindow::ID_STATE : {
      FXRawEvent *ev = ( FXRawEvent* ) data;
      if( ev != NULL ) {
        nfo += "\nMESSAGE TYPE :";
        nfo += XGetAtomName( disp, ev->xclient.message_type );
        nfo += "\nDATA :";
        nfo += XGetAtomName( disp, ev->xclient.data.l[ 1 ] );
        nfo += "\n";


        //updateProperty( ev->xclient.message_type  );
        updateProperty( ev );
        /*
        switch( state ) {
          case STATE_MINIMIZED  : { iconify( );    nfo += "Window state: ICONIFY";    break; }
          case STATE_MAXIMIZED  : { maximize( );   nfo += "Window state: MAXIMIZED";  break; }
          case STATE_SHADED     : { shade( );      nfo += "Window state: SHADED";     break; }
          case STATE_FULLSCREEN : { fullscreen( ); nfo += "Window state: FULLSCREEN"; break; }
          default               : { restore( );    nfo += "Window state: RESTORE";        }
        }

        nfo += "\nSTATE ID :";
        nfo += FXString::value( state );
        nfo += "\n";*/
      }
     //print_info( "onCmdState", nfo );
     //return 1; */
     n_state = state;
      break;
    }
    case FXClientWindow::ID_CHANGESTATE  : {
      if( state != STATE_FULLSCREEN ) {
        if( state == STATE_MINIMIZED || state == STATE_SHADED ) { n_state = STATE_NORMAL; }
        else { n_state = STATE_MINIMIZED; }
      }
      break;
    }
    case FXClientWindow::ID_MAXIMIZE     : { n_state = ( ( state != STATE_MAXIMIZED )  ? STATE_MAXIMIZED  : STATE_NORMAL ); break; }
    case FXClientWindow::ID_MDI_MAXIMIZE : { n_state = ( ( state != STATE_MAXIMIZED )  ? STATE_MAXIMIZED  : STATE_NORMAL ); break; }
    case FXClientWindow::ID_MINIMIZE     : { n_state = ( ( state != STATE_MINIMIZED )  ? STATE_MINIMIZED  : STATE_NORMAL ); break; }
    case FXClientWindow::ID_MDI_MINIMIZE : { n_state = ( ( state != STATE_MINIMIZED )  ? STATE_MINIMIZED  : STATE_NORMAL ); break; }
    case FXClientWindow::ID_SHADE        : { n_state = ( ( state != STATE_SHADED )     ? STATE_SHADED     : STATE_NORMAL ); break; }
    case FXClientWindow::ID_FULLSCREEN   : { n_state = ( ( state != STATE_FULLSCREEN ) ? STATE_FULLSCREEN : STATE_NORMAL ); break; }
    default :                            { n_state = STATE_NORMAL;     break; }
  }

  if( state != n_state ) {
    nfo += "\n";
    switch( n_state ) {
      case STATE_MINIMIZED  : { iconify( );    nfo += "Window state: ICONIFY";    break; }
      case STATE_MAXIMIZED  : { maximize( );   nfo += "Window state: MAXIMIZED";  break; }
      case STATE_SHADED     : { shade( );      nfo += "Window state: SHADED";     break; }
      case STATE_FULLSCREEN : { fullscreen( ); nfo += "Window state: FULLSCREEN"; break; }
      default               : { restore( );    nfo += "Window state: RESTORE";           }
    }
  }
  active( );
  nfo += "\nSTATE ID :";
  nfo += FXString::value( state );
  nfo += "\n";

  print_info( "onCmdState", nfo );

  return 1;
}

long FXClientWindow::onUpdRestore(FXObject* sender,FXSelector,void*){
  if ((state!=STATE_MAXIMIZED) && (state!=STATE_MINIMIZED))
  	sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),NULL);
  else
  	sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),NULL);
  return 1;
  }



FXbool FXClientWindow::setInputFocus( ) {
  if( canFocus( ) ) { return true; }

  FXbool gotfocus = false;
  setFocus( );
  switch( focusmode ) {
    case FOCUSMODE_NONE     : break;
    case FOCUSMODE_GLOBAL   : break;
    case FOCUSMODE_LOCAL    :
    case FOCUSMODE_PASSIVE  : XSetInputFocus( disp, cid, RevertToPointerRoot, CurrentTime ); gotfocus = true; break;
  }
  notifyFocus( );
  return gotfocus;
}


// Change cursor based on location over window
void FXClientWindow::changeCursor(FXint x,FXint y){
  switch(where(x,y)){
    case DRAG_TOP:
    case DRAG_BOTTOM:
      setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGH_CURSOR));
      setDragCursor(getApp()->getDefaultCursor(DEF_DRAGH_CURSOR));
      break;
    case DRAG_LEFT:
    case DRAG_RIGHT:
      setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGV_CURSOR));
      setDragCursor(getApp()->getDefaultCursor(DEF_DRAGV_CURSOR));
      break;
    case DRAG_TOPLEFT:
    case DRAG_BOTTOMRIGHT:
      setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGTL_CURSOR));
      setDragCursor(getApp()->getDefaultCursor(DEF_DRAGTL_CURSOR));
      break;
    case DRAG_TOPRIGHT:
    case DRAG_BOTTOMLEFT:
      setDefaultCursor(getApp()->getDefaultCursor(DEF_DRAGTR_CURSOR));
      setDragCursor(getApp()->getDefaultCursor(DEF_DRAGTR_CURSOR));
      break;
    default:
      setDefaultCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
      setDragCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
      break;
    }
  }


// Revert cursor to normal one
void FXClientWindow::revertCursor(){
  setDefaultCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
  setDragCursor(getApp()->getDefaultCursor(DEF_ARROW_CURSOR));
  }


void FXClientWindow::getTitle()
{
  XTextProperty property;
  FXint nums;
  FXchar **list;

  /*
  _NET_WM_NAME -
  The Client SHOULD set this to the title of the window in UTF-8 encoding. If set the Window Manager should use
  this in preference to WM_NAME. If we don't find the _NET_WM_NAME, just use the WM_NAME
  */

  if( xproperty->getUTF8String( FXAtom::_NET_WM_NAME, title ) ) { return; }

  if( XGetWMName( disp, cid, &property ) ) {
    if( property.value && property.nitems > 0 ) {
      if( ( XTextPropertyToStringList( &property, &list, &nums ) == Success ) && ( nums > 0 ) && *list ) {
    	title = *list;
		XFreeStringList(list);
      }
      else {
		title = ( char* ) property.value;
        XFree( ( char* ) property.value );
      }
    }
  }
}

void FXClientWindow::print_info( const FXString &_fnc, const FXString &_msg, const FXString &_type )
{

  FXString text = "[";
  text += getClassName( );
  if( !_fnc.empty( ) ) { text = text + "::" + _fnc; }
  text += "]: \n  CLIENT ID = " +  FXString::value( ( int ) this->cid ) + ", GROUP ID = " + FXString::value( ( int ) this->group ) + ", TYPE = " + this->type_text + ", TITLE = " + this->Title( ) + "\n";
  if( !_msg.empty( ) ) {
    text += _type + " = { \n\t";
    text += _msg + "\n} \n";
  }

  fxwarning( text.text( ) );
}


Display* FXClientWindow::getDisplay( )
{
  return ( static_cast<Display*>( getApp( )->getDisplay( ) ) );
}
