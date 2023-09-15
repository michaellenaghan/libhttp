/*
 * Copyright (c) 2013-2023 the Civetweb developers
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

static unsigned char b64reverse( char letter );
static size_t round_up_to_nearest_4( size_t len );


/*
 * ssize_t httplib_base64_decode( const unsigned char *restrict src, size_t src_len, char *restrict dst, size_t dst_len );
 *
 * The function httplib_base64_decode() converts a BASE64 buffer of given
 * length to its binary equivalent. If an error occurs or the receive buffer is
 * too small, -1 is returned. Otherwise the return value is the length of the
 * data in the receive buffer, including the terminating NUL character.
 */

LIBHTTP_API ssize_t httplib_base64_decode( const char *restrict src, size_t src_len, unsigned char *restrict dst, size_t *dst_len ) {

	size_t i;
	size_t j;
	unsigned char a;
	unsigned char b;
	unsigned char c;
	unsigned char d;

	if ( dst_len == NULL ) return -1;

	// 6 bits into 8 bits, mod 4 (+ '\0')
	size_t expected_len = round_up_to_nearest_4(src_len) * 6 / 8;

	if ( src == NULL || dst == NULL || *dst_len < expected_len ) {
		if ( dst ) *dst = '\0';
		*dst_len = expected_len;
		return -1;
	}

	j = 0;

	for ( i = 0; i < src_len; i += 4 ) {
		/* Read 4 characters from BASE64 string */
		a = b64reverse(                      src[i + 0]     );
		b = b64reverse(((i + 1) < src_len) ? src[i + 1] : 0 );
		c = b64reverse(((i + 2) < src_len) ? src[i + 2] : 0 );
		d = b64reverse(((i + 3) < src_len) ? src[i + 3] : 0 );

		if (a >= 254 || b >= 254 || c == 254 || d == 254) {
			*dst = '\0';
			*dst_len = expected_len;
			return -1;
		}

		/* Add first (of 3) decoded character */
		dst[j++] = (unsigned char)((unsigned char)(a << 2) + (unsigned char)(b >> 4));

		if (c != 255) {
			dst[j++] = (unsigned char)((unsigned char)(b << 4) + (unsigned char)(c >> 2));
			if (d != 255) {
				dst[j++] = (unsigned char)((unsigned char)(c << 6) + d);
			}
		}
	}

	dst[j] = '\0';

	*dst_len = j;

	assert(*dst_len <= expected_len);

	return (ssize_t)*dst_len;

}  /* httplib_base64_decode */


static unsigned char b64reverse( char letter )
{
	if ((letter >= 'A') && (letter <= 'Z')) {
		return (unsigned char)(letter - 'A');
	}
	if ((letter >= 'a') && (letter <= 'z')) {
		return (unsigned char)(letter - 'a' + 26);
	}
	if ((letter >= '0') && (letter <= '9')) {
		return (unsigned char)(letter - '0' + 52);
	}
	if (letter == '+') {
		return 62;
	}
	if (letter == '/') {
		return 63;
	}
	if (letter == '=') {
		return 255; /* normal end */
	}
	return 254; /* error */
}


static size_t round_up_to_nearest_4( size_t len ) {
	return (len + 3) & ~0x03ul;
}
