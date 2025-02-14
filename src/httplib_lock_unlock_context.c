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
 * void httplib_lock_context( struct httplib_context *ctx );
 *
 * The function httplib_lock_context() puts a lock on the context.
 */

void httplib_lock_context( struct httplib_context *ctx ) {

	if ( ctx != NULL ) httplib_pthread_mutex_lock( & ctx->ctx_mutex );

}  /* httplib_lock_context */



/*
 * void httplib_unlock_context( struct httplib_context *ctx );
 *
 * The function httplib_unlock_context() removes the current lock from the context.
 */

void httplib_unlock_context( struct httplib_context *ctx ) {

	if ( ctx != NULL ) httplib_pthread_mutex_unlock( & ctx->ctx_mutex );

}  /* httplib_unlock_context */
