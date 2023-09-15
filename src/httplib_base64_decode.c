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

static unsigned char b64reverse(char letter);

/*
 * int httplib_base64_decode( const unsigned char *src, int src_len, char *dst, int dst_len );
 *
 * The function httplib_base64_decode() converts a BASE64 buffer of given
 * length to its binary equivalent. If an error occurs or the receive buffer is
 * too small, -1 is returned. Otherwise the return value is the length of the
 * data in the receive buffer, including the terminating NUL character.
 */

LIBHTTP_API ssize_t httplib_base64_decode( const unsigned char *src, size_t src_len, char *dst, size_t *dst_len ) {

	size_t i;
	unsigned char a, b, c, d;
	size_t dst_len_limit = (size_t) - 1;
	size_t dst_len_used = 0;

	if ( src == NULL  ||  src_len <= 0  ||  dst == NULL  ||  dst_len == NULL || *dst_len < 1 ) return -1;

	if ( *dst_len > 0 ) dst[0] = '\0';

	// 6 bits into 8 bits, mod 4 (+ '\0')
	size_t expected_len = (size_t)ceil(src_len * 6 / 8) + 4;
	if ( *dst_len < expected_len ) {
		*dst_len = expected_len;
		return -1;
	}

	dst_len_limit = *dst_len;
	*dst_len = 0;

	for (i = 0; i < src_len; i += 4) {
		/* Read 4 characters from BASE64 string */
		a = b64reverse(src[i]);
		if (a >= 254) {
			return (int)i;
		}

		b = b64reverse(((i + 1) >= src_len) ? 0 : src[i + 1]);
		if (b >= 254) {
			return (int)i + 1;
		}

		c = b64reverse(((i + 2) >= src_len) ? 0 : src[i + 2]);
		if (c == 254) {
			return (int)i + 2;
		}

		d = b64reverse(((i + 3) >= src_len) ? 0 : src[i + 3]);
		if (d == 254) {
			return (int)i + 3;
		}

		/* Add first (of 3) decoded character */
		if (dst_len_used < dst_len_limit) {
			dst[dst_len_used] = (unsigned char)((unsigned char)(a << 2)
			                                    + (unsigned char)(b >> 4));
		}
		dst_len_used++;

		if (c != 255) {
			if (dst_len_used < dst_len_limit) {

				dst[dst_len_used] = (unsigned char)((unsigned char)(b << 4)
				                                    + (unsigned char)(c >> 2));
			}
			dst_len_used++;
			if (d != 255) {
				if (dst_len_used < dst_len_limit) {
					dst[dst_len_used] =
					    (unsigned char)((unsigned char)(c << 6) + d);
				}
				dst_len_used++;
			}
		}
	}

	/* Add terminating zero */
	if (dst_len_used < dst_len_limit) {
		dst[dst_len_used] = '\0';
		dst_len_used++;
	}

	if (dst_len_used > dst_len_limit) {
		/* Out of memory */
		return -1;
	}

	*dst_len = dst_len_used;

	return (ssize_t)*dst_len;

}  /* httplib_base64_decode */


static unsigned char b64reverse(char letter)
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
