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
 * bool XX_httplib_parse_port_string( const struct vec *vec, struct socket *so, int *ip_version );
 *
 * Valid listening port specification is: [ip_address:]port[s]
 * Examples for IPv4: 80, 443s, 127.0.0.1:3128, 192.0.2.3:8080s
 * Examples for IPv6: [::]:80, [::1]:80,
 *   [2001:0db8:7654:3210:FEDC:BA98:7654:3210]:443s
 *   see https://tools.ietf.org/html/rfc3513#section-2.2
 * In order to bind to both, IPv4 and IPv6, you can either add
 * both ports using 8080,[::]:8080, or the short form +8080.
 * Both forms differ in detail: 8080,[::]:8080 create two sockets,
 * one only accepting IPv4 the other only IPv6. +8080 creates
 * one socket accepting IPv4 and IPv6. Depending on the IPv6
 * environment, they might work differently, or might not work
 * at all - it must be tested what options work best in the
 * relevant network environment.
 *
 * The function returns false if an error occurs and true otherwise.
 */

bool XX_httplib_parse_port_string( const struct vec *vec, struct socket *so, int *ip_version ) {

	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
	unsigned int port;
	int ch;
	int len;
	char buf[100] = {0};

	/*
	 * MacOS needs that. If we do not zero it, subsequent bind() will fail.
	 * Also, all-zeroes in the socket address means binding to all addresses
	 * for both IPv4 and IPv6 (INADDR_ANY and IN6ADDR_ANY_INIT).
	 */

	memset( so, 0, sizeof(*so) );

	so->lsa.sin.sin_family = AF_INET;
	*ip_version            = 0;

	if ( sscanf( vec->ptr, "%u.%u.%u.%u:%u%n", &a, &b, &c, &d, &port, &len ) == 5 ) {

		/*
		 * Bind to a specific IPv4 address, e.g. 192.168.1.5:8080
		 */

		so->lsa.sin.sin_addr.s_addr = htonl( (a << 24) | (b << 16) | (c << 8) | d );
		so->lsa.sin.sin_port        = htons( (uint16_t)port );

		*ip_version = 4;
	}

	else if ( sscanf( vec->ptr, "[%49[^]]]:%u%n", buf, &port, &len ) == 2  &&  XX_httplib_inet_pton( AF_INET6, buf, &so->lsa.sin6, sizeof(so->lsa.sin6) ) ) {

		/*
		 * IPv6 address, examples: see above
		 * so->lsa.sin6.sin6_family = AF_INET6; already set by httplib_inet_pton
		 */

		so->lsa.sin6.sin6_port = htons( (uint16_t)port );
		*ip_version = 6;
	}

	else if ( vec->ptr[0] == '+'  &&  sscanf( vec->ptr + 1, "%u%n", &port, &len ) == 1 ) {

		/*
		 * Port is specified with a +, bind to IPv6 and IPv4, INADDR_ANY
		 * Add 1 to len for the + character we skipped before
		 */

		len++;

		/*
		 * Set socket family to IPv6, do not use IPV6_V6ONLY
		 */

		so->lsa.sin6.sin6_family = AF_INET6;
		so->lsa.sin6.sin6_port   = htons(( uint16_t)port );

		*ip_version = 4 + 6;
	}

	else if ( sscanf( vec->ptr, "%u%n", &port, &len ) == 1 ) {

		/*
		 * If only port is specified, bind to IPv4, INADDR_ANY
		 */

		so->lsa.sin.sin_port = htons( (uint16_t)port );
		*ip_version = 4;

	}

	else {
		/*
		 * Parsing failure. Make port invalid.
		 */

		port = 0;
		len  = 0;
	}

	/*
	 * sscanf and the option splitting code ensure the following condition
	 */

	if ( len < 0 && ((unsigned)len) > ((unsigned)vec->len) ) {

		*ip_version = 0;
		return false;
	}

	ch            = vec->ptr[len]; /* Next character after the port number */
	so->has_ssl   = ( ch == 's' );
	so->has_redir = ( ch == 'r' );

	/*
	 * Make sure the port is valid and vector ends with 's', 'r' or ','
	 */

	if ( XX_httplib_is_valid_port( port )  &&  ( ch == '\0'  ||  ch == 's'  ||  ch == 'r'  ||  ch == ',' ) ) return true;

	/*
	 * Reset ip_version to 0 if there is an error
	 */

	*ip_version = 0;
	return false;

}  /* XX_httplib_parse_port_string */
