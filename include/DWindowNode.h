#ifndef __DWINNODE_H
#define __DWINNODE_H

#include<fx.h>
#include<iostream>

#include<FXClientWindow.h>

template<class Wintype> class DWinNode : public Wintype {
public :
  DWinNode<Wintype> *next;	    // Ukazatel na dalsi uzel ve stacku
  DWinNode<Wintype> *prev;	    // Ukazatel na predchzajici uzel
  DWinNode<Wintype> *translist;	// Ukazatel na seznam transientnich oken
  DWinNode<Wintype> *transfor;	// Je-li okno transientni, ukazuje na koho
  FXArray<FXbool>    _status;   // Tato promena umoznuje prirazovat oknu status v ramci konkretniho stacku

  DWinNode( FXComposite *p, FXID clientid, FXint stackid, FXint stacksnum )
  : Wintype( p, clientid, stackid )
  { // inicializace uzlu seznamu
    this->next = NULL;
    this->prev = NULL;
    this->translist = NULL;
    this->transfor  = NULL;

    _status.no( stacksnum );
    for( FXint i = 0; i != stacksnum; i++ ) { _status[ i ] = false; }
  }

  void LinkAfter( DWinNode<Wintype> *node )
  { // Linkovat za zadany uzel
    this->prev = node;
    if( node != NULL ) {
      this->next = node->next;
      if( this->next != NULL ) { this->next->prev = this; }
      node->next = this;
    }
    CycleTest( "LinkAfter" );
  }

  void LinkBefore( DWinNode<Wintype> *node )
  { // Slinkovat pred zadany uzel
    this->next = node;
    if( node != NULL ) {
      this->prev = node->prev;
      if( this->prev != NULL ) { this->prev->next = this; }
      node->prev = this;
    }
    CycleTest( "LinkBefore" );
  }

  FXbool Swap( DWinNode<Wintype> *node )
  { // Funkce provede prelinkovani (zamenu) se zadanym uzlem
    FXbool reshult = false;
    if( node != NULL ) {
      DWinNode<Wintype> *_next = this->next;
      DWinNode<Wintype> *_prev = this->prev;
			
      this->Unlink( );
	  this->LinkAfter( node );
			
      node->Unlink( );
	  if( _next != NULL ) { node->LinkAfter( _next ); }
	  else if( _prev != NULL ) { node->LinkBefore( _prev ); }
			 
			/*
      this->next = node->next;
      this->prev = node->prev;
      node->next = _next;
      node->prev = _prev;
			*/
      reshult = true;
    }
    CycleTest( "Swap" );
    return reshult;
  }

  void Unlink( )
  { // Odpojit uzel ze senamu
    if( this->prev != NULL ) { this->prev->next = this->next; }
    if( this->next != NULL ) { this->next->prev = this->prev; }
    this->prev = this->next = NULL;
  }

  FXbool LinkAsTransient( DWinNode<Wintype> *wnode )
  { // Linkovat jako transientni k danemu uzlu
    FXbool resh = false;

    if( wnode != NULL ) {
      if( wnode->translist == NULL ) { wnode->transient = this; }
      else { this->LinkAfter( wnode->LastTransient( ) ); }
      this->transfor = wnode;
      resh = true;
    }

    return resh;
  }

  DWinNode<Wintype>* LastTransient( )
  { // Vrati ukazatel na posledni uzel v seznamu transientnich oken (jsou-li nejaka)
    DWinNode<Wintype> *wnode = NULL;

    if( this->translist != NULL ) {
      wnode = this->translist;
      while( wnode->next != NULL ) { wnode = wnode->next; }
    }

    return wnode;
  }

  ~DWinNode( )
  {
    this->Unlink( );
  }

protected:
  void CycleTest( const FXString &fnc )
  {
    if( this->next == this ) { std::cout << "DWinNode::" << fnc.text( ) << "]: Nalezen cyklicky odkaz ukazatelem \'next\'! ID: " << this->id( ) << std::endl; }
    if( this->prev == this ) { std::cout << "DWinNode::" << fnc.text( ) << "]: Nalezen cyklicky odkaz ukazatelem \'prev\'! ID: " << this->id( ) << std::endl; }
    std::cout.flush( );
  }
};


#endif /* __DWINNODE_H */
