/*-
 * Copyright (c) 1995 Jean-Marc Zucconi
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software withough specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#ifndef	_SYS_JOYSTICK_H_
#define	_SYS_JOYSTICK_H_

#include <sys/ioccom.h>

struct joystick {
    int x;
    int y;
    int b1;
    int b2;
};

#define JOY_SETTIMEOUT    _IOW('J', 1, int)    /* set timeout */
#define JOY_GETTIMEOUT    _IOR('J', 2, int)    /* get timeout */
#define JOY_SET_X_OFFSET  _IOW('J', 3, int)    /* set offset on X-axis */
#define JOY_SET_Y_OFFSET  _IOW('J', 4, int)    /* set offset on X-axis */
#define JOY_GET_X_OFFSET  _IOR('J', 5, int)    /* get offset on X-axis */
#define JOY_GET_Y_OFFSET  _IOR('J', 6, int)    /* get offset on Y-axis */

#endif /* !_SYS_JOYSTICK_H_ */
