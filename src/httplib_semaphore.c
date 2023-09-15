/*
 * Copyright (c) 2023 Michael Lenaghan
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

#if defined(__APPLE__)


#include <dispatch/dispatch.h>


typedef struct sem {
	dispatch_semaphore_t sem;
} sem_t;


void *XX_httplib_semaphore_create( int const count ) {

	sem_t *semaphorehdl = httplib_calloc( 1, sizeof(sem_t) );
	if ( semaphorehdl == NULL ) return NULL;

	semaphorehdl->sem = dispatch_semaphore_create( count );

	return semaphorehdl;

}  /* XX_httplib_semaphore_create */


int XX_httplib_semaphore_wait( void *semaphorehdl ) {

	if ( semaphorehdl == NULL ) return 0;

	sem_t *_semaphorehdl = semaphorehdl;

	dispatch_semaphore_wait( _semaphorehdl->sem, DISPATCH_TIME_FOREVER );

	return 1;

}  /* XX_httplib_semaphore_wait */


int XX_httplib_semaphore_signal( void *semaphorehdl ) {

	if ( semaphorehdl == NULL ) return 0;

	sem_t *_semaphorehdl = semaphorehdl;

	dispatch_semaphore_signal( _semaphorehdl->sem );

	return 1;

}  /* XX_httplib_semaphore_signal */


void XX_httplib_semaphore_destroy( void *semaphorehdl ) {

	if ( semaphorehdl == NULL ) return;

	httplib_free( semaphorehdl );

}  /* XX_httplib_semaphore_destroy */


#elif defined(_WIN32)


void *XX_httplib_semaphore_create( int const count ) {

	return (void *) CreateSemaphore( NULL, count, count, NULL );
;

}  /* XX_httplib_semaphore_create */


int XX_httplib_semaphore_wait( void *semaphorehdl ) {

	return ( WaitForSingleObject( (HANDLE) semaphorehdl, INFINITE ) == WAIT_OBJECT_0 );

}  /* XX_httplib_semaphore_wait */


int XX_httplib_semaphore_signal( void *semaphorehdl ) {

	return (int) ReleaseSemaphore( (HANDLE) semaphorehdl, 1, NULL ) ;

}  /* XX_httplib_semaphore_signal */


void XX_httplib_semaphore_destroy( void *semaphorehdl ) {

	CloseHandle( (HANDLE) semaphorehdl );

}  /* XX_httplib_semaphore_destroy */


#else


#include <semaphore.h>


void *XX_httplib_semaphore_create( int const count ) {

	void *semaphorehdl = httplib_calloc( 1, sizeof(sem_t) );
	if ( semaphorehdl == NULL ) return NULL;

	sem_t *sem = semaphorehdl;

	if ( sem_init( sem, 0, count ) != 0 ) {

		/*
		 * semaphore not available
		 */

		semaphorehdl = httplib_free( semaphorehdl );
	}

	return semaphorehdl;

}  /* XX_httplib_semaphore_create */


int XX_httplib_semaphore_wait( void *semaphorehdl ) {

	if ( semaphorehdl == NULL ) return 0;

	sem_t *sem = semaphorehdl;

	int ret = sem_wait( sem );

	return ret == 0;

}  /* XX_httplib_semaphore_wait */


int XX_httplib_semaphore_signal( void *semaphorehdl ) {

	if ( semaphorehdl == NULL ) return 0;

	sem_t *sem = semaphorehdl;

	int ret = sem_post( sem );

	return ret == 0;

}  /* XX_httplib_semaphore_signal */


void XX_httplib_semaphore_destroy( void *semaphorehdl ) {

	if ( semaphorehdl == NULL ) return;

	sem_t *sem = semaphorehdl;

	sem_destroy( sem );

	httplib_free( semaphorehdl );

}  /* XX_httplib_semaphore_destroy */


#endif
