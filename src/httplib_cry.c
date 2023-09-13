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
#include "httplib_ssl.h"

/*
 * void httplib_cry( enum httplib_debug debug_level, struct httplib_context *ctx, const struct httplib_connection *conn, const char *fmt, ... );
 *
 * The function httplib_cry() prints a formatted error message to the opened
 * error log stream. It first tries to use a user supplied error handler. If
 * that doesn't work, the alternative is to write to an error log file.
 */

void httplib_cry( enum httplib_debug debug_level, struct httplib_context *ctx, const struct httplib_connection *conn, const char *fmt, ... ) {

	char buf[MG_BUF_LEN];
	char src_addr[IP_ADDR_STR_LEN];
	va_list ap;
	struct file fi;
	time_t timestamp = time( NULL );

	/*
	 * Check if we have a context. Without a context there is no callback
	 * and also other important information like the path to the error file
	 * is missing. No need to continue if that information cannot be
	 * retrieved.
	 */

	if ( ctx == NULL ) return;

	/*
	 * Check if the message is severe enough to display. This is controlled
	 * with a context specific debug level.
	 */

	if ( debug_level > ctx->debug_level ) return;

	/*
	 * Gather all the information from the parameters of this function and
	 * create a NULL terminated string buffer with the error message.
	 */

	va_start( ap, fmt );
	vsnprintf_impl( buf, sizeof(buf), fmt, ap );
	va_end( ap );
	buf[sizeof(buf)-1] = 0;

	/*
	 * Let's first try to use the user's custom error handler callback. If
	 * that succeeds, there is no need for us to do our own error
	 * processing.
	 */

	if ( ctx->callbacks.log_message != NULL  &&  ctx->callbacks.log_message( ctx, conn, buf ) != 0 ) return;

	/*
	 * We now try to open the error log file. If this succeeds the error is
	 * appended to the file.
	 */

	if ( ctx->error_log_file != NULL && XX_httplib_fopen( ctx, conn, ctx->error_log_file, "a+", &fi ) ) {
		/*
		* We now have an open FILE stream pointer in fi.fp and can dump the
		* message in that file. Note though, that some information might not
		* be available for logging if the message has no connection, so some
		* information is skipped if the 'conn' parameter is NULL.
		*
		* Just to be sure that no other process is writing to the same file,
		* we use locking around this operation.
		*/
		flockfile( fi.fp );

		fprintf( fi.fp, "[%010lu]", (unsigned long)timestamp );

		if ( conn != NULL ) {
			XX_httplib_sockaddr_to_string( src_addr, sizeof(src_addr), &conn->client.rsa );
			fprintf( fi.fp, " [client %s]", src_addr );

			if ( conn->request_info.request_method != NULL && conn->request_info.request_uri != NULL ) {
				fprintf( fi.fp, " [request %s %s]", conn->request_info.request_method, conn->request_info.request_uri );
			}
		};

		fprintf( fi.fp, ": %s\n", buf );
		fflush( fi.fp );

		funlockfile( fi.fp );

		XX_httplib_fclose( &fi );
	} else {
		fprintf( stderr, "[%010lu]", (unsigned long)timestamp );

		if ( conn != NULL ) {
			XX_httplib_sockaddr_to_string( src_addr, sizeof(src_addr), &conn->client.rsa );
			fprintf( stderr, " [client %s]", src_addr );

			if ( conn->request_info.request_method != NULL && conn->request_info.request_uri != NULL ) {
				fprintf( stderr, " [request %s %s]", conn->request_info.request_method, conn->request_info.request_uri );
			}
		};

		fprintf( stderr, ": %s\n", buf );
	};

}  /* httplib_cry */
