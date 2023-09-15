# LibHTTP API Reference

### `httplib_base64_decode( src, src_len, dst, dst_len );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`src`**|`const char *`|Pointer to the source buffer|
|**`src_len`**|`size_t`|The number of bytes in the source buffer, not including NUL|
|**`dst`**|`unsigned char *`|Pointer to the destination buffer|
|**`dst_len`**|`*size_t`|Pointer to the number of bytes in the destination buffer, not including NUL|

### Return Value

| Type | Description |
| :--- | :--- |
|`ssize_t`|The size of the destination string (not including NUL) or an error|

### Description

The function `httplib_base64_decode()` decodes a BASE64 decoded NUL terminated string to a block of binary data. The destination buffer should be large enough to contain the whole string and NUL terminating character.

If the function succeeds the actual number of used bytes in the destination buffer (not
including NUL) is returned.

An error is indicated with the return value **-1**.

If `src` or `dst` is `NULL`, or if `src` isn't BASE64, or if `dst_len` is too small, `dst_len` is set to the required number of bytes (not including NUL).

### See Also

* [`httplib_base64_encode();`](httplib_base64_encode.md)
