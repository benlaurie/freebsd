#!/bin/sh
# $Id: edit_cfg.sh,v 1.10 2000/09/09 20:29:58 tom Exp $
##############################################################################
# Copyright (c) 1998,2000 Free Software Foundation, Inc.                     #
#                                                                            #
# Permission is hereby granted, free of charge, to any person obtaining a    #
# copy of this software and associated documentation files (the "Software"), #
# to deal in the Software without restriction, including without limitation  #
# the rights to use, copy, modify, merge, publish, distribute, distribute    #
# with modifications, sublicense, and/or sell copies of the Software, and to #
# permit persons to whom the Software is furnished to do so, subject to the  #
# following conditions:                                                      #
#                                                                            #
# The above copyright notice and this permission notice shall be included in #
# all copies or substantial portions of the Software.                        #
#                                                                            #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    #
# THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    #
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        #
# DEALINGS IN THE SOFTWARE.                                                  #
#                                                                            #
# Except as contained in this notice, the name(s) of the above copyright     #
# holders shall not be used in advertising or otherwise to promote the sale, #
# use or other dealings in this Software without prior written               #
# authorization.                                                             #
##############################################################################
#
# Author: Thomas E. Dickey <dickey@clark.net> 1996,1997,2000
#
# Edit the default value of the term.h file based on the autoconf-generated
# values:
#
#	$1 = ncurses_cfg.h
#	$2 = term.h
#
BAK=save$$
TMP=edit$$
trap "rm -f $BAK $TMP" 0 1 2 5 15
for name in \
	HAVE_TCGETATTR \
	HAVE_TERMIOS_H \
	HAVE_TERMIO_H \
	BROKEN_LINKER
do
	mv $2 $BAK
	if ( grep "[ 	]$name[ 	]" $1 2>&1 >$TMP )
	then
		sed -e 's@#define '$name'.*$@'"`cat $TMP`@" $BAK >$2
	elif test "$name" = "NCURSES_CONST" ; then
		sed -e 's/define '$name'.*$/define '$name' \/\*nothing\*\//' $BAK >$2
	else
		sed -e 's/define '$name'.*$/define '$name' 0/' $BAK >$2
	fi
	if (cmp -s $2 $BAK)
	then
		echo '** same: '$name
		mv $BAK $2
	else
		echo '** edit: '$name
		rm -f $BAK
	fi
done
