/*******************************************************************************
*                         fxwm - FOX Window Manager                            *
********************************************************************************
*        Copyright (C) 2002-2003 by Sander Jansen. All Rights Reserved         *
*        Copyright (C) 2014-2016 by D.A.Tiger All Rights Reserved              *
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
#ifndef __FXCLIENTWIDOW_H
#define __FXCLIENTWIDOW_H
#include<xincs.h>

class FXWindowManager;
class FXWindowProperty;
class WMDecoration;
class WMFrameButton;

enum fxclientfocusmode {
  FOCUSMODE_NONE=0,
  FOCUSMODE_PASSIVE,
  FOCUSMODE_LOCAL,
  FOCUSMODE_GLOBAL
};

enum fxwindowstate {    /// Stav okna :
  STATE_WITHDRAWN = 0,  // Okno je skryto
  STATE_NORMAL,         // Normalni
  STATE_SHADED,         // Stinovy (zarolovane)
  STATE_MINIMIZED,      // Minimalizovane
  STATE_MAXIMIZED,      // Maximalizovane
  STATE_FULLSCREEN,     // Celoobrazovkovy rezim
  STATE_CLOSE,          // Okno zavreno
};

enum fxwindowproperty {
  /// _wm
  PROPERTY_STICKY       = 0x00000001,
  PROPERTY_MODAL        = 0x00000002,
  PROPERTY_SKIPPAGER    = 0x00000003,
/*
  PROPERTY_DESKTOP      = 0x00000004,
  PROPERTY_DOCK         = 0x00000006,
  PROPERTY_TOOLBAR      = 0x00000008,
  PROPERTY_MENU         = 0x00000010,
  PROPERTY_UTILITY      = 0x00000012,
  PROPERTY_SPLASH       = 0x00000014,
  PROPERTY_DIALOG       = 0x00000016,
  PROPERTY_NORMAL       = 0x00000018,
  PROPERTY_DROPDOWNMENU = 0x00000020,
  PROPERTY_POPUPMENU    = 0x00000022,
  PROPERTY_TOOLTIP      = 0x00000024,
  PROPERTY_NOTIFICATION = 0x00000026,
  PROPERTY_COMBO        = 0x00000028,
  PROPERTY_DND          = 0x00000030
*/
};

enum fxwm_WindowClass { /// Trida (typ) WM top-level okna
  CLASS_NORMAL = 0,     // Normalni
  CLASS_DESKTOP,        // Desktop
  CLASS_DOCK,           // Dokovatelne
  CLASS_TOOLBAR,        // Panel
  CLASS_MENU,           // Menu
  CLASS_UTILITY,        // Pomocne (neco mezi panelem a dialogem - dle dokumentace :-( )
  CLASS_SPLASH,         // Uvodni
  CLASS_DIALOG,         // Dialog
  CLASS_DROPDOWNMENU,   //
  CLASS_POPUPMENU,      //
  CLASS_TOOLTIP,        // informacni okno k GUI prvkum
  CLASS_NOTIFICATION,   // samostane informacni okno
  CLASS_COMBO,          //
  CLASS_DND             //
};

//class FXClientWindow;

class FXClientWindow : public FXComposite {
FXDECLARE( FXClientWindow )
  friend class FXWindowManager;
  friend class FXWorkspace;
  friend class WMStack;

  FXFrame          *client;       /// Okno pro namapovani klientskeho obsahu (FXWindow)
  FXWindowProperty *xproperty;    /// XProperties klienta

  /// Client identify (Identita klienta)
  FXID    cid;            /// ID klienta
  FXID    group;          /// Skupinove ID klienta
  FXID    desktop;        /// ID desktopoveho okna
  FXint   workspace;      /// Index prirazene pracovni plochy
  Display *disp;          /// Window manager display

  /// WM Window properties (Vlastnosti okna )
  FXSize wmin;            /// Mininimalni rozmery okna ( 0 znamena, ze takovaou hodnotu nema )
  FXSize wmax;            /// Maximalni rozmery okna
  FXSize wincr;           /// Prirustek pri zmene velikosti okna
  FXSize wbase;           /// Zakladni rozmery okna
  FXPoint amin;           /// Minimalni pomer stran (min aspect)
  FXPoint amax;           /// Maximalni pomer stran (max aspect)

  FXint gravity;          /// Gravity;
  FXint ref_x;            /// Referencni bod (Gravity)
  FXint ref_y;            /// Referencni bod (Gravity)

  FXRectangle carea;      /// Oblast klienta
  FXRectangle ncarea;     /// Normalni Oblast klienta
  FXint       c_bw;       /// Client Sirka oramovani
  FXRectangle dragarea;   /// Nove parametry oblasti pri tazeni mysi
  FXPoint     spot;       /// Vychozi pozice pro tazeni mysi

  FXint spotx;            /// Vychozi X behem tazeni
  FXint spoty;            /// vychozi Y behem tazeni

  FXint leftstrut;        /// Strany prostoru vlevo
  FXint rightstrut;       /// Strany prostoru vpravo
  FXint topstrut;         /// Strany prostoru nahore
  FXint bottomstrut;      /// Strany prostoru dole

  /// WM Window Decorations (Dekorace okna)
  WMDecoration  *deco;
  FXIconCache   *decocache;

  FXuint        decor;          /// Priznaky vlastnosti dekorace okna
  FXuint        decor_ref;      /// Zaloha decoraci - treba pro prepnuti z fullscreenu do normalu
  FXString      title;          /// Titulek okna
  FXIcon       *windowicon;     /// Ikona okna
  FXFont       *font;           /// Font titulkoveho pruhu
  FXint         titlesize;      /// Velikost fontu titulkoveho pruhu
  FXint         bordersize;     /// Sirka ramu okna

  /// WM Window title bar (Titulkovy pruh)
  FXMDIMenu    *mdimenu;        /// Menu
  FXButton     *closebutton;    /// Tlacitko uzavreni okna
  FXButton     *maximizebutton; /// Tlacitko maximalizace okna
  FXButton     *minimizebutton; /// Tlacitko minimalizece okna
  FXButton     *restorebutton;  /// Tlacitko obnoveni okna
  FXMenuButton *menubutton;     /// Talictko pro zobrazeni menu okna
  FXVerticalFrame *wc_frame;    /// Ramec (nepouzito)

  /// WM Window state (stav okna)
  FXint   state;             /// Window State
  //FXuchar initialState;    /// Initial State of Client
  FXint   property;          /// Window Property
  FXuint  wclass;            /// Window class
  FXint   wpriority;         /// Priorita okna (-2,2 )
  FXuchar dragmode;          /// Drag mode
  FXuchar focusmode;         /// Focusmode for Client
  //FXbool  modal;           /// Indikate of modal window
  FXbool wm_delete_window;   ///
  FXbool wm_take_focus;      ///
  FXString type_text;        /// Text typu okna FIXME: DEBUG 01


  /// WM other window links
  //FXClientWindow *next_window;	     /// Next Window in List
  //FXClientWindow *previous_window;   /// Previous Window in List
  //FXClientWindow *transient_windows; /// List of Transient Windows;
  FXClientWindow *transientFor;      /// Transient Window

  /// Private methods
  FXClientWindow( );
  FXuchar where( FXint x, FXint y );
  void changeCursor( FXint x, FXint y );
  void revertCursor( );
  void toClientCoordinates( FXint & x, FXint & y, FXint & w, FXint & h );
  void toWindowCoordinates( FXint & x, FXint & y, FXint & w, FXint & h );
  FXClientWindow * getTransientWindow( );
  void calculateReferencePoint( );
  void positionFrameWindow( );
  void calculateGravity( );
  void configure( );
  void configure_class( );        /// Konfigurace podle typu okna

  /// Clients xproperties
  FXbool getMotifHints( );
  FXbool getNETHints( );
  FXbool getNETState( );
  void   getNETDesktop( );
  void   getNETStrut( );
  void   getNETIcon( );
  void   getWMHints( );
  void   getWMProtocols( );
  void   getWMNormalHints( );
  void   getTitle( );
  void   getDecorations();

public:
  FXClientWindow( FXComposite *p, FXID clientid, FXint workspace );
  virtual ~FXClientWindow( );

  ///////////////////////////////////////////////
  /// Access methods
  ///
  FXWindowManager* getWindowManager( ) { return ( FXWindowManager* ) getApp( ); }	// Ukazatel na instanci WM
  FXint getWorkspace( )                { return workspace; }				// Ukazatel na virtualni plochu
  void  setWorkspace( FXint value )    { workspace = value; }

  /// XEVENTS
  FXbool isEventForClient( FXID wnd );				// TRUE pokud je mozno udalost obslouzit
  void doConfigure( FXRawEvent &ev );				//
  void doEnter( FXRawEvent &ev );				//
  void updateProperty( FXRawEvent *ev /*Atom property*/ );	//

  /// WINDOW STATE
  inline FXbool isWindowClass( FXuint value ) { return ( wclass == value ); } ///FIXME:

  inline FXbool isHidden( )     { return ( !shown( ) ); }					// true, je-li okno skryto ( opak metody FXWindow::shown( ) )
  inline FXbool isFullscreen( ) { return ( ( state == STATE_FULLSCREEN ) ? true : false ); }	// true, pri aktivnim fullscreen modu
  inline FXbool isIconyfy( )    { return ( ( state == STATE_MINIMIZED )  ? true : false ); }	// true, pri ikonifikaci
  inline FXbool isMaximize( )   { return ( ( state == STATE_MAXIMIZED )  ? true : false ); }	// true, je-li okno maximalizovane
  inline FXbool isShading( )    { return ( ( state == STATE_SHADED )     ? true : false ); }	// true, je-li zobrazen pouze tytulkovy panel

  FXbool setInputFocus( );					                       // Nastavi oknu fokus
  virtual bool canFocus( ) const { return flags & FLAG_FOCUSED; }  // true, ma-li okno nastaven focus
  inline FXString Title( ) { getTitle( ); return title; }	       // Text titulku okna
  virtual FXint getDefaultHeight( );				               // Vychozi vyska okna
  virtual FXint getDefaultWidth( );				                   // Vychozi sirka okna

  FXint getTopPriority( ); /*{ return wpriority;}*/      // Vraci prioritu okna v razeni ve stacku
  inline FXint getLeftStrut( )   { return leftstrut;   }	//
  inline FXint getRightStrut( )  { return rightstrut;  }	//
  inline FXint getTopStrut( )    { return topstrut;    }	//
  inline FXint getBottomStrut( ) { return bottomstrut; }	//

  void print_info( const FXString &_fnc, const FXString &_msg, const FXString &_type = "INFO" );

  ///////////////////////////////////////////////
  /// Operations methods
  ///
  virtual void create( );	/// Vytvoreni top-level okna na obrazovce
  virtual void layout( );	/// Vypocet rozmisteni prvku okna (titulkovy pruh, klient, tlacitka)

  /// XNotifikace
  void notifyClient( );		// Zmena aktualni konfigurace klienta
  void notifyActivate( );	// Aktivace okna
  void notifyDeactivate( );	// Deaktivace okna
  void notifyFocus( );		// Zamereni okna
  void notifyClose( );		// Uzavreni okna
  void notifyState( );		// Stav okna

  /// Moznosti okna
  virtual void show( );	  // Zobrazit
  virtual void hide( FXuint _state = STATE_WITHDRAWN );	  // Skryt
  virtual void active( ); // Aktivovat

  void maximize( );	    // Maximalizovat
  void iconify( );	    // Ikonifikovat (minimalizovat)
  void deiconify( );	    // Obnovit ikonifikovane okno
  void restore( );	    // Obnovit puvodni rozmery okna
  void fullscreen( );	    // Fullscreen okno
  void shade( );	    // Srolovat (pouze titlebar)
  void unshade( );	    // Obnovit srolovane okno

  void modal( )     { property |= PROPERTY_MODAL;  }
  void sticky( );   // { property |= PROPERTY_STICKY; getWindowManager( )->client_append( this, -1 ); }
  void above( )     { }
  void below( )     { }
  void skipPager( );
  void raise( );	// Na vrchol stacku

  FXbool testProperty( FXuint value ) { return property & value; }
  ///////////////////////////////////////////////
  /// GUI Messages & handlers methods
  ///
  enum {
    ID_CLOSE_WINDOW = FXComposite::ID_LAST, // Zavrit
    ID_MAXIMIZE,                            // Maximalizovat
    ID_MINIMIZE,                            // Minimalizovat
    ID_RESTORE,                             // Obnovit predchozi stav ( mimo minimalizace )
    ID_FULLSCREEN,                          // Fullscreem mod
    ID_SHADE,

    ID_ACTIVATE,                            // Aktivovat / daktivovat
    ID_STATE,                               // Zmena stavu (modu) okna
    ID_CHANGESTATE                          // - " -
  };

  long onPaint(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onMotion(FXObject*,FXSelector,void*);
  long onEnter(FXObject*,FXSelector,void*);
  long onLeave(FXObject*,FXSelector,void*);
  long onFocusIn(FXObject*,FXSelector,void*);
  long onFocusOut(FXObject*,FXSelector,void*);

  long onCmdActivate( FXObject*, FXSelector, void* );
  long onCmdState( FXObject*, FXSelector, void* );
  long onCmdCloseWindow(FXObject*,FXSelector,void*);
  //long onCmdMinimize(FXObject*,FXSelector,void*);
  long onUpdMinimize(FXObject*,FXSelector,void*);
  //long onCmdMaximize(FXObject*,FXSelector,void*);
  long onUpdMaximize(FXObject*,FXSelector,void*);
  //long onCmdRestore(FXObject*,FXSelector,void*);
  long onUpdRestore(FXObject*,FXSelector,void*);

protected:
  enum {    // Priznak zachyceni okna mysi
    DRAG_NONE        = 0,				// Nezachyceno
    DRAG_TOP         = 1,				// Nahore
    DRAG_BOTTOM      = 2,				// Dole
    DRAG_LEFT        = 4,				// Vlevo
    DRAG_RIGHT       = 8,				// Vpravo
    DRAG_TOPLEFT     = ( DRAG_TOP | DRAG_LEFT ),	// Vlevo nahore
    DRAG_TOPRIGHT    = ( DRAG_TOP | DRAG_RIGHT) ,	// Vpravo nahore
    DRAG_BOTTOMLEFT  = ( DRAG_BOTTOM | DRAG_LEFT ),	// Vlevo dole
    DRAG_BOTTOMRIGHT = ( DRAG_BOTTOM | DRAG_RIGHT ),	// Vpravo dole
    DRAG_INVERTED    = 16,				// Inverzne
    DRAG_TITLE       = 32				// Titulek
  };

  virtual bool doesOverrideRedirect( ) const { return true;} //
  Display* getDisplay( );
};


#endif /* __FXCLIENTWIDOW_H */
