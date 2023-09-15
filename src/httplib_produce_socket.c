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
 * void XX_httplib_produce_socket( struct httplib_context *ctx, const struct socket *sp );
 *
 * The function XX_httplib_produce_socket() is used to produce a socket.
 */

#if defined(ALTERNATIVE_QUEUE)

void XX_httplib_produce_socket( struct httplib_context *ctx, const struct socket *sp ) {

	XX_httplib_semaphore_wait( ctx->client_wait_semaphore );

	for ( int i=0; i < ctx->num_threads; i++ ) {

		/*
		* find a free worker slot and signal it
		*/

		if ( ctx->client_socks[i].in_use == 0 ) {

			// There's subtley around setting `in_use`.
			//
			// It's tempting to let the worker thread set it to `1`, but
			// that introduces a race: what if the master thread accepts
			// another socket and goes looking for free worker threads
			// *before* the selected worker thread wakes up? The master
			// could end up overwriting the socket it just assigned.

			assert(!sp->in_use);
			ctx->client_socks[i]        = *sp;
			ctx->client_socks[i].in_use = 1;

			XX_httplib_event_signal( ctx->client_wait_events[i] );

			return;
		}

	}

	httplib_cry( LH_DEBUG_ERROR, ctx, NULL, "%s: cannot find a free worker slot", __func__ );

}  /* XX_httplib_produce_socket */

#else /* ALTERNATIVE_QUEUE */

/*
 * Master thread adds accepted socket to a queue
 */

void XX_httplib_produce_socket( struct httplib_context *ctx, const struct socket *sp ) {

	if ( ctx == NULL ) return;

	httplib_pthread_mutex_lock( & ctx->thread_mutex );

	/*
	 * If the queue is full, wait
	 */

	int const queue_size = (int)(ARRAY_SIZE(ctx->queue));

	while ( ctx->status == CTX_STATUS_RUNNING  &&  ctx->sq_head-ctx->sq_tail >= queue_size ) {

		httplib_pthread_cond_wait( & ctx->sq_empty, & ctx->thread_mutex );
	}

	if ( ctx->sq_head - ctx->sq_tail < queue_size ) {

		/*
		 * Copy socket to the queue and increment head
		 */

		ctx->queue[ctx->sq_head % queue_size] = *sp;
		ctx->sq_head++;
	}

	httplib_pthread_cond_signal(  & ctx->sq_full      );
	httplib_pthread_mutex_unlock( & ctx->thread_mutex );

}  /* XX_httplib_produce_socket */

#endif /* ALTERNATIVE_QUEUE */
