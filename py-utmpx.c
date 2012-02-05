/*
 *  Copyright (c) 2011 Jakub Szafrański <samu@pirc.pl>
 * 
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 */
 
 /*
  *  FreeBSD 9.0 Python UTMPX Parse module
  *  
  *  Compile this with:
  *  gcc -shared -L/usr/local/lib -I/usr/local/include/python2.7 -lpython2.7 -o pylast.so py-utmpx.c -fPIC
  * 
  */

#include <string.h>
#include <unistd.h>
#include <utmpx.h>
#include <stdio.h>

#define UTX_ACTIVE	"/var/run/utx.active"
#define UTX_LASTLOGIN	"/var/log/utx.lastlogin"
#define UTX_LOG		"/var/log/utx.log"

#include <Python.h>


static PyObject* pyutmpx(PyObject* self, PyObject* args)
{

    PyObject *ret = PyList_New(0);
    PyObject *l_utmp = PyList_New(0);
    PyObject *l_wtmp = PyList_New(0);
    struct utmpx *ut;
    
    setutxdb(UTXDB_ACTIVE, UTX_ACTIVE);
    while ((ut = getutxent()) != NULL) PyList_Append(l_utmp, Py_BuildValue("sssi", ut->ut_line, ut->ut_user, ut->ut_host, ut->ut_tv));
    endutxent();

    setutxdb(UTXDB_LOG, UTX_LOG);
    while ((ut = getutxent()) != NULL) PyList_Append(l_wtmp, Py_BuildValue("sssi", ut->ut_line, ut->ut_user, ut->ut_host, ut->ut_tv));
    endutxent();

    PyList_Append(ret, Py_BuildValue("N", l_utmp));
    PyList_Append(ret, Py_BuildValue("N", l_wtmp));
    return Py_BuildValue("N", ret);
}
static PyMethodDef pylast_methods[] = {
	{"utmpx", pyutmpx, METH_VARARGS},
	{NULL, NULL}
};

void initpylast()
{
	(void) Py_InitModule("pyutmpx", pylast_methods);
}

