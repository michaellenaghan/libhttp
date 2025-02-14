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

/*
 * void XX_httplib_unitialize_ssl( struct httplib_context *ctx );
 *
 * The function XX_httplib_unititialize_ssl() is used to properly stop the SSL
 * subsystem.
 */

void XX_httplib_uninitialize_ssl( struct httplib_context *ctx ) {

	UNUSED_PARAMETER(ctx);

	if ( --XX_httplib_cryptolib_users == 0 ) {

		/* Shutdown according to
		 * https://wiki.openssl.org/index.php/Library_Initialization#Cleanup
		 * http://stackoverflow.com/questions/29845527/how-to-properly-uninitialize-openssl
		 */
		CONF_modules_unload(1);
	}

}  /* XX_httplib_unitialize_ssl */

#endif /* !NO_SSL */
