# LibHTTP API Reference

### `httplib_system_init();`

### Parameters

*none*

### Return Value

| Type | Description |
| :--- | :--- |
|`int`| Success or error code |

### Description

The function 1httplib_system_init()`` is called before any other LibHTTP
functions to do system initialization. The function returns 0 when
successful and -1 if an error occured.

### See Also

* [`httplib_system_exit();`](httplib_system_exit.md)
