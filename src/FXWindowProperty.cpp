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
// Standard library
#include<new>
#include<limits.h>

// Fox library
#include<xincs.h>
#include<fx.h>
//#include<FXArray.h>

// FXWM
#include<FXAtom.h>
#include<FXWindowProperty.h>
#include<FXServerError.h>

/*************************************************************************************************/
// PRIVATE API

FXWindowProperty::FXWindowProperty( FXApp *m, FXWindow *w )
{
  app             = m;
  WINDOW          = w->id();
  DISPLAY         = static_cast<Display*>( app->getDisplay( ) );
  actual_type     = None;
  actual_format   = 0;
  actual_numitems = 0;
  actual_numbytes = 0;
  property        = NULL;
}

FXWindowProperty::FXWindowProperty( FXApp *m, FXID id )
{
  app             = m;
  WINDOW          = id;
  DISPLAY         = static_cast<Display*>( app->getDisplay( ) );
  actual_type     = None;
  actual_format   = 0;
  actual_numitems = 0;
  actual_numbytes = 0;
  property        = NULL;
}

FXWindowProperty::~FXWindowProperty()
{ freeData( ); }

void FXWindowProperty::freeData( )
{
  actual_type     = None;
  actual_format   = 0;
  actual_numitems = 0;
  actual_numbytes = 0;
  if( property ) {
    XFree(property);
    property = NULL;
  }
}

FXbool FXWindowProperty::setRawProperty( Atom key, Atom type, FXint format, FXuchar *data, FXint nelements )
{
  FXServerError servererror( app );

  FXint result;
  result = XChangeProperty( DISPLAY, WINDOW, key, type, format, PropModeReplace, data, nelements );
  if( result != Success || !servererror.check( ) ) { return false; }
  fxwarning( "[ FXWindowProperty::setRawProperty( %d ) ] key: %s; type: %s ", ( int ) WINDOW, XGetAtomName( DISPLAY, key ), XGetAtomName( DISPLAY, type ) );
  return true;
}

FXbool FXWindowProperty::getRawProperty( Atom key, Atom type )
{
  FXint result;

  freeData( );
  result = XGetWindowProperty( DISPLAY, WINDOW, key, 0, ULONG_MAX, false,
                               type, &actual_type, &actual_format, &actual_numitems,
                               &actual_numbytes, ( FXuchar** ) &property );
  if ( ( result != Success ) || ( actual_type == None ) ) {
    freeData( );
    return false;
  }

  if ( actual_numbytes > 0 ) { fxwarning("Partial Transfer of Window Property not implemented\n"); }
  return true;
}


FXbool FXWindowProperty::validate(FXint expected_format,Atom expected_type,FXbool checkempty){
  if( ( expected_format == actual_format ) && ( expected_type == actual_type ) && ( !checkempty || actual_numitems > 0 ) ) {
    return true;
  }
  freeData( );
  return false;
}

/*************************************************************************************************/
// PUBLIC API

FXbool FXWindowProperty::setAtom( Atom key, Atom value )
{
  return setRawProperty(key, XA_ATOM, 32, ( FXuchar* ) &value, 1 );
}

FXbool FXWindowProperty::getAtom( Atom key, Atom &value )
{
  if( !getRawProperty( key, XA_ATOM ) ) { return false; }
  if( !validate( 32, XA_ATOM, true ) )  { return false; }

  value = *( Atom* ) property;
  return true;
}

FXbool FXWindowProperty::setAtomList( Atom key, FXAtomList &list)
{
  if( list.no( ) ) {
    return setRawProperty( key, XA_ATOM, 32, ( FXuchar* ) list.data( ), list.no( ) );
  }
  else {
    return deleteKey( key );
  }
}

FXbool FXWindowProperty::getAtomList( Atom key, FXAtomList &list ){
  Atom * atomlist=NULL;

  if ( !getRawProperty( key, XA_ATOM ) )
    return false;

  if ( !validate( 32, XA_ATOM ) )
    return false;

  atomlist = ( Atom* ) property;
  list.no( actual_numitems );

  /// Copy Atoms to destination List
  for( FXuint i = 0; i < actual_numitems; i++ ){
    list[ i ] = atomlist[ i ];
  }

  freeData( );
  return true;
}


FXbool FXWindowProperty::setString( Atom key, FXString value )
{
  return setRawProperty( key, XA_STRING, 8, ( FXuchar* ) value.text( ), value.length( ) );
}

FXbool FXWindowProperty::getString( Atom key, FXString &value )
{
  if( !getRawProperty( key, XA_STRING ) ) { return false; }
  if( !validate( 8, XA_STRING ) )         { return false; }

  value = ( FXchar* ) property;
  freeData( );
  return true;
}

FXbool FXWindowProperty::setStringList(Atom key,FXStringList & list){
  FXASSERT(list.no());
  FXString value;

  for (FXint i=0;i<list.no();i++){
    value += list[i];
    if (i<(list.no()-1)) value += '\0';
    }

  return setRawProperty(key,XA_STRING,8,(FXuchar*)value.text(),value.length());
  }

FXbool FXWindowProperty::getStringList(Atom key,FXStringList & list){
  if (!getRawProperty(key,XA_STRING))
    return false;

  if (!validate(8,XA_STRING))
    return false;

  FXString original_string((FXchar*)property,actual_numitems);
  FXString section;

  FXint num=0;
  while(1){
    section = original_string.section('\0',num);
    if (section.empty()) break;
    list.append(section);
    num++;
    }
  freeData();
  return true;
  }

FXbool FXWindowProperty::setUTF8String(Atom key,FXString value){
  return setRawProperty(key,FXAtom::UTF8_STRING,8,(FXuchar*)value.text(),value.length());
  }

FXbool FXWindowProperty::getUTF8String(Atom key,FXString & value){
  if (!getRawProperty(key,FXAtom::UTF8_STRING))
    return false;

  if (!validate(8,FXAtom::UTF8_STRING))
    return false;

  value = (FXchar*) property;
  freeData();
  return true;
  }


FXbool FXWindowProperty::setUTF8StringList(Atom key,FXStringList & list){
  FXASSERT(list.no());
  FXString value;

  for (FXint i=0;i<list.no();i++){
    value += list[i];
    if (i<(list.no()-1)) value += '\0';
    }

  return setRawProperty(key,FXAtom::UTF8_STRING,8,(FXuchar*)value.text(),value.length());
  }


FXbool FXWindowProperty::getUTF8StringList(Atom key,FXStringList & list){
  if (!getRawProperty(key,FXAtom::UTF8_STRING))
    return false;

  if (!validate(8,FXAtom::UTF8_STRING))
    return false;

  FXString original_string((FXchar*)property,actual_numitems);
  FXString section;

  FXint num=0;
  while(1){
    section = original_string.section('\0',num);
    if (section.empty()) break;
    list.append(section);
    num++;
    }
  freeData();
  return true;
  }


FXbool FXWindowProperty::setWindow(Atom key,FXID id){
  return setRawProperty(key,XA_WINDOW,32,(FXuchar*)&id,1);
  }

FXbool FXWindowProperty::getWindow(Atom key,FXID & id){
  if (!getRawProperty(key,XA_WINDOW))
    return false;

  if (!validate(32,XA_WINDOW,true))
    return false;

  id = *(FXID*)property;
  freeData();
  return true;
  }


FXbool FXWindowProperty::setWindowList(Atom key,FXWindowList & list){
  if (list.no())
    return setRawProperty(key,XA_WINDOW,32,(FXuchar*)list.data(),list.no());
  else
    return deleteKey(key);
  }

FXbool FXWindowProperty::getWindowList(Atom key,FXWindowList & list){
  FXID * windowlist=NULL;

  if (!getRawProperty(key,XA_ATOM))
    return false;

  if (!validate(32,XA_ATOM))
    return false;

  windowlist=(Atom*)property;
  list.no(actual_numitems);

  /// Copy Atoms to destination List
  for (FXuint i=0;i<actual_numitems;i++){
    list[i]=windowlist[i];
    }
  freeData();
  return true;
  }


FXbool FXWindowProperty::setCardinal(Atom key,FXuint value){
  return setRawProperty(key,XA_CARDINAL,32,(FXuchar*)&value,1);
  }

FXbool FXWindowProperty::getCardinal(Atom key,FXuint & value){
  if (!getRawProperty(key,XA_CARDINAL))
    return false;

  if (!validate(32,XA_CARDINAL,true))
    return false;

  value = *(FXuint*)property;
  freeData();
  return true;
  }



FXbool FXWindowProperty::setCardinalList(Atom key,FXIntegerList & list){
  if (list.no())
    return setRawProperty(key,XA_CARDINAL,32,(FXuchar*)list.data(),list.no());
  else
    return deleteKey(key);
  }


FXbool FXWindowProperty::getCardinalList(Atom key,FXIntegerList & list){
  FXuint * cardinallist=NULL;

  if (!getRawProperty(key,XA_CARDINAL))
    return false;

  if (!validate(32,XA_CARDINAL))
    return false;

  cardinallist=(FXuint*)property;
  list.no(actual_numitems);

  /// Copy Atoms to destination List
  for (FXuint i=0;i<actual_numitems;i++){
    list[i]=cardinallist[i];
    }
  freeData();
  return true;
  }


FXbool FXWindowProperty::getValueList(Atom key,FXIntegerList & list){
  FXuint * integerlist=NULL;

  if (!getRawProperty(key,key))
    return false;

  if (!validate(32,key))
    return false;

  integerlist=(FXuint*)property;
  list.no(actual_numitems);

  /// Copy Atoms to destination List
  for (FXuint i=0;i<actual_numitems;i++){
    list[i]=integerlist[i];
    }
  freeData();
  return true;
  }


/// Set Unsigned Integer for Key
FXbool FXWindowProperty::setValue(Atom key,FXuint value){
  return setRawProperty(key,key,32,(FXuchar*)(FXuint*)&value,1);
  }

/// Set Unsigned Integer List for Key
FXbool FXWindowProperty::setValueList(Atom key,FXIntegerList & list){
  return setRawProperty(key,key,32,(FXuchar*)list.data(),list.no());
  }

FXbool FXWindowProperty::deleteKey(Atom key){
    FXint result;
    result = XDeleteProperty(DISPLAY,WINDOW,key);
    if (result!=Success)
        return false;
    return true;
    }


FXbool FXWindowProperty::getKeys( FXAtomList &list ){
  Atom *atomlist = NULL;
  FXint num;

  atomlist = XListProperties( DISPLAY, WINDOW, &num );
  if( atomlist ) {
    list.no( num );
    for( FXint i = 0; i < num; i++ ) { list[ i ] = atomlist[ i ]; }
    XFree( atomlist );
  }
  else {
    list.no( 0 );
  }
  return true;
}











