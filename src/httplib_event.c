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

#if defined(ALTERNATIVE_QUEUE)

#if defined(__linux__)


#include <sys/prctl.h>
#include <sys/sendfile.h>
#include <sys/eventfd.h>


void *XX_httplib_event_create( void ) {

	int eventhdl = eventfd( 0, EFD_CLOEXEC );

	if ( eventhdl == -1 ) {

		/*
		 * Linux uses -1 on error, Windows NULL.
		 * However, Linux does not return 0 on success either.
		 */

		return NULL;
	}

	return (void *)eventhdl;

}  /* XX_httplib_event_create */


int XX_httplib_event_wait( void *eventhdl ) {

	uint64_t u = 0;
	int s = (int)read( (int)eventhdl, &u, sizeof(u) );
	if ( s != sizeof(u) ) return 0;

	return 1;

}  /* XX_httplib_event_wait */


int XX_httplib_event_signal( void *eventhdl ) {

	uint64_t u = 1;
	int s = (int)write( (int)eventhdl, &u, sizeof(u) );
	if ( s != sizeof(u) ) return 0;

	return 1;

}  /* XX_httplib_event_signal */


void XX_httplib_event_destroy( void *eventhdl ) {

	close( (int)eventhdl );

}  /* XX_httplib_event_destroy */


#elif defined(_WIN32)


void *XX_httplib_event_create( void ) {

	return (void *) CreateEvent( NULL, FALSE, FALSE, NULL );

}  /* XX_httplib_event_create */


int XX_httplib_event_wait( void *eventhdl ) {

	return ( WaitForSingleObject( (HANDLE) eventhdl, INFINITE ) == WAIT_OBJECT_0 );

}  /* XX_httplib_event_wait */


int XX_httplib_event_signal( void *eventhdl ) {

	return (int) SetEvent( (HANDLE) eventhdl );

}  /* XX_httplib_event_signal */


void XX_httplib_event_destroy( void *eventhdl ) {

	CloseHandle( (HANDLE) eventhdl );

}  /* XX_httplib_event_destroy */


#else


struct posix_event {

	pthread_mutex_t mutex;
	pthread_cond_t cond;
};


void *XX_httplib_event_create(void) {

	struct posix_event *eventhdl = httplib_malloc( sizeof(struct posix_event) );
	if ( eventhdl == NULL ) return NULL;

	if ( httplib_pthread_mutex_init( & eventhdl->mutex, NULL ) != 0 ) {

		/*
		 * pthread mutex not available
		 */

		eventhdl = httplib_free( eventhdl );
		return NULL;
	}

	if ( httplib_pthread_cond_init( & eventhdl->cond, NULL ) != 0 ) {

		/*
		 * pthread cond not available
		 */

		httplib_pthread_mutex_destroy( & eventhdl->mutex );
		eventhdl = httplib_free( eventhdl );
		return NULL;
	}

	return eventhdl;

}  /* XX_httplib_event_create */


int XX_httplib_event_wait( void *eventhdl ) {

	if ( eventhdl == NULL ) return 1;

	struct posix_event *ev = eventhdl;

	httplib_pthread_mutex_lock(            & ev->mutex );
	httplib_pthread_cond_wait( & ev->cond, & ev->mutex );
	httplib_pthread_mutex_unlock(          & ev->mutex );

	return 1;

}  /* XX_httplib_event_wait */


int XX_httplib_event_signal( void *eventhdl ) {

	if ( eventhdl == NULL ) return 1;

	struct posix_event *ev = eventhdl;

	httplib_pthread_mutex_lock(   & ev->mutex );
	httplib_pthread_cond_signal(  & ev->cond  );
	httplib_pthread_mutex_unlock( & ev->mutex );

	return 1;

}  /* XX_httplib_event_signal */


void XX_httplib_event_destroy( void *eventhdl ) {

	if ( eventhdl == NULL ) return;

	struct posix_event *ev = eventhdl;

	httplib_pthread_cond_destroy(  & ev->cond  );
	httplib_pthread_mutex_destroy( & ev->mutex );

	ev = httplib_free( ev );

}  /* XX_httplib_event_destroy */


#endif

#endif  /* ALTERNATIVE_QUEUE */
