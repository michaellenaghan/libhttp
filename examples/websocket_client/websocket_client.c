/*
* Copyright (c) 2014-2015 the Civetweb developers
* Copyright (c) 2014 Jordan Shelley
* https://github.com/jshelley
* License http://opensource.org/licenses/mit-license.php MIT License
*/

/* This example is superseeded by other examples, and no longer 
 * actively maintained.
 * See examples/embedded_c for an up to date example.
 */

// Simple example program on how to use websocket client embedded C interface.
#ifdef _WIN32
#include <Windows.h>
#define sleep(x) Sleep(1000 * (x))
#else
#include <unistd.h>
#endif

#include <assert.h>
#include <string.h>
#include "civetweb.h"

#define DOCUMENT_ROOT "."
#define PORT "8888"
#define SSL_CERT "./ssl/server.pem"

const char *websocket_welcome_msg = "websocket welcome\n";
const size_t websocket_welcome_msg_len = 18 /* strlen(websocket_welcome_msg) */;
const char *websocket_acknowledge_msg = "websocket msg ok\n";
const size_t websocket_acknowledge_msg_len =
    17 /* strlen(websocket_acknowledge_msg) */;
const char *websocket_goodbye_msg = "websocket bye\n";
const size_t websocket_goodbye_msg_len = 14 /* strlen(websocket_goodbye_msg) */;


/*************************************************************************************/
/* WEBSOCKET SERVER */
/*************************************************************************************/
int
websocket_server_connect(const struct httplib_connection *conn, void *_ignored)
{
	printf("Server: Websocket connected\n");
	return 0; /* return 0 to accept every connection */
}


void
websocket_server_ready(struct httplib_connection *conn, void *_ignored)
{
	printf("Server: Websocket ready\n");

	/* Send websocket welcome message */
	httplib_lock_connection(conn);
	httplib_websocket_write(conn,
	                   WEBSOCKET_OPCODE_TEXT,
	                   websocket_welcome_msg,
	                   websocket_welcome_msg_len);
	httplib_unlock_connection(conn);
}


int
websocket_server_data(struct httplib_connection *conn,
                      int bits,
                      char *data,
                      size_t data_len,
                      void *_ignored)
{
	printf("Server: Got %lu bytes from the client\n", (unsigned long)data_len);
	printf("Server received data from client: ");
	fwrite(data, 1, data_len, stdout);
	printf("\n");

	if (data_len < 3 || 0 != memcmp(data, "bye", 3)) {
		/* Send websocket acknowledge message */
		httplib_lock_connection(conn);
		httplib_websocket_write(conn,
		                   WEBSOCKET_OPCODE_TEXT,
		                   websocket_acknowledge_msg,
		                   websocket_acknowledge_msg_len);
		httplib_unlock_connection(conn);
	} else {
		/* Send websocket acknowledge message */
		httplib_lock_connection(conn);
		httplib_websocket_write(conn,
		                   WEBSOCKET_OPCODE_TEXT,
		                   websocket_goodbye_msg,
		                   websocket_goodbye_msg_len);
		httplib_unlock_connection(conn);
	}

	return 1; /* return 1 to keep the connetion open */
}


void
websocket_server_connection_close(const struct httplib_connection *conn,
                                  void *_ignored)
{
	printf("Server: Close connection\n");

	/* Can not send a websocket goodbye message here - the connection is already
	 * closed */
}


struct httplib_context *
start_websocket_server()
{
	const char *options[] = {"document_root",
	                         DOCUMENT_ROOT,
	                         "ssl_certificate",
	                         SSL_CERT,
	                         "listening_ports",
	                         PORT,
	                         "request_timeout_ms",
	                         "5000",
	                         0};
	struct httplib_callbacks callbacks;
	struct httplib_context *ctx;

	memset(&callbacks, 0, sizeof(callbacks));

	ctx = httplib_start(&callbacks, 0, options);

	httplib_set_websocket_handler(ctx,
	                         "/websocket",
	                         websocket_server_connect,
	                         websocket_server_ready,
	                         websocket_server_data,
	                         websocket_server_connection_close,
	                         NULL);

	return ctx;
}


/*************************************************************************************/
/* WEBSOCKET CLIENT */
/*************************************************************************************/
struct tclient_data {
	void *data;
	size_t len;
	int closed;
};

static int
websocket_client_data_handler(struct httplib_connection *conn, int flags, char *data, size_t data_len, void *user_data) {

	struct httplib_context *ctx = httplib_get_context(conn);
	struct tclient_data *pclient_data =
	    (struct tclient_data *)httplib_get_user_data(ctx);

	printf("Client received data from server: ");
	fwrite(data, 1, data_len, stdout);
	printf("\n");

	pclient_data->data = malloc(data_len);
	assert(pclient_data->data != NULL);
	memcpy(pclient_data->data, data, data_len);
	pclient_data->len = data_len;

	return 1;
}

static void
websocket_client_close_handler(const struct httplib_connection *conn,
                               void *user_data)
{
	struct httplib_context *ctx = httplib_get_context(conn);
	struct tclient_data *pclient_data =
	    (struct tclient_data *)httplib_get_user_data(ctx);

	printf("Client: Close handler\n");
	pclient_data->closed++;
}


int
main(int argc, char *argv[])
{
	struct httplib_context *ctx = NULL;
	struct tclient_data client1_data = {NULL, 0, 0};
	struct tclient_data client2_data = {NULL, 0, 0};
	struct tclient_data client3_data = {NULL, 0, 0};
	struct httplib_connection *newconn1 = NULL;
	struct httplib_connection *newconn2 = NULL;
	struct httplib_connection *newconn3 = NULL;
	char ebuf[100] = {0};

	assert(websocket_welcome_msg_len == strlen(websocket_welcome_msg));

	/* First set up a websocket server */
	ctx = start_websocket_server();
	assert(ctx != NULL);
	printf("Server init\n\n");

	/* Then connect a first client */
	newconn1 = httplib_connect_websocket_client("localhost",
	                                       atoi(PORT),
	                                       0,
	                                       ebuf,
	                                       sizeof(ebuf),
	                                       "/websocket",
	                                       NULL,
	                                       websocket_client_data_handler,
	                                       websocket_client_close_handler,
	                                       &client1_data);

	if (newconn1 == NULL) {
		printf("Error: %s", ebuf);
		return 1;
	}

	sleep(1); /* Should get the websocket welcome message */
	assert(client1_data.closed == 0);
	assert(client2_data.closed == 0);
	assert(client2_data.data == NULL);
	assert(client2_data.len == 0);
	assert(client1_data.data != NULL);
	assert(client1_data.len == websocket_welcome_msg_len);
	assert(!memcmp(client1_data.data,
	               websocket_welcome_msg,
	               websocket_welcome_msg_len));
	free(client1_data.data);
	client1_data.data = NULL;
	client1_data.len = 0;

	httplib_websocket_client_write(newconn1, WEBSOCKET_OPCODE_TEXT, "data1", 5);

	sleep(1); /* Should get the acknowledge message */
	assert(client1_data.closed == 0);
	assert(client2_data.closed == 0);
	assert(client2_data.data == NULL);
	assert(client2_data.len == 0);
	assert(client1_data.data != NULL);
	assert(client1_data.len == websocket_acknowledge_msg_len);
	assert(!memcmp(client1_data.data,
	               websocket_acknowledge_msg,
	               websocket_acknowledge_msg_len));
	free(client1_data.data);
	client1_data.data = NULL;
	client1_data.len = 0;

	/* Now connect a second client */
	newconn2 = httplib_connect_websocket_client("localhost",
	                                       atoi(PORT),
	                                       0,
	                                       ebuf,
	                                       sizeof(ebuf),
	                                       "/websocket",
	                                       NULL,
	                                       websocket_client_data_handler,
	                                       websocket_client_close_handler,
	                                       &client2_data);

	if (newconn2 == NULL) {
		printf("Error: %s", ebuf);
		return 1;
	}

	sleep(1); /* Client 2 should get the websocket welcome message */
	assert(client1_data.closed == 0);
	assert(client2_data.closed == 0);
	assert(client1_data.data == NULL);
	assert(client1_data.len == 0);
	assert(client2_data.data != NULL);
	assert(client2_data.len == websocket_welcome_msg_len);
	assert(!memcmp(client2_data.data,
	               websocket_welcome_msg,
	               websocket_welcome_msg_len));
	free(client2_data.data);
	client2_data.data = NULL;
	client2_data.len = 0;

	httplib_websocket_client_write(newconn1, WEBSOCKET_OPCODE_TEXT, "data2", 5);

	sleep(1); /* Should get the acknowledge message */
	assert(client1_data.closed == 0);
	assert(client2_data.closed == 0);
	assert(client2_data.data == NULL);
	assert(client2_data.len == 0);
	assert(client1_data.data != NULL);
	assert(client1_data.len == websocket_acknowledge_msg_len);
	assert(!memcmp(client1_data.data,
	               websocket_acknowledge_msg,
	               websocket_acknowledge_msg_len));
	free(client1_data.data);
	client1_data.data = NULL;
	client1_data.len = 0;

	httplib_websocket_client_write(newconn1, WEBSOCKET_OPCODE_TEXT, "bye", 3);

	sleep(1); /* Should get the goodbye message */
	assert(client1_data.closed == 0);
	assert(client2_data.closed == 0);
	assert(client2_data.data == NULL);
	assert(client2_data.len == 0);
	assert(client1_data.data != NULL);
	assert(client1_data.len == websocket_goodbye_msg_len);
	assert(!memcmp(client1_data.data,
	               websocket_goodbye_msg,
	               websocket_goodbye_msg_len));
	free(client1_data.data);
	client1_data.data = NULL;
	client1_data.len = 0;

	httplib_close_connection(newconn1);

	sleep(1); /* Won't get any message */
	assert(client1_data.closed == 1);
	assert(client2_data.closed == 0);
	assert(client1_data.data == NULL);
	assert(client1_data.len == 0);
	assert(client2_data.data == NULL);
	assert(client2_data.len == 0);

	httplib_websocket_client_write(newconn2, WEBSOCKET_OPCODE_TEXT, "bye", 3);

	sleep(1); /* Should get the goodbye message */
	assert(client1_data.closed == 1);
	assert(client2_data.closed == 0);
	assert(client1_data.data == NULL);
	assert(client1_data.len == 0);
	assert(client2_data.data != NULL);
	assert(client2_data.len == websocket_goodbye_msg_len);
	assert(!memcmp(client2_data.data,
	               websocket_goodbye_msg,
	               websocket_goodbye_msg_len));
	free(client2_data.data);
	client2_data.data = NULL;
	client2_data.len = 0;

	httplib_close_connection(newconn2);

	sleep(1); /* Won't get any message */
	assert(client1_data.closed == 1);
	assert(client2_data.closed == 1);
	assert(client1_data.data == NULL);
	assert(client1_data.len == 0);
	assert(client2_data.data == NULL);
	assert(client2_data.len == 0);

	/* Connect client 3 */
	newconn3 = httplib_connect_websocket_client("localhost",
	                                       atoi(PORT),
	                                       0,
	                                       ebuf,
	                                       sizeof(ebuf),
	                                       "/websocket",
	                                       NULL,
	                                       websocket_client_data_handler,
	                                       websocket_client_close_handler,
	                                       &client3_data);

	sleep(1); /* Client 3 should get the websocket welcome message */
	assert(client1_data.closed == 1);
	assert(client2_data.closed == 1);
	assert(client3_data.closed == 0);
	assert(client1_data.data == NULL);
	assert(client1_data.len == 0);
	assert(client2_data.data == NULL);
	assert(client2_data.len == 0);
	assert(client3_data.data != NULL);
	assert(client3_data.len == websocket_welcome_msg_len);
	assert(!memcmp(client3_data.data,
	               websocket_welcome_msg,
	               websocket_welcome_msg_len));
	free(client3_data.data);
	client3_data.data = NULL;
	client3_data.len = 0;

	httplib_stop(ctx);
	printf("Server shutdown\n");

	sleep(10);

	assert(client3_data.closed == 1);

	return 0;
}
