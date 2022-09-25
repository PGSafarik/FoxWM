#ifndef __WMSTACKLIST_H
#define __WMSTACKLIST_H

class WMStack;
class FXWindowManager;
class FXClientWindow;

class WMStackList : public FXObjectListOf<WMStack> {
  FXint cwsi; // Cislo aktualniho stacku virtualni plochy ( Current Workspace Stack Index )

public :
    WMStackList( ): cwsi( 0 ) { }

    WMStack* operator( ) ( ) { return this->at( _current( ) ); }

   ///////////////////////////////////////////////
   //  Implementacni rutiny pro rizeni CWSI
   //
   FXbool _set( FXint id );
   void  _next( )    { if( _set( cwsi + 1 ) == false ) { _first( ); } }
   void  _prev( )    { if( _set( cwsi - 1 ) == false ) { _last( ); } }
   void  _first( )   { cwsi = 0; }
   void  _last( )    { cwsi = no( ) - 1; }
   FXint _current( ) { return cwsi; }

   void _make( FXWindowManager *manager, FXint count = 1 );
   void _destroy( );

   WMStack* _find( const FXString &title );
   void ReasocWin( FXClientWindow *w, FXint wsi, FXbool ignore_sticky = false );

};

#endif /* __WMSTACKLIST_H */
