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
 * LIBHTTP_THREAD XX_httplib_websocket_client_thread( void *data );
 *
 * The function XX_httplib_websocket_client_thread() is the worker thread which
 * connects as a client to a remote websocket server. When finished, the
 * function frees the memory associated with the thread and the connection, but
 * it does not free any context data, because the context can be reused for
 * another connection.
 *
 * To signal the calling application that the context is in use or can be
 * reused, a status flag is set to the appropriate value.
 */

LIBHTTP_THREAD XX_httplib_websocket_client_thread( void *data ) {

	struct httplib_context *ctx;
	struct httplib_connection *conn;

	struct websocket_client_thread_data *cdata;

	if ( (cdata = data       ) == NULL ) return LIBHTTP_THREAD_RETNULL;
	if ( (conn  = cdata->conn) == NULL ) return LIBHTTP_THREAD_RETNULL;
	if ( (ctx   = cdata->ctx ) == NULL ) return LIBHTTP_THREAD_RETNULL;

	ctx->status = CTX_STATUS_RUNNING;

	XX_httplib_set_thread_name( ctx, "ws-client" );

	if ( ctx->callbacks.init_thread != NULL ) {
		void *user_data = ctx->callbacks.init_thread( ctx, THREAD_WEBSOCKET );
		httplib_pthread_setspecific( XX_httplib_tls_key, user_data );
	}

	XX_httplib_read_websocket( ctx, conn, cdata->data_handler, cdata->callback_data );

	if ( cdata->close_handler != NULL ) cdata->close_handler( ctx, conn, cdata->callback_data );

	if ( ctx->callbacks.exit_thread != NULL ) {
		void *user_data = httplib_pthread_getspecific( XX_httplib_tls_key );
		ctx->callbacks.exit_thread( ctx, THREAD_WEBSOCKET, user_data );
		httplib_pthread_setspecific( XX_httplib_tls_key, NULL );
	}

	ctx->workerthreadids = httplib_free( ctx->workerthreadids );
	conn                 = httplib_free( conn                 );
	cdata                = httplib_free( cdata                );
	ctx->user_data       = NULL;
	ctx->num_threads     = 0;
	ctx->status          = CTX_STATUS_TERMINATED;

	return LIBHTTP_THREAD_RETNULL;

}  /* XX_httplib_websocket_client_thread */
