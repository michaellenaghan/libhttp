# LibHTTP API Reference

### `httplib_system_exit();`

### Parameters

*none*

### Return Value

| Type | Description |
| :--- | :--- |
|`int`| Success or error code |

### Description

The function `httplib_system_exit()`` is called after all processing with other
LibHTTP function is completed to deinitialize the system. The
function returns 0 when successful, and -1 if an error occured.

Please note that this function terminates socket communications for all
threads and it should therefore be called only when all running threads have
terminated.

### See Also

* [`httplib_system_init();`](httplib_system_init.md)
