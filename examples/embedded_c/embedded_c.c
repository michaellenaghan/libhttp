/*
* Copyright (c) 2013-2016 the CivetWeb developers
* Copyright (c) 2013 No Face Press, LLC
* License http://opensource.org/licenses/mit-license.php MIT License
*/

/* Simple example program on how to use libhttp embedded into a C program. */
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "libhttp.h"

#include "../../src/extern_md5.c"


#define MAX_BUFFER 4096  // Unsafe use of fixed-sized buffers!


static int exitNow = 0;


#ifdef _WIN32
#define sleep(x)  Sleep((x) * 1000)
#define snprintf(...)  _snprintf(__VA_ARGS__)
#endif


static int
ExampleHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	char body[MAX_BUFFER];
	snprintf(
		body, sizeof(body),
		"<!DOCTYPE html>"
		"<html><body>"
		"<h2>This is the Example handler</h2>"
		"<p>To see a page from the A handler click <a href=\"A\">here</a> (url: \"A\")</p>"
		"<p>To see a page from the A handler click <a href=\"A/A\">here</a> (url: \"A/A\")</p>"
		"<p>To see a page from the A/B handler click <a href=\"A/B\">here</a> (url: \"A/B\")</p>"
		"<p>To see a page from the B handler (callback data 0) click <a href=\"B\">here</a> (url: \"B\")</p>"
		"<p>To see a page from the B handler (callback data 1) click <a href=\"B/A\">here</a> (url: \"B/A\")</p>"
		"<p>To see a page from the B handler (callback data 2) click <a href=\"B/B\">here</a> (url: \"B/B\")</p>"
		"<p>To see a page from the *.foo handler click <a href=\"xy.foo\">here</a> (url: \"xy.foo\")</p>"
		"<p>To see a page from the close handler click <a href=\"close\">here</a> (url: \"close\")</p>"
		"<p>To see a page from the File handler (which leads to the Form handler) click <a href=\"form\">here</a> (url: \"form\")</p>"
		"<p>To see a page from the Cookie handler click <a href=\"cookie\">here</a> (url: \"cookie\")</p>"
		"<p>To see a page from the Checksum handler (an example for parsing files on the fly) click <a href=\"on_the_fly_form\">here</a> (url: \"on_the_fly_form\")</p>"
		"<p>To test websocket handler click <a href=\"/websocket\">here</a> (url: \"websocket\")</p>"
		"<p>To exit click <a href=\"/exit\">here</a> (url: \"exit\")</p>"
		"</body></html>");

	httplib_printf(ctx, conn,
	        "HTTP/1.1 200 OK\r\n"
		"Content-Length: %zd\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"%s",
		strlen(body),
		body);

	return 200;
}


static int
ExitHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	char body[MAX_BUFFER];
	snprintf(
		body, sizeof(body),
		"<!DOCTYPE html>"
		"<html><body>"
		"<p>Server will shut down.</p>"
		"<p>Bye!</p>"
		"</body></html>");

	httplib_printf(ctx, conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %zd\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"%s",
		strlen(body),
		body);

	exitNow = 1;

	return 200;
}


static int
AHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	char body[MAX_BUFFER];
	snprintf(
		body, sizeof(body),
		"<!DOCTYPE html>"
		"<html><body>"
		"<h2>This is the A handler</h2>"
		"</body></html>");

	httplib_printf(ctx, conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %zd\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"%s",
		strlen(body),
		body);

	return 200;
}


static int
ABHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	char body[MAX_BUFFER];
	snprintf(
		body, sizeof(body),
		"<!DOCTYPE html>"
		"<html><body>"
		"<h2>This is the AB handler</h2>"
		"</body></html>");

	httplib_printf(ctx, conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %zd\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"%s",
		strlen(body),
		body);

	return 200;
}


static int
BXHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	/* Handler may access the request info using httplib_get_request_info */
	const struct httplib_request_info *req_info = httplib_get_request_info(conn);

	char body[MAX_BUFFER];
	snprintf(
		body, sizeof(body),
		"<!DOCTYPE html>"
		"<html><body>"
		"<h2>This is the BX handler (callback data %ld)</h2>"
		"<p>The actual uri is %s</p>"
		"</body></html>",
		(intptr_t)cbdata,
		req_info->uri);

	httplib_printf(ctx, conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %zd\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"%s",
		strlen(body),
		body);

	return 200;
}


static int
FooHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	/* Handler may access the request info using httplib_get_request_info */
	const struct httplib_request_info *req_info = httplib_get_request_info(conn);

	char body[MAX_BUFFER];
	snprintf(
		body, sizeof(body),
		"<!DOCTYPE html>"
		"<html><body>"
		"<h2>This is the Foo handler</h2>"
	        "<p>The request was: <pre>%s %s HTTP/%s</pre></p>"
		"</body></html>",
		req_info->request_method,
		req_info->uri,
		req_info->http_version);

	httplib_printf(ctx, conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %zd\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"%s",
		strlen(body),
		body);

	return 200;
}


static int
CloseHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	char body[MAX_BUFFER];
	snprintf(
		body, sizeof(body),
		"<!DOCTYPE html>"
		"<html><body>"
		"<h2>This is the Close handler</h2>"
		"<p>This handler will close the connection in a second...</p>"
		"</body></html>");

	httplib_printf(ctx, conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %zd\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"%s",
		strlen(body),
		body);

	printf("CloseHandler: wait 1 sec\n");
	sleep(1);

	printf("CloseHandler: close\n");
	httplib_close_connection(ctx, conn);

	printf("CloseHandler: wait 10 sec\n");
	sleep(10);

	printf("CloseHandler: return\n");

	return 200;
}


static int
FileHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	/* In this handler, we ignore the req_info and send the file "fileName". */
	const char *fileName = (const char *)cbdata;

	httplib_send_file(ctx, conn, fileName, NULL, NULL);

	return 200;
}


struct ctx_conn_t {
	struct httplib_context *ctx;
	struct httplib_connection *conn;
	char buffer[MAX_BUFFER];
};


static int
field_found(const char *key,
            const char *filename,
            char *path,
            size_t pathlen,
            void *user_data)
{
	struct ctx_conn_t *ctx_conn = user_data;

	strcat(ctx_conn->buffer, "<p>name: ");
	strcat(ctx_conn->buffer, key);
	strcat(ctx_conn->buffer, "</p>");

	if (filename && *filename) {
		snprintf(path, pathlen, "/tmp/%s", filename);
		return FORM_FIELD_STORAGE_STORE;
	}

	return FORM_FIELD_STORAGE_GET;
}


static int
field_get(const char *key, const char *value, size_t valuelen, void *user_data)
{
	struct ctx_conn_t *ctx_conn = user_data;

	strcat(ctx_conn->buffer, "<p>value: ");
	strncat(ctx_conn->buffer, value, valuelen);
	strcat(ctx_conn->buffer, "</p>");

	return 0;
}


static int
field_stored(const char *path, int64_t file_size, void *user_data)
{
	struct ctx_conn_t *ctx_conn = user_data;

	strcat(ctx_conn->buffer, "<p>path: ");
	strcat(ctx_conn->buffer, path);
	strcat(ctx_conn->buffer, "</p>");

	return 0;
}


static int
FormHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	/* Handler may access the request info using httplib_get_request_info */
	const struct httplib_request_info *req_info = httplib_get_request_info(conn);

	/* It would be possible to check the request info here before calling
	 * httplib_handle_form_request. */
	(void)req_info;

	/* Call the form handler */
	struct ctx_conn_t ctx_conn = {.ctx = ctx, .conn = conn};
	struct httplib_form_data_handler fdh = {field_found, field_get, field_stored, &ctx_conn};
	int ret = httplib_handle_form_request(ctx, conn, &fdh);

	char body[MAX_BUFFER];
	snprintf(
		body, sizeof(body),
		"<!DOCTYPE html>"
		"<html><body>"
		"<h2>This is the Form handler</h2>"
	        "<p>There were %d form fields:</p>"
		"%s"
		"</body></html>",
		ret,
		ctx_conn.buffer);

	httplib_printf(ctx, conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %zd\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"%s",
		strlen(body),
		body);

	return 200;
}


static int
FileUploadForm(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	char body[MAX_BUFFER];
	snprintf(
		body, sizeof(body),
		"<!DOCTYPE html>"
		"<html><body>"
		"<h2>This is the File Upload handler</h2>"
		"<form action=\"%s\" method=\"POST\" enctype=\"multipart/form-data\">"
		"    <input type=\"file\" name=\"filesin\" multiple>"
		"    <input type=\"submit\" value=\"Submit\">"
		"</form>"
		"</body></html>",
		(const char *)cbdata);

	httplib_printf(ctx, conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %zd\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"%s",
		strlen(body),
		body);

	return 200;
}

struct tfile_checksum {
	char name[128];
	uint64_t length;
	md5_state_t chksum;
};

#define MAX_FILES (10)

struct tfiles_checksums {
	int index;
	struct tfile_checksum file[MAX_FILES];
};


static int
field_disp_read_on_the_fly(const char *key,
                           const char *filename,
                           char *path,
                           size_t pathlen,
                           void *user_data)
{
	struct tfiles_checksums *context = (struct tfiles_checksums *)user_data;

	(void)key;
	(void)path;
	(void)pathlen;

	if (context->index < MAX_FILES) {
		context->index++;
		strncpy(context->file[context->index - 1].name, filename, 128);
		context->file[context->index - 1].name[127] = 0;
		context->file[context->index - 1].length = 0;
		md5_init(&(context->file[context->index - 1].chksum));
		return FORM_FIELD_STORAGE_GET;
	}
	return FORM_FIELD_STORAGE_ABORT;
}


static int
field_get_checksum(const char *key,
                   const char *value,
                   size_t valuelen,
                   void *user_data)
{
	struct tfiles_checksums *context = (struct tfiles_checksums *)user_data;
	(void)key;

	context->file[context->index - 1].length += valuelen;
	md5_append(&(context->file[context->index - 1].chksum),
	           (const md5_byte_t *)value,
	           valuelen);

	return 0;
}


static int
ChecksumHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	/* Handler may access the request info using httplib_get_request_info */
	const struct httplib_request_info *req_info = httplib_get_request_info(conn);

	/* It would be possible to check the request info here before calling
	 * httplib_handle_form_request. */
	(void)req_info;

	/* Call the form handler */
	struct tfiles_checksums chksums = {0};
	struct httplib_form_data_handler fdh = {field_disp_read_on_the_fly, field_get_checksum, 0, &chksums};

	char files[MAX_BUFFER] = {0};

	int ret = httplib_handle_form_request(ctx, conn, &fdh);
	for (int i = 0; i < chksums.index; i++) {
		md5_byte_t digest[16] = {0};

		md5_finish(&(chksums.file[i].chksum), digest);

		char file[MAX_BUFFER] = {0};
		snprintf(
			file, sizeof(file),
			"<p>File \"%s\" (%llu bytes): %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x</p>",
			chksums.file[i].name,
			chksums.file[i].length,
			(unsigned int)digest[ 0],
			(unsigned int)digest[ 1],
			(unsigned int)digest[ 2],
			(unsigned int)digest[ 3],
			(unsigned int)digest[ 4],
			(unsigned int)digest[ 5],
			(unsigned int)digest[ 6],
			(unsigned int)digest[ 7],
			(unsigned int)digest[ 8],
			(unsigned int)digest[ 9],
			(unsigned int)digest[10],
			(unsigned int)digest[11],
			(unsigned int)digest[12],
			(unsigned int)digest[13],
			(unsigned int)digest[14],
			(unsigned int)digest[15]
			);
		strcat(files, file);
	}

	char body[MAX_BUFFER];
	snprintf(
		body, sizeof(body),
		"<!DOCTYPE html>"
		"<html><body>"
		"<h2>This is the File Checksum handler</h2>"
	        "<p>There were %d files:</p>"
		"%s"
		"</body></html>",
		ret,
		files);

	httplib_printf(ctx, conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %zd\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"%s",
		strlen(body),
		body);

	return 200;
}


static int
CookieHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	/* Handler may access the request info using httplib_get_request_info */
	const struct httplib_request_info *req_info = httplib_get_request_info(conn);

	const char *cookie = httplib_get_header(conn, "Cookie");

	char first_str[64] = {0};
	httplib_get_cookie(cookie, "first", first_str, sizeof(first_str));

	int first = (first_str[0] == 0) ? 1 : 0;

	char count_str[64] = {0};
	httplib_get_cookie(cookie, "count", count_str, sizeof(count_str));

	int count = (count_str[0] == 0) ? 1 : atoi(count_str);

	if (first) {
		char body[MAX_BUFFER];
		snprintf(
			body, sizeof(body),
			"<!DOCTYPE html>"
			"<html><body>"
			"<h2>This is the Cookie handler</h2>"
			"<p>The actual uri is %s.</p>"
			"<p>This is the first time you opened this page.</p>"
			"</body></html>",
			req_info->uri);

		time_t t = time(0);
		struct tm *ptm = localtime(&t);

		httplib_printf(ctx, conn,
			"HTTP/1.1 200 OK\r\n"
			"Content-Length: %zd\r\n"
			"Content-Type: text/html\r\n"
			"Set-Cookie: count=%d\r\n"
			"Set-Cookie: first=%04i-%02i-%02iT%02i:%02i:%02i\r\n"
			"\r\n"
			"%s",
			strlen(body),
			count + 1,
			ptm->tm_year + 1900,
			ptm->tm_mon + 1,
			ptm->tm_mday,
			ptm->tm_hour,
			ptm->tm_min,
			ptm->tm_sec,
			body);
	} else {
		char body[MAX_BUFFER];
		snprintf(
			body, sizeof(body),
			"<!DOCTYPE html>"
			"<html><body>"
			"<h2>This is the Cookie handler</h2>"
			"<p>The actual uri is %s.</p>"
			"<p>You first opened this page on %s.</p>"
			"<p>You've opened this page %d times.</p>"
			"</body></html>",
			req_info->uri,
			first_str,
			count);

		httplib_printf(ctx, conn,
			"HTTP/1.1 200 OK\r\n"
			"Content-Length: %zd\r\n"
			"Content-Type: text/html\r\n"
			"Set-Cookie: count=%d\r\n"
			"\r\n"
			"%s",
			strlen(body),
			count + 1,
			body);
	}

	return 200;
}


static int
WebSocketStartHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	char body[MAX_BUFFER];
	snprintf(
		body, sizeof(body),
		"<!DOCTYPE html>"
		"<html>"
		"<head>"
		"<script>"
		"function load() {\n"
		"    var wsproto = (location.protocol === 'https:') ? 'wss:' : 'ws:';\n"
		"    var connection = new WebSocket(wsproto + '//' + window.location.host + '/websocket');\n"
		"    connection.onmessage = function (e) {\n"
		"        document.getElementById('websock_text_field').innerHTML = e.data;\n"
		"    }\n"
		"    connection.onerror = function (error) {\n"
		"        alert('WebSocket error');\n"
		"        connection.close();\n"
		"    }\n"
		"}\n"
		"</script>"
		"</head>"
		"<body onload=\"load()\">"
		"<h2>This is the WebSocket handler</h2>"
		"<p id='websock_text_field'>No websocket connection yet...</p>"
		"</body>"
		"</html>");

	httplib_printf(ctx, conn,
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %zd\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"%s",
		strlen(body),
		body);

	return 200;
}


/* MAX_WS_CLIENTS defines how many clients can connect to a websocket at the
 * same time. The value 5 is very small and used here only for demonstration;
 * it can be easily tested to connect more than MAX_WS_CLIENTS clients.
 * A real server should use a much higher number, or better use a dynamic list
 * of currently connected websocket clients. */
#define MAX_WS_CLIENTS (5)

struct t_ws_client {
	struct httplib_connection *conn;
	int state;
} static ws_clients[MAX_WS_CLIENTS];


#define ASSERT(x)									\
	{										\
		if (!(x)) {								\
			printf("Assertion failed in line %u\n", (unsigned)__LINE__);	\
		}									\
	}


static int
WebSocketConnectHandler(struct httplib_context *ctx, const struct httplib_connection *conn, void *cbdata)
{
	int reject = 1;
	int i;

	httplib_lock_context(ctx);
	for (i = 0; i < MAX_WS_CLIENTS; i++) {
		if (ws_clients[i].conn == NULL) {
			ws_clients[i].conn = (struct httplib_connection *)conn;
			ws_clients[i].state = 1;
			httplib_set_user_connection_data(ws_clients[i].conn, (void *)(ws_clients + i));
			reject = 0;
			break;
		}
	}
	httplib_unlock_context(ctx);

	printf("Websocket client %s\r\n\r\n", (reject ? "rejected" : "accepted"));

	return reject;
}


static void
WebSocketReadyHandler(struct httplib_context *ctx, struct httplib_connection *conn, void *cbdata)
{
	const char *text = "Hello from the websocket ready handler";
	struct t_ws_client *client = httplib_get_user_connection_data(conn);

	httplib_websocket_write(ctx, conn, WEBSOCKET_OPCODE_TEXT, text, strlen(text));
	printf("Greeting message sent to websocket client\r\n\r\n");
	ASSERT(client->conn == conn);
	ASSERT(client->state == 1);

	client->state = 2;
}


static int
WebsocketDataHandler(struct httplib_context *ctx, struct httplib_connection *conn, int bits, char *data, size_t len, void *cbdata)
{
	struct t_ws_client *client = httplib_get_user_connection_data(conn);
	ASSERT(client->conn == conn);
	ASSERT(client->state >= 1);

	printf("Websocket got data:\r\n");
	for (size_t i = 0; i < len; i++) {
		printf("%c", data[i]);
	}
	printf("\r\n\r\n");

	return 1;
}


static void
WebSocketCloseHandler(struct httplib_context *ctx, const struct httplib_connection *conn, void *cbdata)
{
	struct t_ws_client *client = httplib_get_user_connection_data(conn);
	ASSERT(client->conn == conn);
	ASSERT(client->state >= 1);

	httplib_lock_context(ctx);
	client->state = 0;
	client->conn = NULL;
	httplib_unlock_context(ctx);

	printf("Client droped from the set of webserver connections\r\n\r\n");
}


static void
InformWebsockets(struct httplib_context *ctx)
{
	static unsigned long cnt = 0;
	char text[32];
	int i;

	sprintf(text, "%lu", ++cnt);

	httplib_lock_context(ctx);
	for (i = 0; i < MAX_WS_CLIENTS; i++) {
		if (ws_clients[i].state == 2) {
			httplib_websocket_write(ctx, ws_clients[i].conn, WEBSOCKET_OPCODE_TEXT, text, strlen(text));
		}
	}
	httplib_unlock_context(ctx);
}


static int
init_ssl(struct httplib_context *ctx, void *ssl_context, void *user_data)
{
	/* Add application specific SSL initialization */

	return 0;
}


int
main(int argc, char *argv[])
{
	int has_ssl = httplib_check_feature(2);

	/* Start libhttp web server */
	struct httplib_callbacks callbacks = {0};
	if (has_ssl) {
		callbacks.init_ssl = init_ssl;
	}

	struct httplib_option options[8] = {
		{"document_root", "."},
		{"enable_directory_listing", "yes"},
		{"error_log_file", "error.log"},
		{"listening_ports", "8888"},
		{0},  // "ssl_certificate"
		{0},  // "ssl_cipher_list"
		{0},  // "ssl_protocol_version"
		{0}
	};
	if (has_ssl) {
		// A very klunky approach!
		options[3].value = "8888r,8843s";

		options[4].name = "ssl_certificate";
		options[4].value = "../../resources/cert/server.pem";

		options[5].name = "ssl_cipher_list";
		options[5].value = "DES-CBC3-SHA:AES128-SHA:AES128-GCM-SHA256";

		options[6].name = "ssl_protocol_version";
		options[6].value = "3";
	}

	struct httplib_context *ctx = httplib_start(&callbacks, 0, options);
	if (!ctx) {
		exit(EXIT_FAILURE);
	}

	printf("Server started.\n");

	/* Add handler /example, to explain the example */
	httplib_set_request_handler(ctx, "/example", ExampleHandler, 0);
	httplib_set_request_handler(ctx, "/exit", ExitHandler, 0);

	/* Add handler for /A* and special handler for /A/B */
	httplib_set_request_handler(ctx, "/A", AHandler, 0);
	httplib_set_request_handler(ctx, "/A/B", ABHandler, 0);

	/* Add handler for /B, /B/A, /B/B but not for /B* */
	httplib_set_request_handler(ctx, "/B$", BXHandler, (void *)0);
	httplib_set_request_handler(ctx, "/B/A$", BXHandler, (void *)1);
	httplib_set_request_handler(ctx, "/B/B$", BXHandler, (void *)2);

	/* Add handler for all files with .foo extention */
	httplib_set_request_handler(ctx, "**.foo$", FooHandler, 0);

	/* Add handler for /close extention */
	httplib_set_request_handler(ctx, "/close", CloseHandler, 0);

	/* Add handler for /form  (serve a file outside the document root) */
	httplib_set_request_handler(ctx, "/form", FileHandler, (void *)"./form.html");

	/* Add handler for form data */
	httplib_set_request_handler(ctx,
				"/handle_form.embedded_c.example.callback",
				FormHandler,
				(void *)0);

	/* Add a file upload handler for parsing files on the fly */
	httplib_set_request_handler(ctx, "/on_the_fly_form", FileUploadForm, (void *)"/on_the_fly_form.md5.callback");
	httplib_set_request_handler(ctx, "/on_the_fly_form.md5.callback", ChecksumHandler, (void *)0);

	/* Add handler for /cookie example */
	httplib_set_request_handler(ctx, "/cookie", CookieHandler, 0);

	/* Add HTTP site to open a websocket connection */
	httplib_set_request_handler(ctx, "/websocket", WebSocketStartHandler, 0);

	/* WS site for the websocket connection */
	httplib_set_websocket_handler(ctx,
				"/websocket",
				(httplib_websocket_connect_handler)WebSocketConnectHandler,
				WebSocketReadyHandler,
				WebsocketDataHandler,
				(httplib_websocket_close_handler)WebSocketCloseHandler,
				0);

	/* List all listening ports */
	struct httplib_server_ports ports[32] = {0};
	int port_cnt = httplib_get_server_ports(ctx, 32, ports);
	printf("\n%d listening ports:\n\n", port_cnt);

	for (int n = 0; n < port_cnt && n < 32; n++) {
		const char *proto = ports[n].has_ssl ? "https" : "http";
		const char *host;

		if ((ports[n].protocol & 1) == 1) {
			/* IPv4 */
			host = "127.0.0.1";
			printf("Browse files at %s://%s:%d/\n", proto, host, ports[n].port);
			printf("Run example at %s://%s:%d%s\n",
			       proto,
			       host,
			       ports[n].port,
			       "/example");
			printf(
			    "Exit at %s://%s:%d%s\n", proto, host, ports[n].port, "/exit");
			printf("\n");
		}

		if ((ports[n].protocol & 2) == 2) {
			/* IPv6 */
			host = "[::1]";
			printf("Browse files at %s://%s:%d/\n", proto, host, ports[n].port);
			printf("Run example at %s://%s:%d%s\n",
			       proto,
			       host,
			       ports[n].port,
			       "/example");
			printf(
			    "Exit at %s://%s:%d%s\n", proto, host, ports[n].port, "/exit");
			printf("\n");
		}
	}

	/* Wait until the server exits */
	while (!exitNow) {
		InformWebsockets(ctx);
		sleep(1);
	}

	/* Stop the server */
	httplib_stop(ctx);

	printf("Server stopped.\n");

	return EXIT_SUCCESS;
}
