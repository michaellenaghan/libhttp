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
 * void XX_httplib_handle_file_based_request( struct httplib_context *ctx, struct httplib_connection *conn, const char *path, struct file *file );
 *
 * The function XX_httplib_handle_file_based_request() handles a request which
 * involves a file.
 */

void XX_httplib_handle_file_based_request( struct httplib_context *ctx, struct httplib_connection *conn, const char *path, struct file *file ) {

	if ( ctx == NULL  ||  conn == NULL ) return;

	if ( ctx->static_file_max_age > 0  &&  ! conn->in_error_handler  &&  XX_httplib_is_not_modified( ctx, conn, file ) ) {

		XX_httplib_handle_not_modified_static_file_request( ctx, conn, file );
	}

	else XX_httplib_handle_static_file_request( ctx, conn, path, file, NULL, NULL );

}  /* XX_httplib_handle_file_based_request */
