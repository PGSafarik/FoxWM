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
// Fox library
#include<fx.h>
#include<xincs.h>

// FXWM
#include<FXAtom.h>

#define DISPLAY ( ( Display* ) ( app->getDisplay( ) ) )

Atom FXAtom::WM_STATE                         = None;
Atom FXAtom::WM_CHANGE_STATE                  = None;
Atom FXAtom::WM_PROTOCOLS                     = None;
Atom FXAtom::WM_DELETE_WINDOW                 = None;
Atom FXAtom::WM_COLORMAP_WINDOWS              = None;
Atom FXAtom::WM_TAKE_FOCUS                    = None;
Atom FXAtom::UTF8_STRING                      = None;
Atom FXAtom::_MOTIF_WM_HINTS                  = None;
Atom FXAtom::_NET_SUPPORTED                   = None;
Atom FXAtom::_NET_SUPPORTING_WM_CHECK         = None;
Atom FXAtom::_NET_CLIENT_LIST                 = None;
Atom FXAtom::_NET_CLIENT_LIST_STACKING        = None;
Atom FXAtom::_NET_DESKTOP_GEOMETRY            = None;
Atom FXAtom::_NET_DESKTOP_VIEWPORT            = None;
Atom FXAtom::_NET_CURRENT_DESKTOP             = None;
Atom FXAtom::_NET_NUMBER_OF_DESKTOPS          = None;
Atom FXAtom::_NET_ACTIVE_WINDOW               = None;
Atom FXAtom::_NET_WM_NAME                     = None;
Atom FXAtom::_NET_WM_ICON                     = None;
Atom FXAtom::_NET_WM_STATE                    = None;
Atom FXAtom::_NET_WM_STATE_MODAL              = None;
Atom FXAtom::_NET_WM_STATE_STICKY             = None;
Atom FXAtom::_NET_WM_STATE_MAXIMIZED_VERT     = None;
Atom FXAtom::_NET_WM_STATE_MAXIMIZED_HORZ     = None;
Atom FXAtom::_NET_WM_STATE_SHADED             = None;
Atom FXAtom::_NET_WM_STATE_SKIP_TASKBAR       = None;
Atom FXAtom::_NET_WM_STATE_SKIP_PAGER         = None;
Atom FXAtom::_NET_WM_STATE_HIDDEN             = None;
Atom FXAtom::_NET_WM_STATE_FULLSCREEN         = None;
Atom FXAtom::_NET_WM_STATE_ABOVE              = None;
Atom FXAtom::_NET_WM_STATE_BELOW              = None;
Atom FXAtom::_NET_WM_WINDOW_TYPE              = None;
Atom FXAtom::_NET_WM_WINDOW_TYPE_DESKTOP      = None;
Atom FXAtom::_NET_WM_WINDOW_TYPE_DOCK         = None;
Atom FXAtom::_NET_WM_WINDOW_TYPE_TOOLBAR      = None;
Atom FXAtom::_NET_WM_WINDOW_TYPE_MENU         = None;
Atom FXAtom::_NET_WM_WINDOW_TYPE_UTILITY      = None;
Atom FXAtom::_NET_WM_WINDOW_TYPE_SPLASH       = None;
Atom FXAtom::_NET_WM_WINDOW_TYPE_DIALOG       = None;
Atom FXAtom::_NET_WM_WINDOW_TYPE_NORMAL       = None;
Atom FXAtom::_NET_WM_DESKTOP                  = None;
Atom FXAtom::_NET_WM_STRUT                    = None;
Atom FXAtom::_NET_WM_STRUT_PARTIAL            = None;
Atom FXAtom::_KDE_NET_WM_WINDOW_TYPE_OVERRIDE = None;
Atom FXAtom::_NET_CLOSE_WINDOW                = None;
Atom FXAtom::_NET_RESTACK_WINDOW              = None;
Atom FXAtom::_NET_WM_STATE_DEMANDS_ATTENTION  = None;
Atom FXAtom::_NET_WM_STATE_FOCUSED            = None;
Atom FXAtom::_NET_WM_USER_TIME_WINDOW         = None;
Atom FXAtom::_NET_WM_USER_TIME                = None;

FXbool FXAtom::init(FXApp * app) {
  const FXchar * atomnames[]={
    "WM_STATE",
    "WM_CHANGE_STATE",
    "WM_PROTOCOLS",
    "WM_DELETE_WINDOW",
    "WM_COLORMAP_WINDOWS",
    "WM_TAKE_FOCUS",
    "UTF8_STRING",
    "_MOTIF_WM_HINTS",
    "_NET_SUPPORTED",
    "_NET_SUPPORTING_WM_CHECK",
    "_NET_CLIENT_LIST",
    "_NET_CLIENT_LIST_STACKING",
    "_NET_DESKTOP_GEOMETRY",
    "_NET_DESKTOP_VIEWPORT",
    "_NET_CURRENT_DESKTOP",
    "_NET_NUMBER_OF_DESKTOPS",
    "_NET_ACTIVE_WINDOW",
    "_NET_WM_NAME",
    "_NET_WM_ICON",
    "_NET_WM_STATE",
    "_NET_WM_STATE_MODAL",
    "_NET_WM_STATE_STICKY",
    "_NET_WM_STATE_MAXIMIZED_VERT",
    "_NET_WM_STATE_MAXIMIZED_HORZ",
    "_NET_WM_STATE_SHADED",
    "_NET_WM_STATE_SKIP_TASKBAR",
    "_NET_WM_STATE_SKIP_PAGER",
    "_NET_WM_STATE_HIDDEN",
    "_NET_WM_STATE_FULLSCREEN",
    "_NET_WM_STATE_ABOVE",
    "_NET_WM_STATE_BELOW",
    "_NET_WM_WINDOW_TYPE",
    "_NET_WM_WINDOW_TYPE_DESKTOP",
    "_NET_WM_WINDOW_TYPE_DOCK",
    "_NET_WM_WINDOW_TYPE_TOOLBAR",
    "_NET_WM_WINDOW_TYPE_MENU",
    "_NET_WM_WINDOW_TYPE_UTILITY",
    "_NET_WM_WINDOW_TYPE_SPLASH",
    "_NET_WM_WINDOW_TYPE_DIALOG",
    "_NET_WM_WINDOW_TYPE_NORMAL",
    "_NET_WM_DESKTOP",
    "_NET_WM_STRUT",
    "_NET_WM_STRUT_PARTIAL",
    "_KDE_NET_WM_WINDOW_TYPE_OVERRIDE",
    "_NET_CLOSE_WINDOW",
    "_NET_RESTACK_WINDOW",
    "_NET_WM_STATE_DEMANDS_ATTENTION",
    "_NET_WM_STATE_FOCUSED",
    "_NET_WM_USER_TIME_WINDOW",
    "_NET_WM_USER_TIME",

  };

  const FXint atomcount = ARRAYNUMBER( atomnames );
  Atom xa_atoms[ atomcount ];
  XInternAtoms( DISPLAY, ( char** )atomnames, atomcount, false, xa_atoms );

  for( FXint i = 0; i < atomcount; i++ ) {
    if( xa_atoms[ i ] == None ) {
      fxwarning( "Atom[ %d ] = None \n", i );
    }
  }

  WM_STATE                         = xa_atoms[ 0 ];
  WM_CHANGE_STATE                  = xa_atoms[ 1 ];
  WM_PROTOCOLS                     = xa_atoms[ 2 ];
  WM_DELETE_WINDOW                 = xa_atoms[ 3 ];
  WM_COLORMAP_WINDOWS              = xa_atoms[ 4 ];
  WM_TAKE_FOCUS                    = xa_atoms[ 5 ];
  UTF8_STRING                      = xa_atoms[ 6 ];
  _MOTIF_WM_HINTS                  = xa_atoms[ 7 ];
  _NET_SUPPORTED                   = xa_atoms[ 8 ];
  _NET_SUPPORTING_WM_CHECK         = xa_atoms[ 9 ];
  _NET_CLIENT_LIST                 = xa_atoms[ 10 ];
  _NET_CLIENT_LIST_STACKING        = xa_atoms[ 11 ];
  _NET_DESKTOP_GEOMETRY            = xa_atoms[ 12 ];
  _NET_DESKTOP_VIEWPORT            = xa_atoms[ 13 ];
  _NET_CURRENT_DESKTOP             = xa_atoms[ 14 ];
  _NET_NUMBER_OF_DESKTOPS          = xa_atoms[ 15 ];
  _NET_ACTIVE_WINDOW               = xa_atoms[ 16 ];
  _NET_WM_NAME                     = xa_atoms[ 17 ];
  _NET_WM_ICON                     = xa_atoms[ 18 ];
  _NET_WM_STATE                    = xa_atoms[ 19 ];
  _NET_WM_STATE_MODAL              = xa_atoms[ 20 ];
  _NET_WM_STATE_STICKY             = xa_atoms[ 21 ];
  _NET_WM_STATE_MAXIMIZED_VERT     = xa_atoms[ 22 ];
  _NET_WM_STATE_MAXIMIZED_HORZ     = xa_atoms[ 23 ];
  _NET_WM_STATE_SHADED             = xa_atoms[ 24 ];
  _NET_WM_STATE_SKIP_TASKBAR       = xa_atoms[ 25 ];
  _NET_WM_STATE_SKIP_PAGER         = xa_atoms[ 26 ];
  _NET_WM_STATE_HIDDEN             = xa_atoms[ 27 ];
  _NET_WM_STATE_FULLSCREEN         = xa_atoms[ 28 ];
  _NET_WM_STATE_ABOVE              = xa_atoms[ 29 ];
  _NET_WM_STATE_BELOW              = xa_atoms[ 30 ];
  _NET_WM_WINDOW_TYPE              = xa_atoms[ 31 ];
  _NET_WM_WINDOW_TYPE_DESKTOP      = xa_atoms[ 32 ];
  _NET_WM_WINDOW_TYPE_DOCK         = xa_atoms[ 33 ];
  _NET_WM_WINDOW_TYPE_TOOLBAR      = xa_atoms[ 34 ];
  _NET_WM_WINDOW_TYPE_MENU         = xa_atoms[ 35 ];
  _NET_WM_WINDOW_TYPE_UTILITY      = xa_atoms[ 36 ];
  _NET_WM_WINDOW_TYPE_SPLASH       = xa_atoms[ 37 ];
  _NET_WM_WINDOW_TYPE_DIALOG       = xa_atoms[ 38 ];
  _NET_WM_WINDOW_TYPE_NORMAL       = xa_atoms[ 39 ];
  _NET_WM_DESKTOP                  = xa_atoms[ 40 ];
  _NET_WM_STRUT                    = xa_atoms[ 41 ];
  _NET_WM_STRUT_PARTIAL            = xa_atoms[ 42 ];
  _KDE_NET_WM_WINDOW_TYPE_OVERRIDE = xa_atoms[ 43 ];
  _NET_CLOSE_WINDOW                = xa_atoms[ 44 ];
  _NET_RESTACK_WINDOW              = xa_atoms[ 45 ];
  _NET_WM_STATE_DEMANDS_ATTENTION  = xa_atoms[ 46 ];
  _NET_WM_STATE_FOCUSED            = xa_atoms[ 47 ];
  _NET_WM_USER_TIME_WINDOW         = xa_atoms[ 48 ];
  _NET_WM_USER_TIME                = xa_atoms[ 49 ];

  return true;
}
















