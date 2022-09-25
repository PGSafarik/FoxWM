/*******************************************************************************
*                         fxwm - FOX Window Manager                            *
********************************************************************************
*        Copyright (C) 2002-2004 by Sander Jansen. All Rights Reserved         *
*                               ---                                            *
* This program is free software; you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by         *
* the Free Software Foundation; either version 2 of the License, or            *
* (at your option) any later version.                                          *
*                                                                              *
* This program is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of               *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
* GNU General Public License for more details.                                 *
* You should have received a copy of the GNU General Public License            *
* along with this program; if not, write to the Free Software                  *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    *
*******************************************************************************/
// Fox library
#include<fx.h>
#include<xincs.h>

// FXWM
#include<FXServerError.h>

#define DISPLAY ((Display*)(app->getDisplay()))

FXbool FXServerError::status = true;
/*
static int fxerrorhandler(Display* dpy,XErrorEvent* eev){
  char buf[256];

  // A BadWindow due to X_SendEvent is likely due to XDND
  ///if(eev->error_code==BadWindow && eev->request_code==25) return 0;

  // WM_TAKE_FOCUS causes sporadic errors for X_SetInputFocus
  //if(eev->request_code==42) return 0;

  // Get error codes
  XGetErrorText(dpy,eev->error_code,buf,sizeof(buf));

  FXServerError::status = false;

  // Print out meaningful warning
  fxwarning("X Error: code %d major %d minor %d: %s.\n",eev->error_code,eev->request_code,eev->minor_code,buf);
  return 1;
  }


// Fatal error (e.g. lost connection)
static int fxfatalerrorhandler(Display*){
  fxerror("X Fatal error.\n");
  return 1;
  }
*/

FXServerError::FXServerError(FXApp * a) : app(a) {
  status=true;
  }

FXServerError::~FXServerError(){
  app=NULL;
  status=false;
  }

FXbool FXServerError::check(FXbool sync){
  if (sync)
    XSync(DISPLAY,false);

  return status;
  }



