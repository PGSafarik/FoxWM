#ifndef __WMFRAMEBUTTON_H
#define __WMFRAMEBUTTON_H

#include<fx.h>
#include<iostream>


class WMFrameButton : public FXButton {
  FXDECLARE( WMFrameButton )

  FXIconCache      *db_cache;
  FXString          db_name;

  WMFrameButton( const FXMDIDeleteButton& );
  WMFrameButton& operator =( const FXMDIDeleteButton& );

public:
  WMFrameButton( FXComposite* p, const FXString &label, FXIconCache *cache, FXObject* tgt = NULL, FXSelector sel = 0, FXuint opts = FRAME_RAISED , FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0 );

  ///////////////////////////////////////////////
  ///
  ///
  long onPaint( FXObject*, FXSelector, void* );

  ///////////////////////////////////////////////
  ///
  ///
  virtual FXint getDefaultWidth( );
  virtual FXint getDefaultHeight( );

protected:
  WMFrameButton(){}

};

#endif /* __WMFRAMEBUTTON_H */


