/*
 * Copyright (c) 2016 Lammert Bies
 * Copyright (c) 2013-2016 the Civetweb developers
 * Copyright (c) 2004-2013 Sergey Lyubka
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * ============
 * Release: 2.0
 */

#include "httplib_main.h"

/*
 * bool XX_httplib_is_put_or_delete_method( const struct httplib_connection *conn );
 *
 * The function XX_httplib_is_put_or_delete_method() returns true of the method
 * of the request on a connection is one which writes at the file level on the
 * server like PUT, DELETE, MKCOL and PATCH.
 */

bool XX_httplib_is_put_or_delete_method( const struct httplib_connection *conn ) {

	const char *s;

	if ( conn == NULL ) return false;

	s = conn->request_info.request_method;
	return  s != NULL  &&  ( ! strcmp( s, "PUT" )  ||  ! strcmp( s, "DELETE" )  ||  ! strcmp( s, "MKCOL" )  ||  ! strcmp( s, "PATCH" ) );

}  /* X_httplib_is_put_or_delete_method */
