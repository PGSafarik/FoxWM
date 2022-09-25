# FoxWM - The FOX window manager
-25.09.2022-

*Content:*
  1. [General info](#1-general-info)
  2. [Compilation and installation](#2-compilation-and-installation)
  3. [Known errors and bugs](#3-known-errors-and-bugs)
  4. [The project state and plans for the future](#4-project-state-and-plans-for-the-futhure)
  5. [Author and project licencion](#5-Author-and-project-licencion)
  6. [Internet](#6-internet)
  
<p align="center"><img src="docs/images/screenshot.png?raw=true" width="70%" />  

## 1. GENERAL INFO:
!PROJECT STOPPED! see on [chapter 4](#4-project-state-and-plans-for-the-futhure)

Documentation its in working process...

## 2. COMPILATION AND INSTALLATION:
*Dependecies:*
  * [Foxtoolkit 1.7.76](http://fox-toolkit.org/ftp/fox-1.7.76.tar.gz)
  * [{DEPENDENCY}{VERSION}]({LINK})
  
*Aditional dependencies for compilation:*
  * cmake >= 3.10
  * g++
  * {DEPENDENCY}{VERSION}
  
After downloading the project from Github (and possibly unzipping it), go to the folder with the project and enter the following command sequence:
```
  mkdir build
  cd build 
  cmake .. && make
  sudo make install
```
After successfully compiling and building the project, the resulting program, including the Lua language launcher, 
will be installed in the directory /usr/local/bin

Note:
The foxlib library MUST be installed on your system at compile time. Version 1.7 is a development version, so it is possible that you will not have it on your system and it will not be available in the repository, so you will have to install it manually. More information on how to do this can be found on the foxtoolkit website. The following lines refer to any CMakeLists modifications.

If you have the library installed in a location other than the standard one, this fact must be entered in the CMakeLists.txt file in the project directory. Find and change the lines setting the library path:
``` writen by Foxlib
    INCLUDE_DIRECTORIES( "/Cesta/vasi/instalaci/foxlib/include" )   #Foxlib headers
    LINK_DIRECTORIES( "/Cesta/vasi/instalaci/foxlib/lib" )          #Foxlib library
```
These lines are commented out by default, so remove the leading '#' characters after editing, and then save the file. Next, follow the instructions for building and installing the project above as normal.

## 3. KNOWN ERRORS AND BUGS
* {DD/MM/20YY} - {SIMPLE DESCRIPTION / NAME } - OPEN
  {DESCRIPTION}

## 4. PROJECT STATE AND PLANS FOR THE FUTHURE
All modifications in the project are of a maintenance nature at the moment. However, the repository of the project on Github will be preserved for any interested parties, or for future generations to know that such a project ever existed...

## 5. AUTHOR AND PROJECT LICENCION:
Copyright (C) 2002-2004 by Sander Jansen. <sander@knology.net>
Copyright (C) 2018 - 2022 Pavel Šafařík <drakarax@seznam.cz>
All rights reseved.

This file is part of Gorgona.

Gorgona is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gorgona is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the 
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with foxGHI. If not, see 
<https://www.gnu.org/licenses/>.

## 6. INTERNET
  * [Author's blog (CZ)](http://bfuplusplus.blogspot.cz/)
  * [{NAME} {(LANG)}]({LINK})

=== END ===
