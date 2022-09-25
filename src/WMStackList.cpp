#include<fx.h>
#include<WMStack.h>
#include<FXWindowManager.h>
#include<WMStackList.h>

FXbool WMStackList::_set( FXint id )
{
  FXbool resh = false;

  if( id >= 0 && id < no( ) ) {
    cwsi = id;
    resh = true;
  }

  return resh;
}

void WMStackList::_make( FXWindowManager *manager, FXint count )
{
  for( FXint i = 0; i != count; i++  ) { append( new WMStack( manager, i ) ); }
}

void WMStackList::_destroy( ) { }

WMStack* WMStackList::_find( const FXString &title )
{
  WMStack *found = NULL;
  if( !title.empty( ) ) {
    for( FXint i = 0; i != no( ); i++ ) {
       found = at( i );
       if( found->getTitle( ) == title ) { return found; }
    }
  }

  return NULL;
}

void WMStackList::ReasocWin( FXClientWindow *w, FXint wsi, FXbool ignore_sticky )
{ /* Fluxbox (BScreen::reassociateWindow)*/
  if( w != NULL ) {
    FXint oldWSid =  w->getWorkspace( );
    fxwarning( "step 2.1 \n" );
    if( wsi >= no( ) ) { wsi = _current( ); }
    if( !w->isIconyfy( ) && w->getWorkspace( ) == wsi ) { return; }

    if( w->isIconyfy( ) ) {
      fxwarning( "step 2.2 \n" );
      at( _current( ) )->removeClient( static_cast<WMTopWindow*>( w ) );
      at( wsi )->addClient( static_cast<WMTopWindow*>( w ) );
      fxwarning( "step 2.3 \n" );
    }
    else if( ignore_sticky || !w->testProperty( PROPERTY_STICKY ) ){
      fxwarning( "step 2.4 \n" );
      at( w->getWorkspace( ) )->removeClient( static_cast<WMTopWindow*>( w ) );
      at( wsi )->addClient( static_cast<WMTopWindow*>( w ) );
      fxwarning( "step 2.5 \n" );
    }
    //at( oldWSid )->handle( w, FXSEL( SEL_COMMAND, WMStack::ID_UPDATE ), NULL );
    //at( w->getWorkspace( ) )->handle( w, FXSEL( SEL_COMMAND, WMStack::ID_UPDATE ), NULL );
    fxwarning( "win %d reassociate in ws %d on ws %d \n", w->id( ), oldWSid, w->getWorkspace( ) );
  }
}

