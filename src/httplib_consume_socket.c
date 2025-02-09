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
 * int XX_httplib_consume_socket( struct httplib_context *ctx, struct socket *sp, int thread_index );
 *
 * The function XX_httplib_consume_socket() takes an accepted socket from the
 * queue for further processing.
 */

int XX_httplib_consume_socket( struct httplib_context *ctx, struct socket *sp, int thread_index ) {

	assert(!ctx->client_socks[thread_index].in_use);
	XX_httplib_event_wait( ctx->client_wait_events[thread_index] );
	assert(ctx->client_socks[thread_index].in_use || ctx->status != CTX_STATUS_RUNNING );

	*sp = ctx->client_socks[thread_index];

	return ( ctx->status == CTX_STATUS_RUNNING );

}  /* XX_httplib_consume_socket */
