/*******************************************************************************
*                         fxwm - FOX Window Manager                            *
********************************************************************************
*        Copyright (C) 2002-2003 by Sander Jansen. All Rights Reserved         *
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
********************************************************************************/
// Fox library
#include<xincs.h>
#include<fx.h>
#include<FXArray.h>

// FXWM
#include<FXWindowManager.h>

int main(int argc,char *argv[]){

  if (FOX_MAJOR!=fxversion[0] || FOX_MINOR!=fxversion[1] || FOX_LEVEL!=fxversion[2]) {
    fxerror("Error FOX Header/Library mismatch!\nThe FOX Window Manager was compiled with header files from FOX v%d.%d.%d but linked to FOX v%d.%d.%d\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL,fxversion[0],fxversion[1],fxversion[2]);
    return 1;
    }

  fxwarning("[ ::main( ) ]: Using FOX v%d.%d.%d\n",fxversion[0],fxversion[1],fxversion[2]);


  // Make application
  FXWindowManager application;

  //Init application... get rid of fox specific flags
  application.init(argc,argv);


  // Create it
  application.create();

//  application.addSignal(SIGSEGV,&application,FXApp::ID_QUIT);
//  application.addSignal(SIGFPE,&application,FXApp::ID_QUIT);
//  application.addSignal(SIGTERM,&application,FXApp::ID_QUIT);
  application.addSignal(SIGINT,&application,FXApp::ID_QUIT);
//  application.addSignal(SIGCHLD,&application,FXApp::ID_QUIT);
//  application.addSignal(SIGHUP,&application,FXApp::ID_QUIT);

  return application.run();
  }




