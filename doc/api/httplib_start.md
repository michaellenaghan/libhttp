# LibHTTP API Reference

### `httplib_start( callbacks, user_data, options );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`callbacks`**|`const struct httplib_callbacks *`| A structure with optional callback functions to process requests from the web server |
|**`user_data`**|`void *`| A pointer to optional user data |
|**`options`**|`char **`| A list of options used to initialize the web server. The list consists of an NULL terminated list of option-value string pairs. |

### Return Value

| Type | Description |
| :--- | :--- |
|`struct httplib_context *`| A pointer to a context structure when successful, or NULL in case of failure |

### Description

The function `httplib_start()` is the only function needed to call to initialize the webserver. After the function returns and a pointer to a context structure is provided, it is guaranteed that the server has started and is listening on the designated ports. In case of failure a NULL pointer is returned.  The behaviour of the web server is controlled by a list of callback functions and a list of options.  The callback functions can do application specific processing of events which are encountered by the webserver. If a specific callback function is set to NULL, the webserver uses their default callback routine. The options list controls how the webserver should be started and contains settings for for example the ports to listen on, the maximum number of threads created to handle requests in parallel and if settings for SSL encryption.

As a side effect on Unix systems, SIGCHLD and SIGPIPE signals will be ignored. If custom processing is needed for these signals, signal handlers must be setup after the call to `httplib_start()` has completed.

### See Also

* [`struct httplib_callbacks;`](httplib_callbacks.md)
* [`httplib_stop();`](httplib_stop.md)
