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
#include<DWindowNode.h>

class FXWindowManager;
class FXClientWindow;
class FXAtom;

typedef FXArray<FXID>       FXWindowList;
typedef DWinNode<FXClientWindow> WMTopWindow;

class FXWorkspace : public FXObject {
FXDECLARE(FXWorkspace)
  FXWindowManager * manager;  /// Window Manager.
  FXString          title;    /// Title of the Workspace

  FXClientWindow *  back;	  /// beginning of the client list ( Pocatek seznamu klientu - Nevzdalenejsi okno )
  FXClientWindow *  front;    /// end of the client list       ( Konec seznamu klientu   - Nejblizsi okno )

  FXint             ls;       /// Left Strut Space
  FXint             rs;       /// Right Strut Space
  FXint             ts;       /// Top Strut Space
  FXint             bs;       /// Bottom Strut Space

  FXWorkspace( );
public:
  FXWorkspace( FXWindowManager *manager );
  virtual ~FXWorkspace( );

  void notifyClientList( FXWindowList *list = NULL );
  void updateStrut( );
  void restack( );
  void show( );
  void hide( );

  void addClient( /*FXClientWindow */ WMTopWindow *window);
  void removeClient( FXClientWindow *window);
  void removeTransient( FXClientWindow *w );
  void listWindows( FXClientWindow *, FXWindowList * );
  void raise( FXClientWindow *window);
  void lower( FXClientWindow *window);

  void getMaximizeRange( FXint &x,FXint &y, FXint &w, FXint &h );

protected:
  void removeWindow( FXClientWindow * );                                         /// Odstranit okno ze seznamu
  void addWindowFront( FXClientWindow * );                                       /// Vlozit na konec seznamu
  void addWindowFrontMost( FXClientWindow * );                                   /// Zaradi za konec seznamu
  void addTransientWindow( FXClientWindow *parent, FXClientWindow *transient);   /// Priradit oknu transientni okno
  FXClientWindow* getLastTransient( FXClientWindow * );                          /// Posledni transietni okno (je-li nejake)

  Display *disp;
};
