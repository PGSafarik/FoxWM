#include<WMFrameButton.h>
#include<iostream>

#define MENUBUTTONWINDOW_WIDTH  16
#define MENUBUTTONWINDOW_HEIGHT 14

FXDEFMAP( WMFrameButton ) WMFrameButtonMap[ ] = {
  FXMAPFUNC( SEL_PAINT, 0, WMFrameButton::onPaint ),
};
FXIMPLEMENT(WMFrameButton, FXButton, WMFrameButtonMap, ARRAYNUMBER( WMFrameButtonMap ) )

/**************************************************************************************************/
WMFrameButton::WMFrameButton( FXComposite* p, const FXString &label, FXIconCache *cache, FXObject* tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h )
             : FXButton( p, FXString::null, NULL, tgt, sel, opts, x, y, w, h ), db_cache( cache )
{
  db_name = label;
  FXString text;
  text = "\t" + label;
  this->setText( text );
}

/**************************************************************************************************/
long WMFrameButton::onPaint( FXObject*, FXSelector, void *data )
{
   FXString _id;
   FXIcon *_ic = NULL;
   FXDCWindow dc( this, ( FXEvent* ) data );

   dc.setForeground( backColor );
   dc.fillRectangle( 0, 0, width, height );
   dc.setForeground( FXRGB( 0, 0, 0 ) );
   dc.drawRectangle( 0, 0, width-1, height-1 );

   if( db_cache != NULL ) {
     if( state == STATE_UP ) { _id = "pressed"; }
     else { _id = ( ( this->isEnabled( ) == true  ) ? "active" : "inactive" ); }
     _id = db_name + "-" + _id + ".xpm";
     _ic = db_cache->find( _id );
     if( _ic != NULL ) {
       setShape( _ic );
       dc.drawIcon( _ic, 0, 0 );
     }
     else { std::cout << "icon not in artcache with decoration" << std::endl; }
   }

   return 1;
}

/**************************************************************************************************/
FXint WMFrameButton::getDefaultWidth( )
{
  FXIcon * _icon = db_cache->find( db_name + "-active.xpm" );
  FXint w = ( ( _icon != NULL ) ? _icon->getWidth( ) : MENUBUTTONWINDOW_WIDTH );
  return padleft + padright + ( border << 1 ) + w;
}

FXint WMFrameButton::getDefaultHeight( )
{
  FXIcon * _icon = db_cache->find( db_name + "-active.xpm" );
  FXint h = ( ( _icon != NULL ) ? _icon->getHeight( ) : MENUBUTTONWINDOW_HEIGHT );
  return padtop + padbottom + ( border << 1 ) + h;
}

/**************************************************************************************************/



