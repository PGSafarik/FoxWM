#include<WMDecoration.h>

FXIMPLEMENT( WMDecoration, FXObject, NULL, 0 );

/*************************************************************************************************/
WMDecoration::WMDecoration( FXApp *a ) : FXObject( ), d_app( a )
{

}

WMDecoration::~WMDecoration( )
{

}


/*************************************************************************************************/
void WMDecoration::draw_content( FXDC *dc, const FXRectangle &rect, FXbool focus )
{
   dc->setForeground( d_app->getBaseColor( ) );
   dc->fillRectangle( rect.x, rect.y, rect.w, rect.h );
}

void WMDecoration::draw_frame( FXDC *dc, const FXRectangle &rect, FXbool focus, FXint strong )
{
    dc->setForeground( d_app->getBaseColor( ) );
    dc->fillRectangle( 0, 0, rect.w - 1, 1 );
    dc->fillRectangle( 0, 0, 1, rect.h - 2 );

    dc->setForeground( d_app->getHiliteColor( ) );
    dc->fillRectangle( 1, 1, rect.w - 2, 1 );
    dc->fillRectangle( 1, 1, 1, rect.h - 2 );

    dc->setForeground( d_app->getShadowColor( ) );
    dc->fillRectangle( 1, rect.h - 2, rect.w - 1, 1 );
    dc->fillRectangle( rect.w - 2, 1, 1, rect.h - 2 );

    dc->setForeground( d_app->getBorderColor( ) );
    dc->fillRectangle( 0, rect.h - 1, rect.w, 1 );
    dc->fillRectangle( rect.w - 1, 0, 1, rect.h );
}

void WMDecoration::draw_titlebar( FXDC *dc, const FXRectangle &rect, FXbool focus )
{
    dc->setForeground( ( ( focus == true ) ? d_app->getSelbackColor( ) : d_app->getShadowColor( ) ) );
    dc->fillRectangle( rect.x, rect.y, rect.w, rect.h );
}

void WMDecoration::draw_titletext( FXDC *dc, const FXString &text, const FXPoint &pos, FXbool focus  )
{
  FXint length = text.length( );
  FXFont *fnt  = d_app->getNormalFont( );

  if( length == 0 ) { length = 1; }

  dc->setForeground( d_app->getSelforeColor( ) );
  dc->setFont( fnt );
  dc->drawText( pos.x, pos.y, text.text( ), length );
}
