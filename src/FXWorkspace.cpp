/*******************************************************************************
*                         fxwm - FOX Window Manager                            *
********************************************************************************
*        Copyright (C) 2002-2003 by Sander Jansen. All Rights Reserved         *
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
#include<new>
#include<iostream>

// Fox Library
#include<xincs.h>
#include<fx.h>
#include<FXArray.h>

// FXWM
#include<FXWindowManager.h>
#include<FXClientWindow.h>
#include<FXWorkspace.h>
#include<FXAtom.h>
//#include<DWinNode.h>

#define DISPLAY ((Display*)((manager->getDisplay())))

FXIMPLEMENT(FXWorkspace,FXObject,NULL,0)

FXWorkspace::FXWorkspace( )
           : manager( NULL ), title( "Workspace" ), back( NULL ), front( NULL ), ls( 0 ), rs( 0 ),
             ts( 0 ), bs( 0 ), disp( NULL )
{ }

FXWorkspace::FXWorkspace( FXWindowManager *m )
           : manager( m ), title( "Workspace" ), back( NULL ), front( NULL ), ls( 0 ), rs( 0 ),
             ts( 0 ), bs( 0 ), disp( NULL )
{ }

FXWorkspace::~FXWorkspace( )
{ manager=NULL; }

/*************************************************************************************************/
void FXWorkspace::hide( )
{ }

void FXWorkspace::show( )
{
  fxwarning( "[FXWorkspace::show]: BEGIN \n" );
  if( disp == NULL ) { disp = static_cast<Display*>( manager->getDisplay( ) ); }
  fxwarning( "[FXWorkspace::show]: END \n" );
}

FXClientWindow *FXWorkspace::getLastTransient( FXClientWindow *p )
{
  fxwarning( "[FXWorkspace::getLastTransient]: BEGIN \n" );
  while( p->next_window ) { p = p->next_window; }
  fxwarning( "[FXWorkspace::getLastTransient]: END \n" );
  return p;
}

void FXWorkspace::listWindows( FXClientWindow *p, FXWindowList *windowlist  ) {
  fxwarning( "[FXWorkspace::listWindow]: BEGIN \n" );
  FXID  _id = 0;

  while( p ) {
    if( p->transient_windows ) { listWindows( getLastTransient( p->transient_windows ), windowlist ); }
    _id =  p->id( );
    windowlist->append( _id );

    ///FIXME - TAKTO ALE ROZHODNE NE!!!!
    if( p != p->previous_window ) { p = p->previous_window; }
    else {
      std::cout << "[FXWorkspace::listWindows]: Okno " << _id << " / " << p->Title( ).text( ) << " predchazi samo sobe" << std::endl;
      std::cout << "[FXWorkspace::listWindows]: WM muze byt v nestabilnim stavu, oprace je nasilne ukoncena!" << std::endl;
      p = NULL;
    }
  }
  fxwarning( "[FXWorkspace::listWindow]: END \n" );
}

void FXWorkspace::restack( )
{
  fxwarning( "[FXWorkspace::restack]: BEGIN \n" );

  FXWindowList windowlist;
  listWindows( front, &windowlist );
  XRestackWindows( disp, windowlist.data( ), windowlist.no( ) );
  //XChangeProperty( disp, manager->getRootWindow( )->id( ), FXAtom::_NET_CLIENT_LIST_STACKING, XA_WINDOW, 32, PropModeReplace, (unsigned char* ) windowlist.data( ), windowlist.no( ) );
  //notifyClientList_Restack( );
  fxwarning( "[FXWorkspace::restack]: END \n" );
}

void FXWorkspace::notifyClientList( FXWindowList *list )
{
  fxwarning( "[FXWorkspace::notifyClientList]: BEGIN \n" );
/*
  FXWindowList *wl = NULL;
  if( list == NULL ) { listWindows( back, wl ); } else { wl = list; }
  XChangeProperty( disp, manager->getRootWindow( )->id( ), FXAtom::_NET_CLIENT_LIST_STACKING, XA_WINDOW, 32, PropModeReplace, (unsigned char* ) wl->data( ), wl->no( ) );
*/
  FXWindowList wl;
  listWindows( front, &wl );
  XChangeProperty( disp, manager->getRootWindow( )->id( ), FXAtom::_NET_CLIENT_LIST_STACKING, XA_WINDOW, 32, PropModeReplace, (unsigned char* ) wl.data( ), wl.no( ) );

  //XChangeProperty( disp, manager->getRootWindow( )->id( ), FXAtom::_NET_CLIENT_LIST, XA_WINDOW, 32, PropModeReplace, (unsigned char* ) wl.data( ), wl.no( ) );
  //manager->notifyClientList( FXAtom::_NET_CLIENT_LIST_STACKING );
  fxwarning( "[FXWorkspace::notifyClientList]: END \n" );
}


void FXWorkspace::addTransientWindow( FXClientWindow *parent, FXClientWindow *transient )
{
  fxwarning( "[FXWorkspace::addTransientWindow]: BEGIN \n" );
  FXClientWindow * window;

  if( !parent->transient_windows ) {
    parent->transient_windows  = transient;
	transient->next_window     = NULL;
	transient->previous_window = NULL;
  }
  else {
    window = getLastTransient( parent->transient_windows );
	//while( window->next_window ) { window = window->next_window; }
    transient->previous_window = window;
    window->next_window        = transient;
	transient->next_window     = NULL;
  }
  fxwarning( "[FXWorkspace::addtransientWindow]: END \n" );
}

void FXWorkspace::addWindowFront( FXClientWindow *window )
{
  fxwarning( "[FXWorkspace::addWindowFront]: BEGIN \n" );
  if( !front ) {
    front = back = window;
    window->next_window     = NULL;
    window->previous_window = NULL;
  }
  else {
    window->next_window     = NULL;
	window->previous_window = front;
    front->next_window      = window;
	front                   = window;
  }
  fxwarning( "[FXWorkspace::addWindowFront]: END \n" );
}

void FXWorkspace::addWindowFrontMost( FXClientWindow *w )
{
  fxwarning( "[FXWorkspace::addWindowFrontMost]: BEGIN \n" );
  FXint pos = 1;

  FXClientWindow *p = front;
  if( !p ) {
    std::cout << "window " << w->id( ) << " je prvni ve stacku" << std::endl;
  	back = front = w;
    w->next_window     = NULL;
    w->previous_window = NULL;
  }
  else {
	while( p ) {
      if( w->getTopPriority( ) >= p->getTopPriority( ) ) {
        if( p->next_window ) {
          std::cout << "window " << w->id( ) << " je zarazeno jako " << pos << std::endl;
          w->previous_window = p;
          w->next_window = p->next_window;
          FXASSERT( p->next_window );
          p->next_window->previous_window = w;
          p->next_window = w;
        }
        else { // Front of List
          std::cout << "window " << w->id( ) << "je "<< pos << ", \'front\' => vrchol stacku" << std::endl;
          w->previous_window = p;
          w->next_window = NULL;
          p->next_window = w;
          front = w;
        }
  	    return;
      }
	  p = p->previous_window;
	  pos++;
    }
    // Back of list
    std::cout << "window " << w->id( ) << "je \'back\' => dno stacku" << std::endl;
    w->previous_window = NULL;
    w->next_window = back;
    FXASSERT( back );
    back->previous_window = w;
    back = w;
  }
  fxwarning( "[FXWorkspace::addWindowFrontMost]: END \n" );
}

void FXWorkspace::removeWindow( FXClientWindow *w)
{
  fxwarning( "[FXWorkspace::removeWindow]: BEGIN \n" );
  if( w->next_window && w->previous_window ) {
    w->previous_window->next_window = w->next_window;
    w->next_window->previous_window = w->previous_window;
  }
  else if( w->previous_window ) {
    w->previous_window->next_window = NULL;
    front = w->previous_window;
  }
  else if( w->next_window ) {
    w->next_window->previous_window = NULL;
    back = w->next_window;
  }
  else {
    back = front = NULL;
  }
  restack( );
  updateStrut( );
  notifyClientList( );

  if( back != NULL ) { front->handle( this, FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE ), NULL ); }
  fxwarning( "[FXWorkspace::removeWindow]: END \n" );
}


void FXWorkspace::raise( FXClientWindow * window )
{
  fxwarning( "[FXWorkspace::raise]: BEGIN \n" );
  FXClientWindow *w = window;
  FXClientWindow *f = front;

  while( w->transientFor ) { w = w->transientFor; }
  while( ( f != NULL ) && (f != w ) ){
  	fxmessage( "\tRAISE Check: %s\n", f->title.text( ) );
  	if( f->getTopPriority( ) <= w->getTopPriority( ) ) {
  		fxmessage( "\tRAISE: Remove Window\n" );
  		removeWindow( w );
  		fxmessage( "\tRAISE: Add Front Most Window\n" );
  		addWindowFrontMost( w );
  		fxmessage( "\tRAISE: Restack\n" );
  		restack( );
  		break;
    }
  	f = f->previous_window;
  }
  fxwarning( "[FXWorkspace::raise]: END \n" );
}

void FXWorkspace::lower(FXClientWindow * window)
{
  fxwarning( "[FXWorkspace::lower]: BEGIN \n" );
  fxwarning( "[FXWorkspace::lower]: END \n" );
}

void FXWorkspace::addClient( /*FXClientWindow*/ WMTopWindow *window)
{
  fxwarning( "[FXWorkspace::addClient]: BEGIN \n" );
  if( window->transientFor ) { addTransientWindow( window->transientFor, window ); }
  else if( window->isWindowClass( CLASS_DESKTOP ) ) {
    /// Put it behind all windows ( Vlozit za vsechna okna )
    window->previous_window = NULL;
    window->next_window     = back;
    back->previous_window   = window;
    back = window;
  }
  else if( ( window->isWindowClass( CLASS_DOCK ) == true ) || ( window->isWindowClass( CLASS_TOOLBAR ) == true ) ) { addWindowFront( window ); }
  //else if( window->isWindowType( PROPERTY_TOOLBAR ) ) { addWindowFront( window ); }
  else { addWindowFrontMost( window ); }

  restack();
  updateStrut();
  notifyClientList( );
  fxwarning( "[FXWorkspace::addClient]: END \n" );
}

void FXWorkspace::getMaximizeRange( FXint &x, FXint &y, FXint &w, FXint &h )
{
  fxwarning( "[FXWorkspace::getMaximizeRange]: BEGIN \n" );
  x = ls;
  y = ts;
  w = manager->getRootWindow( )->getWidth( ) - rs;
  h = manager->getRootWindow( )->getHeight( ) - bs;
  fxwarning( "[FXWorkspace::getMaximizeRange]: END \n" );
}


void FXWorkspace::updateStrut( )
{
  fxwarning( "[FXWorkspace::updateStruct]: BEGIN \n" );
  FXClientWindow * p = front;
  ls = rs = ts = bs = 0;

  while( p ){
    ls += p->getLeftStrut( );
    rs += p->getRightStrut( );
    ts += p->getTopStrut( );
    bs += p->getBottomStrut( );

    if( p != p->previous_window ) { p = p->previous_window; }
    else {
      std::cout << "[FXWorkspace::updateStruct]: Okno " << p->id( ) << " / " << p->Title( ).text( ) << " predchazi samo sobe" << std::endl;
      std::cout << "[FXWorkspace::updateStruct]: WM muze byt v nestabilnim stavu, oprace je nasilne ukoncena!" << std::endl;
      p = NULL;
    }
  }
  fxwarning( "[FXWorkspace::updateStruct]: END \n" );
}

void FXWorkspace::removeTransient( FXClientWindow *w )
{
  fxwarning( "[FXWorkspace::removeTransient]: BEGIN \n" );
  if( w->next_window && w->previous_window) {
    w->previous_window->next_window = w->next_window;
    w->next_window->previous_window = w->previous_window;
    }
  else if (w->previous_window) {
    w->previous_window->next_window = NULL;
    }
  else if (w->next_window){
  	FXASSERT(w->transientFor);
  	FXASSERT(w->transientFor->transient_windows);
    w->transientFor->transient_windows = w->next_window;
    w->next_window->previous_window=NULL;
    }
  else {
  	FXASSERT(w->transientFor);
  	FXASSERT(w->transientFor->transient_windows);
    w->transientFor->transient_windows = NULL;
  }
  fxwarning( "[FXWorkspace::removeTransient]: END \n" );
}


void FXWorkspace::removeClient( FXClientWindow *w )
{
  fxwarning( "[FXWorkspace::removeClient]: BEGIN \n" );
  if (w->transientFor){
    removeTransient(w);
    }
	else {
		if (w->next_window && w->previous_window) {
			w->previous_window->next_window = w->next_window;
			w->next_window->previous_window = w->previous_window;
			}
		else if (w->previous_window){
			w->previous_window->next_window = NULL;
			front = w->previous_window;
			}
		else if (w->next_window) {
			back = w->next_window;
			w->next_window->previous_window=NULL;
			}
		else {
			back = front = NULL;
			}
		}
  restack();
  updateStrut();
  notifyClientList( );

  if( back != NULL ) { front->handle( this, FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE ), NULL ); }
  fxwarning( "[FXWorkspace::removeClient]: END \n" );
}
