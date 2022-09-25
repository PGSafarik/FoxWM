namespace Decorations {

class FXWM_Engine {
  FXuint    flags;          /// Decorations property flag
  FXuint    default_flg;    /// Zaloha decoraci - treba pro prepnuti z fullscreenu do normalu
  FXString  title;          /// Widow title string
  FXIcon   *windowicon;     /// Window icon
  FXFont   *font;           /// Title bar font
  FXint     titlesize;      /// Title bar font size
  FXint     bordersize;     /// Size of Window frame

public :
  FXWM_Engine( FXClientWindow *client );
  virtual ~FXWM_Engine( ) { }

/////////////////////////////////////////////////
/// Operatins method
///
  void     set_decor( FXuint flag );
  void     set_default( FXuint flag )
  FXuint   get_decor( FXbool def = FALSE );
  void     set_titlesize( FXint value );
  FXint    get_titlesize( );
  void     set_bordersize( FXint value );
  FXint    get_bordersize( );
  void     set_text( const FXString &text );
  FXString get_text( );


/////////////////////////////////////////////////
/// Operatins method
///
  virtual void upadate( FXuint decor_state )
  {

  }

  virtual void draw(  )
  {

  }

};




/*
class FXWM_Decorations_XFCE : public FXObject {
FXDECLARE( FXWM_Decorations)
  FXString title;	/// Titulek
  FXint    titlesize;	/// Velikost titulkoveho fontu
  FXFont   *font;	/// Titulkovy font

  FXint   bordersize;	/// Sirka ramu

  FXImage *frame_img[ 6 ];	 /// Obrazky ramce okna
  FXIcon  *buttons_img[ 5 ];	 /// Obrazky titlkovych tlacitek
  FXColor  colors_active[ 3 ];   /// barvy aktivniho okna ( Back, fore, font)
  FXColor  colors_deactive[ 3 ]; /// barvy neaktivniho okna

public:
  FXWM_Decorations( FXWindowClient *win, FXuint opts );
  virtual ~FXWM_Decorations( );



protected:




};
*/

}
