#ifndef __WMSTACK_H
#define __WMSTACK_H

//#include<fx.h>
//#include<iostream>
#include<DWindowNode.h>

class FXWindowManager;
class FXClientWindow;
class FXAtom;

typedef FXArray<FXID>            WMIdList;
typedef DWinNode<FXClientWindow> WMTopWindow;

struct WMStrutSpace {
 FXint left;
 FXint right;
 FXint top;
 FXint bottom;

 inline void reset( ) { left = right = top = bottom = 0; }
};

class WMStack : public FXObject {
FXDECLARE( WMStack )
  Display         *s_display;	//
  FXWindowManager *s_wm;     	// Zpravce oken
  FXint            s_id;        // ID Stacku;
  FXString         s_title;  	// titulek stacku (je-li nejaky)
  WMTopWindow     *s_first;   	// Pocatek seznamu klientu - Nevzdalenejsi okno od korenoveho
  WMTopWindow     *s_last;  	// Konec seznamu klientu   - Nejblizsi okno ke korenovemu
  WMStrutSpace    *s_space;  	//
  FXbool           s_hide;      //

public :
  WMStack( FXWindowManager *wm, FXint id = 0 );
  virtual ~WMStack( );

  void addClient( WMTopWindow *wnode );
  void removeClient( WMTopWindow *wnode );

  void show( );
  void hide( );
  void lower( WMTopWindow *wnode );
  void raise( WMTopWindow *wnode );
  void getMaximizeRange( FXint &x, FXint &y, FXint &width, FXint &height );

  FXString getTitle( ) { return s_title; }
  void     setTitle( const FXString &value ) { s_title = value; }
  enum {
    ID_RAISE = 1,
    ID_REPAINT,
    ID_UPDATE,
    ID_RESTACK,
    ID_LAST
  };
  long onCmd_Notify( FXObject *sender, FXSelector sel, void *data );

protected :
  WMStack( ) { }

  void   AppendWindow( WMTopWindow *wnode );  // Pridat okno na konec stacku
  void   InsertWindow( WMTopWindow *wnode );  // Vlozit okno do stacku
  void   PrependWindow( WMTopWindow *wnode ); // Vlozit okno na pocatek stacku
  void   RemoveWindow( WMTopWindow *wnode );  // Odebrat okno

  FXbool AddTransientWindow( WMTopWindow *wnode );
  FXbool RemoveTransient( WMTopWindow *wnode );

  void   ListWindow( WMIdList *list, WMTopWindow *start = NULL );
  void   NotifyClients( );
  void   Restack( );
  void   Update( );
  void   UpdateStrut( );


  void   repaintAllWindow( WMTopWindow *start = NULL, FXRawEvent *e = NULL );
  void   visibleAllWindow( WMTopWindow *start, FXbool visible );

  FXint  Test_CNode( const FXString &func, WMTopWindow *n );
};



#endif /* __WMSTACK_H */
