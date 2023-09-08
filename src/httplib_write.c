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
 */

#include "httplib_main.h"

/*
 * int httplib_write( const struct httplib_context *ctx, struct httplib_connection *conn, const void *buffie, size_t lennie );
 *
 * The function httplib_write() writes a number of bytes over a connection.
 * The amount of characters written is returned. If an error occurs
 * the value 0 is returned.
 */

int httplib_write( const struct httplib_context *ctx, struct httplib_connection *conn, const void *buffie, size_t lennie ) {

	int64_t len;
	int64_t total;
	const char *buf;

	if ( ctx == NULL  ||  conn == NULL  ||  buffie == NULL  ||  lennie == 0 ) return 0;

	buf = buffie;
	len = (int64_t)lennie;

	total = XX_httplib_push_all( ctx, NULL, conn->client.sock, conn->ssl, buf, len );

	return (int)total;

}  /* httplib_write */
