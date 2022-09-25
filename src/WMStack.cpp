
// standard library
#include<new>
#include<iostream>

// Fox Library
#include<xincs.h>
#include<fx.h>
#include<FXArray.h>

// FXWM
#include<FXWindowManager.h>
#include<FXClientWindow.h>
#include<WMStack.h>
#include<FXAtom.h>

//#define DISPLAY ((Display*)((manager->getDisplay())))

FXDEFMAP( WMStack ) STACKMAP[ ] = {
  FXMAPFUNC( SEL_COMMAND, WMStack::ID_RAISE, WMStack::onCmd_Notify ),
  FXMAPFUNC( SEL_COMMAND, WMStack::ID_RESTACK, WMStack::onCmd_Notify ),
  FXMAPFUNC( SEL_COMMAND, WMStack::ID_UPDATE, WMStack::onCmd_Notify ),
  FXMAPFUNC( SEL_COMMAND, WMStack::ID_REPAINT, WMStack::onCmd_Notify )
};
FXIMPLEMENT( WMStack, FXObject, STACKMAP, ARRAYNUMBER( STACKMAP ) )

/**************************************************************************************************/
WMStack::WMStack( FXWindowManager *wm, FXint id )
{
  s_display = NULL;
  s_wm      = wm;
  s_id      = id;
  s_title   = FXString::null;
  s_first   = NULL;
  s_last    = NULL;
  s_hide    = false;
  s_space   = new WMStrutSpace;
  s_space->reset( );

}

WMStack::~WMStack( )
{

}

/**************************************************************************************************/
void WMStack::addClient( WMTopWindow *wnode )
{
  // Kontrola zda je dane okno konkretni instance stacku zpravovano
  if( wnode->_status[ s_id ] == true ) { return; }
  wnode->_status[ s_id ] = true;

  // Asociace s konkretnim stackem
  wnode->setWorkspace( s_id );

  if( AddTransientWindow( wnode ) != true ){
    if( wnode->isWindowClass( CLASS_DESKTOP ) == true ) { PrependWindow( wnode ); }
    else {
      if( ( wnode->isWindowClass( CLASS_DOCK ) == true ) || ( wnode->isWindowClass( CLASS_TOOLBAR ) == true ) ) {
        AppendWindow( wnode );
      }
      else { InsertWindow( wnode ); }
    }
  }
}

void WMStack::removeClient( WMTopWindow *wnode )
{
  if( RemoveTransient( wnode ) != true ) { RemoveWindow( wnode ); }
  else {
    if( s_first != NULL ) { s_last->handle( this, FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE ), NULL ); }
  }

  wnode->_status[ s_id ] = false;
}
/*************************************************************************************************/
void WMStack::show( )
{
  fxwarning( "[WMStack::show]: BEGIN \n" );
  if( s_title.empty( ) ) { s_title = "Fox Workspace"; }
  if( s_display == NULL ) { s_display = static_cast<Display*>( s_wm->getDisplay( ) ); }
  if( s_hide == true ){
    visibleAllWindow( s_last, true );
    s_hide = false;
  }
  fxwarning( "[WMStack::show]: END \n" );
}

void WMStack::hide( )
{
  fxwarning( "[WMStack::hide]: BEGIN \n" );
  if( s_hide == false ){
    visibleAllWindow( s_last, false );
    s_hide = true;
  }

  fxwarning( "[WMStack::hide]: END \n" );
}

void WMStack::lower( WMTopWindow *wnode )
{
  fxwarning( "[WMStack::lower]: BEGIN (not implement) \n" );
  fxwarning( "[WMStack::lower]: END \n" );
}

void WMStack::raise( WMTopWindow *wnode )
{ // Dane okno posune co nejvice dopredu
  //fxwarning( "[WMStack::raise]: BEGIN \n" );
  WMTopWindow *w = wnode;
  WMTopWindow *f = s_last;

  while( w->transientFor != NULL ) { w = static_cast<WMTopWindow*>( w->transientFor ); } // Tak dlouho dokud w neukazuje na rodice tr. oken
  //fxmessage( "\tRAISE Window: %d - %s\n", w->id( ), w->title.text( ) );
  while( ( f != NULL ) && ( f != w ) ) {
    //fxmessage( "\tRAISE Check: %d - %s\n", f->id( ), f->title.text( ) );
    if( f->getTopPriority( ) <= w->getTopPriority( ) ) {
      //fxmessage( "\tRAISE: Remove Window\n" );
      RemoveWindow( w );
      //fxmessage( "\tRAISE: Insert Window\n" );
      InsertWindow( w );
      //fxmessage( "\tRAISE: Restack\n" );
      Restack( );
      break;
    }
    f = f->prev;
  }
  //fxwarning( "[WMStack::raise]: END \n" );
}

void WMStack::getMaximizeRange( FXint &x, FXint &y, FXint &width, FXint &height )
{
  //fxwarning( "[WMStack::getMaximizeRange]: BEGIN \n" );
    x = s_space->left;
    y = s_space->top;
    width  = s_wm->getRootWindow( )->getWidth( )  - s_space->right;
    height = s_wm->getRootWindow( )->getHeight( ) - s_space->bottom;
  //fxwarning( "[WMStack::getMaximizeRange]: END \n" );
}

/*************************************************************************************************/
long WMStack::onCmd_Notify( FXObject *sender, FXSelector sel, void *data )
{
   long resh = 1;

   switch( FXSELID( sel ) ) {
     // Widow call raising self
     case WMStack::ID_RAISE :
     {
       if( data != NULL ) {
         raise( static_cast<WMTopWindow*>( data ) );
         resh = 0;
       }
       break;
     }
     case WMStack::ID_REPAINT :
     {
       repaintAllWindow( s_last, (FXRawEvent*) data  );
       break;
     }
     case WMStack::ID_UPDATE :
     {
       Update( );
       break;
     }
     case WMStack::ID_RESTACK :
     {
       Restack( );
       break;
     }


   }

   return resh;
}


/*************************************************************************************************/
void WMStack::AppendWindow( WMTopWindow *wnode )
{ // prida okno na dno stacku
  //fxwarning( "[WMStack::AppendWindow]: BEGIN \n" );
  if( ( s_last != wnode ) || ( wnode->_status[ s_id ] != false ) ) {
    if( s_first == NULL ) { s_first = s_last = wnode; }
    else {
      wnode->LinkAfter( s_last );
      s_last = wnode;
    }
  }
  //fxwarning( "[WMStack::AppendWindow]: END \n" );
}

void WMStack::PrependWindow( WMTopWindow *wnode )
{ // Zaradi okno za pocatek (koren, vrchol) stacku
  //fxwarning( "[WMStack::PrependWindow]: BEGIN \n" );
  if( s_first != wnode  || ( wnode->_status[ s_id ] != false ) ) {
    wnode->LinkBefore( s_first );
    s_first = wnode;
    if( s_last == NULL ) { s_last = s_first; }
  }
  //fxwarning( "[WMStack::PrependWindow]: END \n" );
}

void WMStack::InsertWindow( WMTopWindow *wnode )
{ // Vlozi okno do stacku dle jeho priority
  //fxwarning( "[WMStack::InsertWindow]: BEGIN \n" );
  if( wnode->_status[ s_id ] == false ) { return; }
  //Zaciname od vrchulu stacku (Tedy od nejvzdalenejsiho okna od korenu )
  WMTopWindow *p = s_last;

  if( p == NULL ) {
    // Stack je prazdny => wnode je tedy prvni okno ve stacku
    s_first = s_last = wnode;
  }
  else {
    while( p ) {
      // Hleda se prvni okno ode dna stacku, ktere ma stejnou, nebo nizsi prioritu, jako wnode.
      if( wnode->getTopPriority( ) >= p->getTopPriority( ) ) {
        // Nenasleduje-li za nim zadny potomek, je nutno wnode pripojiti na dno stacku
        if( p->next == NULL ) { AppendWindow( wnode ); }
        // V opacnem pripade bude pripojen hned za tento uzel
        else { wnode->LinkAfter( p ); }
        // hledany uzel byl nalezen, okno zarazeno => navrat z funkce
        return;
      }
      p = p->prev;
    }
    // Zde je jasne, ze se jedna o okno zatim s nejnizsi prioritou, tudiz jej nalikujeme na koren stacku
    PrependWindow( wnode );
  }
  //fxwarning( "[WMStack::InsertWindow]: END \n" );
}

void WMStack::RemoveWindow( WMTopWindow *wnode )
{ // Odebere okno ze stacku (nikoliv vsak z pameti!!!)
  //fxwarning( "[WMStack::RemoveWindow]: BEGIN \n" );
  if( wnode->_status[ s_id ] == false ) { return; }
  if( s_first != NULL ) {
    if   ( ( wnode->next == NULL ) && ( wnode->prev == NULL ) ) { s_first = s_last = NULL; }
    else if( wnode->next == NULL ) { s_last = wnode->prev; }
    else if( wnode->prev == NULL ) { s_first = wnode->next;  }
    wnode->Unlink( );
    if( s_first != NULL ) { s_last->handle( this, FXSEL( SEL_COMMAND, FXClientWindow::ID_ACTIVATE ), NULL ); }
  }
  else { std::cout << "pozadevek nelze provest, nebot zadne okno neni zarazeno do seznamu" << std::endl; }

  //fxwarning( "[WMStack::RemoveWindow]: END \n" );
}

FXbool WMStack::AddTransientWindow( WMTopWindow *wnode )
{ // Pripoji dane okno jako tranzientni
  //fxwarning( "[WMStack::AddTransientWindow]: BEGIN \n" );
  FXbool resh = false;

  if( wnode->_status[ s_id ] != false ) {
    WMTopWindow *p = static_cast<WMTopWindow*>( wnode->transientFor );

    if( p != NULL ) {
      if( p->translist == NULL ) { p->translist = wnode; }
      else { wnode->LinkAfter( p->LastTransient( ) ); }
      resh = true;
    }
  }
  //fxwarning( "[WMStack::addTransientWindow]: END \n" );
  return resh;
}

FXbool WMStack::RemoveTransient( WMTopWindow *wnode )
{ // Odebere zadane okno ze senamu transientnich oken
  //fxwarning( "[WMStack::RemoveTransient]: BEGIN \n" );
  FXbool resh = false;

  if( ( wnode->transientFor != NULL ) && ( wnode->_status[ s_id ] != false )  ) {
    WMTopWindow *p = static_cast<WMTopWindow*>( wnode->transientFor );
    if   ( ( wnode->next == NULL ) && ( wnode->prev == NULL ) ) { p->translist = NULL; }
    else if( wnode->prev == NULL ) { p->translist = wnode->next;  }
    wnode->Unlink( );
    resh = true;
  }
  //fxwarning( "[WMStack::RemoveTransient]: END \n" );
  return resh;
}

/*************************************************************************************************/
void WMStack::ListWindow( WMIdList *list, WMTopWindow *start )
{ // Projde stack a vytvori seznam Id top-level oken
  //fxwarning( "[WMStack::ListWindow]: BEGIN \n" );
  WMTopWindow *p = ( ( start == NULL ) ? s_last : start );

  while( p != NULL ) {
    if( p->translist != NULL ) { ListWindow( list, p->LastTransient( ) ); }
    list->append( p->id( ) );
    p = p->prev;
  }

  //fxwarning( "[WMStack::ListWindow]: END \n" );
}

void WMStack::NotifyClients( )
{
  //fxwarning( "[WMStack::NotifyClientList]: BEGIN \n" );
  WMIdList wl;

  ListWindow( &wl );
  XChangeProperty( s_display, s_wm->getRootWindow( )->id( ), FXAtom::_NET_CLIENT_LIST_STACKING, XA_WINDOW, 32, PropModeReplace, (unsigned char* ) wl.data( ), wl.no( ) );
  //fxwarning( "[WMStack::NotifyClientList]: END \n" );
}

void WMStack::Restack( )
{
  //fxwarning( "[WMStack::Restack]: BEGIN \n" );
  WMIdList wl;

  ListWindow( &wl );
  XRestackWindows( s_display, wl.data( ), wl.no( ) );
  //fxwarning( "[WMStack::Restack]: BEGIN \n" );
}

void WMStack::UpdateStrut( )
{
  //fxwarning( "[WMStack::updateStruct]: BEGIN \n" );
  WMTopWindow *p = s_last;

  s_space->reset( );
  while( p != NULL ) {
    //s_space->calc( p );
   s_space->left   += p->getLeftStrut( );
   s_space->right  += p->getRightStrut( );
   s_space->top    += p->getTopStrut( );
   s_space->bottom += p->getBottomStrut( );
   p = p->prev;
   /*
    if( p != p->prev ) { p = p->prev; }
    else {
      Test_CNode( "UpdateStrut", p );
      p = NULL;
    }*/
  }
  //fxwarning( "[WMStack::updateStruct]: BEGIN \n" );
}

void WMStack::Update( )
{
  //fxwarning( "[WMStack::Update]: BEGIN \n" );
  WMIdList wl;

  ListWindow( &wl );
  XRestackWindows( s_display, wl.data( ), wl.no( ) );
  UpdateStrut( );
  XChangeProperty( s_display, s_wm->getRootWindow( )->id( ), FXAtom::_NET_CLIENT_LIST_STACKING, XA_WINDOW, 32, PropModeReplace, (unsigned char* ) wl.data( ), wl.no( ) );
  //fxwarning( "[WMStack::Update]: END \n" );
}

void WMStack::repaintAllWindow( WMTopWindow *start, FXRawEvent *e )
{ // Projde stack a vytvori seznam Id top-level oken
  //fxwarning( "[WMStack::ListWindow]: BEGIN \n" );
  if( start == NULL ) { return; }
  //std::cout << "STACK REPAINT" << std::endl;
  WMTopWindow *p = start;

  while( p != NULL ) {
    if( p->translist != NULL ) { repaintAllWindow( p->LastTransient( ), e ); }
    p->handle( this, FXSEL( SEL_PAINT, 0 ), e );
    p = p->prev;
  }

  //fxwarning( "[WMStack::ListWindow]: END \n" );
}

void WMStack::visibleAllWindow( WMTopWindow *start, FXbool visible )
{
  std::cout << "STACK VISIBLE - " << ( ( visible == true ) ? "Show" : "Hide" ) << std::endl;
  if( start == NULL ) { return; }
  WMTopWindow *p;
  FXint id;
  p = start;


  while( p != NULL ) {
    if( p->translist != NULL ) { visibleAllWindow( p->LastTransient( ), visible ); }
    if( visible == true ) { p->restore( ); } else { p->hide( ); }
    std::cout << "\t " << p->id( ) << std::endl;
    p = p->prev;
  }
}

FXint WMStack::Test_CNode( const FXString &func, WMTopWindow *n )
{
   FXint res = 0;
   if( n->next == n ) { res = 1; }
   if( n->prev == n ) { res = -1; }

   if( res != 0 ) {
     std::cout << "[WMStack::" << func.text( ) << "] Window " << n->id( ) << ", " << n->Title( ).text( ) << std::endl;
     std::cout << "Detekovano Zacyklene okno ukazatelem \'" << ( ( res > 0 ) ? "next" : "prev"  ) << "\'" << std::endl;
   }

   return res;
}

