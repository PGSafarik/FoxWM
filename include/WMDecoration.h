#ifndef __WMDECORATION_H
#define __WMDECORATION_H

#include<fx.h>
#include<iostream>

//#include<FXClientWindow.h>

class WMDecoration : public FXObject {
FXDECLARE( WMDecoration )
  FXApp *d_app;

public :
  WMDecoration( FXApp *a );
  virtual ~WMDecoration( );

  ///////////////////////////////////////////////
  ///
  ///
  void draw_content( FXDC *dc, const FXRectangle &rect, FXbool focus );
  void draw_frame( FXDC *dc, const FXRectangle &rect, FXbool focus, FXint strong );
  void draw_titlebar( FXDC *dc, const FXRectangle &rect, FXbool focus );
  void draw_titletext( FXDC *dc, const FXString &text, const FXPoint &pos, FXbool focus  );


protected :
  WMDecoration( ) { }

};

#endif /* __WMDECORATION_H */
