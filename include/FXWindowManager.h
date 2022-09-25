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
#ifndef FXWINDOWMANAGER_H
#define FXWINDOWMANAGER_H

#include<WMStackList.h>
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
class FXClientWindow;
class FXWorkspace;
class WMStack;
//class WMStackList;
class FXWindowProperty;
class FXWindowDispatcher;

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
enum fxwmfocusmode {
  WM_FOCUS_SLOPPY = 0,
  WM_FOCUS_CLICK,
  WM_FOCUS_FOLLOW,
};

#define NUM_ATOMS 50

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
typedef FXArray<FXID>                  FXWindowList;
typedef FXObjectListOf<FXClientWindow> FXClientList;
//typedef FXObjectListOf<FXWorkspace>    FXWorkspaceList;
//typedef FXObjectListOf<WMStack>        FXWorkspaceList;
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
class FXWindowManager : public FXApp {
FXDECLARE(FXWindowManager)
  friend class FXClientWindow;
  friend class FXWorkspace;
  friend class WMStack;

private:
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
    FXint            grabcount;
    FXuint           focusmodel;
    FXMainWindow    *netwindow;
    FXWindow        *gnomewindow;
    //FXWorkspaceList  workspaces;
    WMStackList      workspaces;
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
    FXClientList    clientwindows; /// Seznam oken typu Client
    FXWindowList    clientids;     /// Seznam identifikatoru Client Ids

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
    FXWindowProperty *rootwindowproperty;
    FXWindowProperty *netwindowproperty;
    Atom xa_atoms[ NUM_ATOMS ];
    FXWindowDispatcher *cl_dispatch;

private:
    FXuint current_workspace;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// Konstruktor - Destruktor
    FXWindowManager( );
    virtual void create( );
    virtual ~FXWindowManager( );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    void grab( );
    void ungrab( );
    void sync( );

    void client_insert( FXID wid );                         // Vytvoreni klientskeho okna
    void client_append( FXClientWindow *window, FXint ws ); // Pridat klienta na urcitou plochu ( -1 na vsechny )
    void client_remove( FXClientWindow *window, FXID id );  // Odstraneni klientskeho okna

    void   wm_refresh( );	// Znovunacteni konfigurace a stylu
    void   wm_quit( );		// Ukonceni WM
    void   wm_restart( );	// Restart WM
    void   wm_shutdown( );	// Vypnuti pocitace
    FXbool wm_configure( );	// Konfigurace WM

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// Send Notifications for Listeners
    void notifyClientList( Atom a );
    void notifyWorkspaces( );


    FXbool hasDestroyEvent( FXID wid );
    FXbool hasUnmapEvent( FXID wid );
    FXbool hasLeaveEvent( FXID wid );

    FXbool isBadWindow( FXID winid );

    FXClientWindow* getClientWindow( FXID wnd );
    FXClientWindow* findClientWindow( FXID win_id );

    WMStack* getCurrentWorkspace( )   { return workspaces( );  }
    WMStack* getWorkspace( FXint id ) { return workspaces[ id ]; }


    Atom getAtom( FXuint a ) { return xa_atoms[ a ]; }

    FXbool Exec( const FXArray<const FXchar*> &args, FXbool wait = false, FXbool ver = false );
    void Autostart( );

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /*
    enum {
      ID_TERMINAL  = FXAPP::ID_LAST,

      ID_WORKSPACE_CHANGE,
      ID_WORKSPACE_NEXT,
      ID_WORKSPACE_PREW,
      ID_WORKSPACE_DESKTOP

      ID_WM_QUIT,
      ID_WM_RESTART,
      ID_WM_REFRESH,

      ID_LAST
    };
    */

protected:
    void initAtoms( );
    virtual bool dispatchEvent( FXRawEvent &event );
    virtual FXbool dispatchClient( FXRawEvent &event );
    virtual FXbool dispatchOf( Atom a, FXClientWindow *window, void *data );
    FXClientWindow* getEventWindow( FXRawEvent &event );
    int switchWorkspace( FXint wid );
};
#endif
