dnl    configure.ac - Autoconf script 
dnl    
dnl    Copyright 2015  Monaco F. J.   <monaco@icmc.usp.br>
dnl
dnl    This file is part of POSIXeg. 
dnl
dnl    POSIXeg is free software: you can redistribute it and/or modify
dnl    it under the terms of the GNU General Public License as published by
dnl    the Free Software Foundation, either version 3 of the License, or
dnl    (at your option) any later version.
dnl
dnl    POSIXeg is distributed in the hope that it will be useful,
dnl    but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl    GNU General Public License for more details.
dnl
dnl    You should have received a copy of the GNU General Public License
dnl    along with .  If not, see <http://www.gnu.org/licenses/>.
dnl

AC_INIT([Nutshell], [0.1], [emanuelvalente@gmail.com])     dnl Init Autoconf
AC_PREREQ([2.65])                              dnl Required autoconf version    
AC_CONFIG_AUX_DIR([build-aux])                 dnl Where to write auxiliary files                                                                              
AC_CONFIG_MACRO_DIR([m4])                      dnl m4 files here       
AC_CONFIG_HEADERS([config.h])                  dnl Generate config.h   

AM_INIT_AUTOMAKE([-Wall -Werror 1.14.1 subdir-objects])	dnl Init Automake options

AM_PROG_AR                      dnl Archiver for static lib                     
LT_INIT                         dnl Libtool support in 'configure'  

AC_PROG_CC			dnl Define CC

dnl Define variables to be used by Automake

AC_SUBST([CPP_FLAGS],"--ansi -D_POSIX_C_SOURCE=200112L -Werror --pedantic-errors -Wall") dnl Define flags
AC_SUBST([C_FLAGS])
AC_SUBST([LD_FLAGS])

AC_CONFIG_FILES([Makefile shell/Makefile runcmd/Makefile])	dnl Name output files
AC_OUTPUT			dnl Do output

