/*
 * Copyright (c) 2016-2019 Lammert Bies
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

#if !defined(NO_SSL)

#include "httplib_main.h"
#include "httplib_ssl.h"
#include "httplib_utils.h"

#if defined(SSL_ALREADY_INITIALIZED)
int XX_httplib_cryptolib_users = 1; /* Reference counter for crypto library. */
#else
int XX_httplib_cryptolib_users = 0; /* Reference counter for crypto library. */
#endif

/*
 * int XX_httplib_initialize_ssl( struct httplib_context *ctx );
 *
 * The function XX_httplib_initialize_ssl() initializes the use of SSL
 * encrypted communication on the given context.
 */

int XX_httplib_initialize_ssl( struct httplib_context *ctx ) {

	UNUSED_PARAMETER(ctx);

	if ( httplib_atomic_inc( & XX_httplib_cryptolib_users ) > 1 ) return 1;

	OPENSSL_init_ssl( 0, NULL );
	OPENSSL_init_ssl( OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL );

	return 1;

}  /* XX_httplib_initialize_ssl */

#endif /* !NO_SSL */
