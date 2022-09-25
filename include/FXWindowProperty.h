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
#ifndef FXWINDOWPROPERTY_H
#define FXWINDOWPROPERTY_H

typedef FXArray<FXID>       FXWindowList;
typedef FXArray<Atom>       FXAtomList;
typedef FXArray<FXString>   FXStringList;
typedef FXArray<FXuint>     FXIntegerList;


/// Property Manager for a Window.
/// -------------------------------
/// No memory allocated by X will be returned by any public function. All memory allocated by X is
/// automatically destroyed by FXWindowProperty.

class FXWindowProperty {
// In order to retrieve/store window properties we need the following things:
//   - DISPLAY which is stored in FXWindowManager
//   - The target Window Id
//   - Additional Atoms which are also stored in FXWindowManager
//   - Error Manager also from FXWindowManager
    FXApp   *app;
    FXID     WINDOW;
    Display *DISPLAY;
    Atom        		actual_type;        /// Actual Type of Data
    FXint       		actual_format;      /// Actual Format of Data
    long unsigned int   actual_numitems;    /// Number of Items Read (8,16,32)
    long unsigned int   actual_numbytes;    /// Number of Bytes left to read
    FXuchar*    		property;           /// Property Data

    FXbool  setRawProperty( Atom key, Atom type, FXint format, FXuchar *data, FXint nelements );
    FXbool  getRawProperty( Atom key, Atom type );
    FXbool  validate( FXint expected_format, Atom expected_type, FXbool checkempty = false );
    void    freeData( );

    FXWindowProperty( ) { }

public:
  FXWindowProperty(FXApp *m, FXWindow *w );
  FXWindowProperty(FXApp *m, FXID id );
  virtual ~FXWindowProperty( );

//-------------------------------------------------------------
  /// Set Atom for Key
  FXbool setAtom( Atom key, Atom value );
  /// Return Atom for Key
  FXbool getAtom( Atom key, Atom &value );
  /// Set Atom List for Key
  FXbool setAtomList( Atom key, FXAtomList &list );
  /// Return Atom List for Key
  FXbool getAtomList( Atom key, FXAtomList &list );

//-------------------------------------------------------------
  /// Set String for Key
  FXbool setString( Atom key, FXString value );
  /// Return String for Key
  FXbool getString( Atom key, FXString &value );
  /// Set String List for Key
  FXbool setStringList( Atom key, FXStringList &list );
  /// Return String List for Key
  FXbool getStringList( Atom key, FXStringList &list );

//-------------------------------------------------------------
  /// Set UTF8 String for Key
  FXbool setUTF8String( Atom key, FXString value );
  /// Return UTF8 String for Key
  FXbool getUTF8String( Atom key, FXString &value );
  /// Set UTF8 String List for Key
  FXbool setUTF8StringList( Atom key,FXStringList &list );
  /// Return UTF8 String List for Key
  FXbool getUTF8StringList( Atom key, FXStringList &list );

//-------------------------------------------------------------
  /// Set Window ID for Key
  FXbool setWindow( Atom key, FXID id );
  /// Return Window ID for Key
  FXbool getWindow( Atom key, FXID &id );
  /// Set Window ID List for Key
  FXbool setWindowList( Atom key, FXWindowList &id );
  /// Return Window ID List for Key
  FXbool getWindowList( Atom key, FXWindowList &id );

//-------------------------------------------------------------
  /// Set Cardinal for Key
  FXbool setCardinal( Atom key, FXuint value );
  /// Return Cardinal for Key
  FXbool getCardinal( Atom key, FXuint &value );
  /// Set Cardinal List for Key
  FXbool setCardinalList( Atom key, FXIntegerList &list );
  /// Return Cardinal List for Key
  FXbool getCardinalList( Atom key, FXIntegerList &list );

//-------------------------------------------------------------
  /// For settings unsigned integers to keys where key and type are the same.
  /// Set Unsigned Integer for Key
  FXbool setValue( Atom key, FXuint value );
  /// Set Unsigned Integer List for Key
  FXbool setValueList( Atom key, FXIntegerList &value );
  /// Get Value List for key/type
  FXbool getValueList( Atom key, FXIntegerList &list );

//-------------------------------------------------------------
  /// Delete Key
  FXbool deleteKey( Atom key );
  /// Return a List of Keys defined for this Window
  FXbool getKeys( FXAtomList &list );
};

#endif










