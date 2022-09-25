
class WMNetWindow : public FXMainWindow {
FXDECLARE( WMNetWindow )
  WindowManager   *nw_app;		// Instance aplikace zpravce oken
  WMWorkspaceList *nw_workspace;	// Seznam stacku oken

public :
  WMNetWindow( WindowManager *a );
  virtual ~WMNetWindow( );

  ///////////////////////////////////////////////
  ///
  ///
  FXint    workspace_Num( );
  FXint    workspace_Current( );
  WMStack* workspace_Get( FXint id );
  WMStack* workspace_Aktive( FXint id );

  ////////////////////////////////////////////////
  ///
  ///
  virtual void create( );
  FXbool ClientInsert( FXID *clientid );
  FXbool ClientRemove( FXClientWindow *window, FXID *clientid );


  ////////////////////////////////////////////////
  //
  //
  enum {
    // NetWindow
    ID_LOAD = FXMainWindow::ID_LAST // Nastaveni vlastnosti zpravy oken
    ID_QUIT,						// Ukonceni zpravce oken

    // Sluzby virtualnich pracovnich ploch (Virtual WorkSpace - VWS)
    WORKSPACE_INSERT,	// Pridat novou VWS
    WORKSPACE_ERASE,	// Odebrat zvolenou VWS
    WORKSPACE_NEXT,		// Prepnout se na dalsi VWS v bufferu
    WORKSPACE_PREV,		// Prepnout se predchazejici VWS v bufferu
    WORKSPACE_FIRST,    // Prepnuti na prvni VWS v buferu
    WORKSPACE_LAST,     // Prepnuti na posledni VWS v bufferu
    WORKSPACE_DESCTOP,	// Skryt vsechna normalni nmapovana okna v aktualni VWS
    WORKSPACE_NOTIFY,   // Predani informaci o vlastnostech VWS
    WORKSPACE_SHOW,     // Zobrazit okna VWS

    // Zprava klientskych top-level oken
    CLIENT_ADD,			// Pridat nove okno klienta
    CLIENT_REMOVE,		// Odebrat okno klienta
    CLIENT_RAISE,		// Posunout okno klienta do popredi
    CLIENT_RESTACK,		// Restacking konkretniho okna, nebo vsech
    CLIENT_ACTIVED,		// Okno klienta je aktivni
    CLIENT_NEXT,		// Aktivovat dalsi okno v poradi (je-li takove, jinak ignorovano)
    CLIENT_PREV,      	// Aktivovat predesle okno ve stacku (je-li nejake, jinak desctop, pager, root)
    CLIENT_PARENT,		// Aktivovat rodice okna ve stacku (je-li nejake, jinak desctop, root)
    CLIENT_CHILD,		// Aktivovat potomka okna (je-li takovy, jinak ignorovano)
    CLIENT_BEGIN,		// Aktivace desktopu, ci root okna

    // A co dal... ?
    ID_LAST
  };

  long OnCmd_Main( FXObject *sender, FXSelector sel, void *data ):
  long OnCmd_Clients( FXObject *sender, FXSelector sel, void *data ):
  long OnCmd_Workspaces( FXObject *sender, FXSelector sel, void *data ):

protected :
  FXRootWindow* GetRootWindow( ) { return nw_app->getRootWindow( ); }
  FXID GetRootWindowId( ) { return nw_app->getRootWindow( )->id( ); }

};
