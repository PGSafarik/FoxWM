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
#ifndef FXAtom_H
#define FXAtom_H

class FXAtom {
public:
  /* ICCCM */
  static Atom WM_STATE;             ///
  static Atom WM_CHANGE_STATE;      ///
  static Atom WM_PROTOCOLS;         ///
  static Atom WM_DELETE_WINDOW;     ///
  static Atom WM_COLORMAP_WINDOWS;  ///
  static Atom WM_TAKE_FOCUS;        ///
  /* */
  static Atom UTF8_STRING;
  /* EWMH */
  static Atom _MOTIF_WM_HINTS;                  ///
  static Atom _NET_SUPPORTED;                   /// ROOT {WM} - seznam podporovanych vlastnosti WM
  static Atom _NET_SUPPORTING_WM_CHECK;         /// NET  {WM} - Indikuje aktivniho spravce oken
  static Atom _NET_CLIENT_LIST;                 /// ROOT {WM} - seznam oken zpravovanych WM. Zacina od nejstarsiho okna
  static Atom _NET_CLIENT_LIST_STACKING;        /// ROOT {WM} - seznam oken zpravovanych WM. Poradi prekryvani zdola-nahoru
  static Atom _NET_WM_DESKTOP;                  /// ROOT {cl mess} - Client informuje WM o tom ze je (nebo chce byt) destopem
  //static Atom _NET_DESKTOP_NAMES;            /// ROOT {WM} - Seznam jmen vsech virtualnich ploch (not implement)
  static Atom _NET_DESKTOP_GEOMETRY;            /// ROOT {WM} - Pole o dvou hodnotach, definujici spolecnou velikost ploch
  static Atom _NET_DESKTOP_VIEWPORT;            /// ROOT {WM} - Pole o dvou hodnotach, definujici levy horni roh pro kazdou plochu. MESSAGE - Pozadavek na aktivaci okna
  static Atom _NET_CURRENT_DESKTOP;             /// ROOT {WM} - Index aktivni virtualni plochy
  static Atom _NET_NUMBER_OF_DESKTOPS;          /// ROOT {WM} - Informuje o pocetu virtualnich ploch
  static Atom _NET_ACTIVE_WINDOW;               /// ROOT {cl mess} - Aktivace konkretniho okna
  static Atom _NET_RESTACK_WINDOW;              /// ROOT {cl mess} - Zmena pozice okna ve stacku (odeslana z pageru, nemela by byt ignorovana)
  static Atom _NET_WM_NAME;                     /// WIN PROP {WM} - Titulek okna (UTF-8)
  //static Atom _NET_WM_VISIBLE_NAME;            /// WIN PROP {WM} - Viditelny Titulek okna (UTF-8)
  static Atom _NET_WM_ICON;                     /// WIN PROP {WM} - Ikona okna
  //static Atom _NET_WM_VISIBLE_ICON;           /// WIN PROP {WM} - Ikona okna
  static Atom _NET_WM_STATE;                    /// APP WIN PROP {WM} - Stav okna (seznam)
  static Atom   _NET_WM_STATE_MODAL;            ///     - Modalni dialogove okno
  static Atom   _NET_WM_STATE_STICKY;           ///     - fixni pozice vuci hlavnimu oknu
  static Atom   _NET_WM_STATE_MAXIMIZED_VERT;   ///     - Maximalni vyska okna (verticalni rozmer)
  static Atom   _NET_WM_STATE_MAXIMIZED_HORZ;   ///     - Maximalni sirka okna (horizontalni rozmer)
  static Atom   _NET_WM_STATE_SHADED;           ///     - Stinove okno (vetsinou je z okna zobrazen pouze titulkovy pruh, tzv. zarolovane okno)
  static Atom   _NET_WM_STATE_SKIP_TASKBAR;     ///     - Nezobrazovat okno v panelech
  static Atom   _NET_WM_STATE_SKIP_PAGER;       ///     - Nezobrazovat okno v pageru
  static Atom   _NET_WM_STATE_HIDDEN;           ///     - Skryte okno ( okno neni zobrazeno na obrazovce, avsak existuje a lze jej zpristupnit)
  static Atom   _NET_WM_STATE_FULLSCREEN;       ///     - Celoobrazovkovy rezim okna ( Okno je zbaveno dekoraci, posunuto na pocatecni pozici hl. okna a nasledne zvetseno do jeho velikosti. Prekryje veskera dalsi okna )
  static Atom   _NET_WM_STATE_ABOVE;            ///     - Udrzovat okno "nahore"
  static Atom   _NET_WM_STATE_BELOW;            ///     - Udrzovat okno "ve spod"
  static Atom   _NET_WM_STATE_DEMANDS_ATTENTION;///     - Indikuje, ze byla provedena nejaka akce s oknem
  static Atom   _NET_WM_STATE_FOCUSED;          ///     - Indikuje aktivni stav dekoraci okna
  static Atom _NET_WM_WINDOW_TYPE;              /// APP WIN PROP {WM} Typ okna (seznam)
  static Atom   _NET_WM_WINDOW_TYPE_DESKTOP;    ///     - Desktopove okno
  static Atom   _NET_WM_WINDOW_TYPE_DOCK;       ///     - Dokovaci okno
  static Atom   _NET_WM_WINDOW_TYPE_TOOLBAR;    ///     - Okno toolbaru
  static Atom   _NET_WM_WINDOW_TYPE_MENU;       ///     - Okno menu
  static Atom   _NET_WM_WINDOW_TYPE_UTILITY;    ///     -
  static Atom   _NET_WM_WINDOW_TYPE_SPLASH;     ///     - Uvodni okno
  static Atom   _NET_WM_WINDOW_TYPE_DIALOG;     ///     - Dialogove okno
//  static Atom   _NET_WM_WINDOW_TYPE_DROPDOWN_MENU;     ///     -
//  static Atom   _NET_WM_WINDOW_TYPE_POPUP_MENU;     ///     -
//  static Atom   _NET_WM_WINDOW_TYPE_TOOLTIP;     ///     -
//  static Atom   _NET_WM_WINDOW_TYPE_NOTIFICATION;     ///     -
//  static Atom   _NET_WM_WINDOW_TYPE_COMBO;     ///     -
//  static Atom   _NET_WM_WINDOW_TYPE_DND;     ///     -
  static Atom   _NET_WM_WINDOW_TYPE_NORMAL;     ///     - Normalni okno
  static Atom _NET_WM_STRUT;                    /// APP WIN PROP {WM} Rezervace prostoru pro okno
  static Atom _NET_WM_STRUT_PARTIAL;            /// APP WIN PROP {WM} Rezervace prostoru pro okno
  static Atom _NET_CLOSE_WINDOW;                /// OTHER ROOT {WM} Zadost o zavreni okna
//  static Atom _NET_MOVERESIZE_WINDOW;              /// OTHER ROOT {WM} - Zmena pozice ci velikosti okna
//  static Atom _NET_WM_MOVERESIZE;              /// OTHER ROOT {WM} - Zmena pozice ci velikosti okna
//  static Atom _NET_RESTACK_WINDOW;              /// OTHER ROOT {WM} - Zmena pozice okna ve stacku
//  static Atom _NET_REQUEST_FRAME_EXTENTS;              /// OTHER ROOT {WM} - Zadost okna o odhad velikosti a pozic ramu pred jeho namapovanim
  static Atom _NET_WM_USER_TIME_WINDOW;         ///
  static Atom   _NET_WM_USER_TIME;              /// Cas posledni aktivity okna
  /* extension */
  static Atom _KDE_NET_WM_WINDOW_TYPE_OVERRIDE; /// Okno bez dekoraci (KDE)

public:
  static FXbool init( FXApp* );
};

#endif










