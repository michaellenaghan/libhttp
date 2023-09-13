# LibHTTP API Reference

### `httplib_check_feature( feature );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`feature`**|`enum httplib_feature const`| A value indicating the feature to be checked |

### Return Value

| Type | Description |
| :--- | :--- |
|`bool`| A value indicating whether or not a feature is available. A `true` value indicates available, and `false` indicates unavailable. |

### Description

The function `httplib_check_feature()` can be called from an application program to check of specific features have been compiled in the LibHTTP version which the application has been linked to. The feature to check is provided as `enum httplib_feature` parameter. If the feature is available in the currently linked library version, a value `true` is returned. Otherwise the function `httplib_check_feature()` returns `false`.

The following parameter values can be used:

| Value | Compilation option | Description |
| :---: | :---: | :--- |
| `FEATURE_SSL` | NO_SSL | *Support for HTTPS*. If this feature is available, the webserver van use encryption in the client-server connection. SSLv2, SSLv3, TLSv1.0, TLSv1.1 and TLSv1.2 are supported depending on the SSL library LibHTTP has been compiled with, but which protocols are used effectively when the server is running is dependent on the options used when the server is started. |

Parameter values other than the values mentioned above will give undefined results.

### See Also

* [`httplib_get_option();`](httplib_get_option.md)
