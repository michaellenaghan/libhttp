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

static bool	hexdump2string( void *mem, int memlen, char *buf, int buflen );

/*
 * void XX_httplib_ssl_get_client_cert_info( struct httplib_connection *conn );
 *
 * The function XX_httplib_ssl_get_client_cert_info() returns information from
 * a client provided certificate and hooks it up to the connection info.
 */

void XX_httplib_ssl_get_client_cert_info( struct httplib_connection *conn ) {

	X509 *cert = SSL_get_peer_certificate(conn->ssl);
	if (cert) {
		unsigned char buf[256];

		/* Handle to algorithm used for fingerprint */
		const EVP_MD *digest = EVP_get_digestbyname("sha1");

		/* Get Subject and issuer */
		X509_NAME *subj = X509_get_subject_name(cert);
		X509_NAME *iss = X509_get_issuer_name(cert);

		/* Get serial number */
		ASN1_INTEGER *serial = X509_get_serialNumber(cert);

		/* Translate serial number to a hex string */
		BIGNUM *serial_bn = ASN1_INTEGER_to_BN(serial, NULL);
		char *serial_str = NULL;
		if (serial_bn) {
			serial_str = BN_bn2hex(serial_bn);
			conn->request_info.client_cert->serial = serial_str ? httplib_strdup(serial_str) : NULL;
			OPENSSL_free(serial_str);

			BN_free(serial_bn);
		}

		/* Translate subject and issuer to a string */
		char str_buf[1024];
		(void)X509_NAME_oneline(subj, str_buf, (int)sizeof(str_buf));
		conn->request_info.client_cert->subject = httplib_strdup(str_buf);
		(void)X509_NAME_oneline(iss, str_buf, (int)sizeof(str_buf));
		conn->request_info.client_cert->issuer = httplib_strdup(str_buf);

		/* Calculate SHA1 fingerprint and store as a hex string */

		unsigned int ulen = 0;

		/* (ASN1_digest is deprecated. Do the calculation manually, using EVP_Digest.) */
		int ilen = i2d_X509(cert, NULL);
		unsigned char *tmp_buf = (ilen > 0)
		              ? (unsigned char *)httplib_malloc((unsigned)ilen + 1)
		              : NULL;
		if (tmp_buf) {
			unsigned char *tmp_p = tmp_buf;
			(void)i2d_X509(cert, &tmp_p);
			if (!EVP_Digest(
			        tmp_buf, (unsigned)ilen, buf, &ulen, digest, NULL)) {
				ulen = 0;
			}
			httplib_free(tmp_buf);
		}

		if (!hexdump2string(buf, (int)ulen, str_buf, (int)sizeof(str_buf))) {
			*str_buf = 0;
		}
		conn->request_info.client_cert->finger = httplib_strdup(str_buf);

		conn->request_info.client_cert->cert = (void *)cert;
	}

}  /* XX_httplib_ssl_get_client_cert_info */



/*
 * static int hexdump2string( void *mem, int memlen, char *buf, int buflen );
 *
 * The function hexdump2string() takes a binary blob and converts it to a hex
 * encoded string.
 */

static bool hexdump2string( void *mem, int memlen, char *buf, int buflen ) {

	int i;
	const char hexdigit[] = "0123456789abcdef";

	if ( memlen <= 0  ||  buflen <= 0 ) return false;
	if ( buflen < (3 * memlen)        ) return false;

	for (i=0; i<memlen; i++) {

		if (i > 0) buf[3*i-1] = ' ';

		buf[3*i  ] = hexdigit[(((uint8_t *)mem)[i] >> 4) & 0xF];
		buf[3*i+1] = hexdigit[ ((uint8_t *)mem)[i]       & 0xF];
	}
	buf[3*memlen-1] = 0;

	return true;

}  /* hexdump2string */

#endif /* !NO_SSL */
