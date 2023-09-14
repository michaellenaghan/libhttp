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
#include "httplib_pthread.h"

/*
 * int XX_httplib_consume_socket( struct httplib_context *ctx, struct socket *sp, int thread_index );
 *
 * The function XX_httplib_consume_socket() takes an accepted socket from the
 * queue for further processing.
 */

#if defined(ALTERNATIVE_QUEUE)

int XX_httplib_consume_socket( struct httplib_context *ctx, struct socket *sp, int thread_index ) {

	assert(!ctx->client_socks[thread_index].in_use);
	XX_httplib_event_wait( ctx->client_wait_events[thread_index] );
	assert(ctx->client_socks[thread_index].in_use);

	*sp = ctx->client_socks[thread_index];
	assert(*sp->in_use);

	return ( ctx->status == CTX_STATUS_RUNNING );

}  /* XX_httplib_consume_socket */

#else /* ALTERNATIVE_QUEUE */

/* Worker threads take accepted socket from the queue */
int XX_httplib_consume_socket( struct httplib_context *ctx, struct socket *sp, int thread_index ) {

	UNUSED_PARAMETER(thread_index);

	httplib_pthread_mutex_lock( & ctx->thread_mutex );

	/*
	 * If the queue is empty, wait. We're idle at this point.
	 */

	while ( ctx->sq_head == ctx->sq_tail  &&  ctx->status == CTX_STATUS_RUNNING ) httplib_pthread_cond_wait( & ctx->sq_full, & ctx->thread_mutex );

	/*
	 * If we're stopping, sq_head may be equal to sq_tail.
	 */

	if ( ctx->sq_head > ctx->sq_tail ) {
		int const queue_size = (int)(ARRAY_SIZE(ctx->queue));

		/*
		 * Copy socket from the queue and increment tail
		 */

		*sp = ctx->queue[ctx->sq_tail % queue_size];
		ctx->sq_tail++;

		/*
		 * Wrap pointers if needed
		 */

		while ( ctx->sq_tail > queue_size ) {

			ctx->sq_tail -= queue_size;
			ctx->sq_head -= queue_size;
		}
	}

	httplib_pthread_cond_signal(  & ctx->sq_empty     );
	httplib_pthread_mutex_unlock( & ctx->thread_mutex );

	return ( ctx->status == CTX_STATUS_RUNNING );

}  /* XX_httplib_consume_socket */

#endif /* ALTERNATIVE_QUEUE */
