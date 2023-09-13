/*
 * Copyright (C) 2016 Lammert Bies
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
 */



#if !defined(NO_SSL)

int				XX_httplib_get_first_ssl_listener_index( const struct httplib_context *ctx );
int				XX_httplib_initialize_ssl( struct httplib_context *ctx );
bool				XX_httplib_set_ssl_option( struct httplib_context *ctx );
const char *			XX_httplib_ssl_error( void );
void				XX_httplib_ssl_get_client_cert_info( struct httplib_connection *conn );
unsigned long			XX_httplib_ssl_get_protocol( int version_id );
int				XX_httplib_ssl_use_pem_file( struct httplib_context *ctx, const char *pem );
int				XX_httplib_sslize( struct httplib_context *ctx, struct httplib_connection *conn, SSL_CTX *s, int (*func)(SSL *) );
void				XX_httplib_tls_dtor( void *key );
void				XX_httplib_uninitialize_ssl( struct httplib_context *ctx );

extern int			XX_httplib_cryptolib_users;

#endif  /* NO_SSL */
