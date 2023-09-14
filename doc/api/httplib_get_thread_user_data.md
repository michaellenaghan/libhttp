# LibHTTP API Reference

### `httplib_get_thread_user_data( void );`

### Parameters

*none*

### Return Value

| Type | Description |
| :--- | :--- |
|`void *`|A pointer to the user data, or NULL if no user data was registered with the thread|

### Description

The function `httplib_get_thread_user_data()` returns the user data associated with a thread. This user data is represented with a pointer which has been prevously registered with a call to the `init_thread()` callback. With this function it is possible to pass state information between callback functions refering to a specific thread.

### See Also

* [`struct httplib_callbacks;`](httplib_callbacks.md)
