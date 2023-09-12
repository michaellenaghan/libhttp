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
#include "httplib_utils.h"

/*
 * int XX_httplib_set_ports_option( struct httplib_context *ctx );
 *
 * The function XX_httplib_set_ports_option() set the port options for a
 * context. The function returns the total number of ports opened, or 0 if no
 * ports have been opened.
 */

int XX_httplib_set_ports_option( struct httplib_context *ctx ) {

	const char *list;
	char error_string[ERROR_STRING_LEN];
	int on;
	int off;
	struct vec vec;
	struct socket so;
	struct socket *ptr;
	struct pollfd *pfd;
	union usa usa;
	socklen_t len;
	int ip_version;
	int ports_total;
	int ports_ok;

	if ( ctx == NULL ) return 0;

	on          = 1;
	off         = 0;
	ports_total = 0;
	ports_ok    = 0;

	memset( & so,  0, sizeof(so)  );
	memset( & usa, 0, sizeof(usa) );

	len  = sizeof(usa);
	list = ctx->listening_ports;

	while ( (list = XX_httplib_next_option( list, &vec, NULL )) != NULL ) {

		ports_total++;

		if ( ! XX_httplib_parse_port_string( &vec, &so, &ip_version ) ) {

			httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: %.*s: invalid port spec (entry %i). Expecting list of: %s", __func__, (int)vec.len, vec.ptr, ports_total, "[IP_ADDRESS:]PORT[s|r]" );
			continue;
		}

#if !defined(NO_SSL)
		if ( so.has_ssl  &&  ctx->ssl_ctx == NULL ) {

			httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: cannot add SSL socket (entry %i). Is -ssl_certificate option set?", __func__, ports_total );
			continue;
		}
#endif  /* ! NO_SLL */

		if ( ( so.sock = socket( so.lsa.sa.sa_family, SOCK_STREAM, 6 ) ) == INVALID_SOCKET ) {

			httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: cannot create socket (entry %i)", __func__, ports_total );
			continue;
		}

#if defined(_WIN32)

		/*
		 * Windows SO_REUSEADDR lets many procs binds to a
		 * socket, SO_EXCLUSIVEADDRUSE makes the bind fail
		 * if someone already has the socket -- DTL
		 *
		 * NOTE: If SO_EXCLUSIVEADDRUSE is used,
		 * Windows might need a few seconds before
		 * the same port can be used again in the
		 * same process, so a short Sleep may be
		 * required between httplib_stop and httplib_start.
		 */

		if ( setsockopt( so.sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (SOCK_OPT_TYPE)&on, sizeof(on) ) != 0 ) {

			/*
			 * Set reuse option, but don't abort on errors.
			 */

			httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: cannot set socket option SO_EXCLUSIVEADDRUSE (entry %i)", __func__, ports_total );
		}
#else  /* _WIN32 */
		if ( setsockopt( so.sock, SOL_SOCKET, SO_REUSEADDR, (SOCK_OPT_TYPE)&on, sizeof(on) ) != 0 ) {

			/*
			 * Set reuse option, but don't abort on errors.
			 */

			httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: cannot set socket option SO_REUSEADDR (entry %i)", __func__, ports_total );
		}
#endif  /* _WIN32 */

		if ( ip_version > 4 ) {

			if ( ip_version == 6 ) {

				if ( so.lsa.sa.sa_family == AF_INET6  &&  setsockopt( so.sock, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&off, sizeof(off) ) != 0 ) {

					/*
					 * Set IPv6 only option, but don't abort on errors.
					 */

					httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: cannot set socket option IPV6_V6ONLY (entry %i)", __func__, ports_total );
				}
			}
		}

		if ( so.lsa.sa.sa_family == AF_INET ) {

			len = sizeof(so.lsa.sin);

			if ( bind( so.sock, &so.lsa.sa, len ) != 0 ) {

				httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: cannot bind to %.*s: %d (%s)", __func__, (int)vec.len, vec.ptr, (int)ERRNO, httplib_error_string( ERRNO, error_string, ERROR_STRING_LEN ) );
				closesocket( so.sock );
				so.sock = INVALID_SOCKET;
				continue;
			}
		}

		else if ( so.lsa.sa.sa_family == AF_INET6 ) {

			len = sizeof(so.lsa.sin6);

			if ( bind( so.sock, &so.lsa.sa, len ) != 0 ) {

				httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: cannot bind to IPv6 %.*s: %d (%s)", __func__, (int)vec.len, vec.ptr, (int)ERRNO, httplib_error_string( ERRNO, error_string, ERROR_STRING_LEN ) );
				closesocket( so.sock );
				so.sock = INVALID_SOCKET;
				continue;
			}
		}

		else {
			httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: cannot bind: address family not supported (entry %i)", __func__, ports_total );
			continue;
		}

		if ( listen( so.sock, SOMAXCONN ) != 0 ) {

			httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: cannot listen to %.*s: %d (%s)", __func__, (int)vec.len, vec.ptr, (int)ERRNO, httplib_error_string( ERRNO, error_string, ERROR_STRING_LEN ) );
			closesocket( so.sock );
			so.sock = INVALID_SOCKET;
			continue;
		}

		if ( getsockname( so.sock, &(usa.sa), &len ) != 0  ||  usa.sa.sa_family  !=  so.lsa.sa.sa_family ) {

			int err = (int)ERRNO;
			httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: call to getsockname failed %.*s: %d (%s)", __func__, (int)vec.len, vec.ptr, err, httplib_error_string( ERRNO, error_string, ERROR_STRING_LEN ) );
			closesocket( so.sock );
			so.sock = INVALID_SOCKET;
			continue;
		}

/*
 * Update lsa port in case of random free ports
 */

		if ( so.lsa.sa.sa_family == AF_INET6 ) so.lsa.sin6.sin6_port = usa.sin6.sin6_port;
		else                                   so.lsa.sin.sin_port   = usa.sin.sin_port;

		ptr = httplib_realloc( ctx->listening_sockets, (ctx->num_listening_sockets+1) * sizeof(ctx->listening_sockets[0]) );

		if ( ptr != NULL ) ctx->listening_sockets = ptr;
		else {
			httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: out of memory on listening sockets", __func__ );
			closesocket( so.sock );
			so.sock = INVALID_SOCKET;
			continue;
		}


		pfd = httplib_realloc( ctx->listening_socket_fds, (ctx->num_listening_sockets+1) * sizeof(ctx->listening_socket_fds[0]) );

		if ( pfd != NULL ) ctx->listening_socket_fds = pfd;
		else {
			httplib_cry( LH_DEBUG_CRASH, ctx, NULL, "%s: out of memory on fds", __func__ );
			closesocket( so.sock );
			so.sock = INVALID_SOCKET;
			continue;
		}


		XX_httplib_set_close_on_exec( so.sock );

		ctx->listening_sockets[ctx->num_listening_sockets] = so;
		ctx->num_listening_sockets++;

		ports_ok++;
	}

	if ( ports_ok != ports_total ) {

		XX_httplib_close_all_listening_sockets( ctx );
		ports_ok = 0;
	}

	return ports_ok;

}  /* XX_httplib_set_ports_option */
