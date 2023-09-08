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

/*
 * void XX_httplib_interpret_uri();
 *
 * The function XX_httplib_interpret_uri() interprets an URI and decides what
 * type of request is involved. The function takes the following parameters:
 *
 * ctx:				in:  The context in which to communicate
 * conn:			in:  The request (must be valid)
 * filename:			out: Filename
 * filename_buf_len:		in:  Size of the filename buffer
 * filep:			out: file structure
 * is_found:			out: file is found (directly)
 * is_script_resource:		out: handled by a script?
 * is_websocket_request:	out: websocket connection?
 * is_put_or_delete_request:	out: put/delete file?
 */

void XX_httplib_interpret_uri( struct httplib_context *ctx, struct httplib_connection *conn, char *filename, size_t filename_buf_len, struct file *filep, bool *is_found, bool *is_script_resource, bool *is_websocket_request, bool *is_put_or_delete_request ) {

/* TODO (high): Restructure this function */

	const char *uri;
	const char *root;
	const char *rewrite;
	struct vec a;
	struct vec b;
	int match_len;
	char gz_path[PATH_MAX];
	char const *accept_encoding;
	bool truncated;

	if ( ctx == NULL  ||  conn == NULL  ||  filep == NULL ) return;

	uri  = conn->request_info.local_uri;
	root = ctx->document_root;

	memset( filep, 0, sizeof(*filep) );

	*filename                 = '\0';
	*is_found                 = false;
	*is_script_resource       = false;
	*is_put_or_delete_request = XX_httplib_is_put_or_delete_method( conn );

	*is_websocket_request     = XX_httplib_is_websocket_protocol( conn );

	if ( *is_websocket_request  &&  ctx->websocket_root != NULL ) root = ctx->websocket_root;

	/*
	 * Note that root == NULL is a regular use case here. This occurs,
	 * if all requests are handled by callbacks, so the WEBSOCKET_ROOT
	 * config is not required.
	 */

	if ( root == NULL ) {

		/*
		 * all file related outputs have already been set to 0, just return
		 */

		return;
	}

	/*
	 * If document_root is NULL, leave the file empty.
	 */

	XX_httplib_snprintf( ctx, conn, &truncated, filename, filename_buf_len, "%s%s", root, uri );

	if ( truncated ) goto interpret_cleanup;

	rewrite = ctx->url_rewrite_patterns;

	while ( (rewrite = XX_httplib_next_option( rewrite, &a, &b )) != NULL ) {

		match_len = XX_httplib_match_prefix( a.ptr, a.len, uri );

		if ( match_len > 0 ) {

			XX_httplib_snprintf( ctx, conn, &truncated, filename, filename_buf_len, "%.*s%s", (int)b.len, b.ptr, uri + match_len );
			break;
		}
	}

	if ( truncated ) goto interpret_cleanup;

	/*
	 * Local file path and name, corresponding to requested URI
	 * is now stored in "filename" variable.
	 */

	if ( XX_httplib_stat( ctx, conn, filename, filep ) ) {
		*is_found = true;
		return;
	}

	/*
	 * If we can't find the actual file, look for the file
	 * with the same name but a .gz extension. If we find it,
	 * use that and set the gzipped flag in the file struct
	 * to indicate that the response need to have the content-
	 * encoding: gzip header.
	 * We can only do this if the browser declares support.
	 */

	if ( (accept_encoding = httplib_get_header( conn, "Accept-Encoding" )) != NULL ) {

		if ( strstr( accept_encoding, "gzip" ) != NULL ) {

			XX_httplib_snprintf( ctx, conn, &truncated, gz_path, sizeof(gz_path), "%s.gz", filename );

			if ( truncated ) goto interpret_cleanup;

			if ( XX_httplib_stat( ctx, conn, gz_path, filep ) ) {

				if ( filep ) {

					filep->gzipped = 1;
					*is_found      = true;
				}

				/*
				 * Currently gz files can not be scripts.
				 */

				return;
			}
		}
	}

	return;

/*
 * Reset all outputs
 */

interpret_cleanup:

	memset( filep, 0, sizeof(*filep) );

	*filename                 = 0;
	*is_found                 = false;
	*is_script_resource       = false;
	*is_websocket_request     = false;
	*is_put_or_delete_request = false;

}  /* XX_httplib_interpret_uri */
