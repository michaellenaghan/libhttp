# LibHTTP API Reference

### `httplib_set_connection_user_data( conn, data );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`conn`**|`struct httplib_connection *`|connection to add the user data|
|**`data`**|`void *`|Pointer to the user data|

### Return Value

*none*

### Description

The function `httplib_set_connection_user_data()` can be used to add or change the user data pointer attached to a connection. Using the value NULL in the call will remove a previously assigned user data pointer.

### See Also

* [`httplib_get_connection_user_data();`](httplib_get_connection_user_data.md)
