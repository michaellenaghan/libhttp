
Overview
=====

LibHTTP is small and easy to use web server. It may be embedded into
C host applications or used as a stand-alone server. See `Embedding.md`
for information on embedding LibHTTP into host applications.

The stand-alone server is self-contained, and does not require any external
software to run. Some Windows users may need to install the
[Visual C++ Redistributable](http://www.microsoft.com/en-us/download/details.aspx?id=30679).

Installation
----

LibHTTP uses shell-like glob patterns. Pattern match starts at the beginning
of the string, so essentially patterns are prefix patterns. Syntax is as
follows:

     **      Matches everything
     *       Matches everything but slash character, '/'
     ?       Matches any character
     $       Matches the end of the string
     |       Matches if pattern on the left side or the right side matches.

All other characters in the pattern match themselves. Examples:

    **.foo$      Any string that ends with .foo
    /foo         Any string that begins with /foo
    **a$|**b$    Any string that ends with a or b

# Configuration Options

Below is a list of configuration options understood by LibHTTP.
Every option is followed by its default value. If a default value is not
present, then the default is empty.

### access\_control\_allow\_origin
Access-Control-Allow-Origin header field, used for cross-origin resource
sharing (CORS).
See the [Wikipedia page on CORS](http://en.wikipedia.org/wiki/Cross-origin_resource_sharing).

### access\_control\_list
An Access Control List (ACL) allows restrictions to be put on the list of IP
addresses which have access to the web server. In the case of the LibHTTP
web server, the ACL is a comma separated list of IP subnets, where each
subnet is pre-pended by either a `-` or a `+` sign. A plus sign means allow,
where a minus sign means deny. If a subnet mask is omitted, such as `-1.2.3.4`,
this means to deny only that single IP address.

Subnet masks may vary from 0 to 32, inclusive. The default setting is to allow
all accesses. On each request the full list is traversed, and
the last match wins. Examples:

    -0.0.0.0/0,+192.168/16    deny all accesses, only allow 192.168/16 subnet

To learn more about subnet masks, see the
[Wikipedia page on Subnetwork](http://en.wikipedia.org/wiki/Subnetwork).

### access\_log\_file
Path to a file for access logs. Either full path, or relative to the current
working directory. If absent (default), then accesses are not logged.

### allow\_sendfile\_call `yes`
This option can be used to enable or disable the use of the Linux `sendfile` system call. It is only available for Linux systems and only affects HTTP (not HTTPS) connections. While using the `sendfile` call will lead to a performance boost for HTTP connections, this call may be broken for some file systems and some operating system versions.

### authentication\_domain `localhost`
Authorization realm used for HTTP digest authentication. This domain is
used in the encoding of the `.htpasswd` authorization files as well.
Changing the domain retroactively will render the existing passwords useless.

### debug\_level `LH_DEBUG_WARNING`

### decode\_url `yes`
URL encoded request strings are decoded in the server, unless it is disabled
by setting this option to `no`.

### document\_root `.`
A directory to serve. By default, the current working directory is served.
The current directory is commonly referenced as dot (`.`).
It is recommended to use an absolute path for document\_root, in order to
avoid accidentally serving the wrong directory.

### enable\_directory\_listing `no`
Enable directory listing, either `yes` or `no`.

### enable\_keep\_alive `yes`
Enable connection keep alive, either `yes` or `no`.

Experimental feature. Allows clients to reuse TCP connection for subsequent
HTTP requests, which improves performance.
For this to work when using request handlers it is important to add the
correct Content-Length HTTP header for each request. If this is forgotten the
client will time out.

### error\_log\_file
Path to a file for error logs. Either full path, or relative to the current
working directory. If absent (default), then errors are not logged.

### error\_pages
This option may be used to specify a directory for user defined error pages.
The error pages may be specified for an individual http status code (e.g.,
404 - page requested by the client not found), a group of http status codes
(e.g., 4xx - all client errors) or all errors. The corresponding error pages
must be called error404.ext, error4xx.ext or error.ext, whereas the file
extention may be one of the extentions specified for the index_files option.
See the [Wikipedia page on HTTP status codes](http://en.wikipedia.org/wiki/HTTP_status_code).

### extra\_mime\_types
Extra mime types, in tha form `extension1=type1,exten-sion2=type2,...`.
See the [Wikipedia page on Internet media types](http://en.wikipedia.org/wiki/Internet_media_type).
Extension must include a leading dot. Example:
`.cpp=plain/text,.java=plain/text`

### global\_auth\_file
Path to a global passwords file, either full path or relative to the current
working directory. If set, per-directory `.htpasswd` files are ignored,
and all requests are authorized against that file.

The file has to include the realm set through `authentication_domain` and the
password in digest format:

    user:realm:digest
    test:test.com:ce0220efc2dd2fad6185e1f1af5a4327

Password files may be generated using `libhttp -A` as explained above, or
online tools e.g. [this generator](http://www.askapache.com/online-tools/htpasswd-generator).

### hide\_files\_patterns `.?*`
A pattern for the files to hide. Files that match the pattern will not
show up in directory listing and return `404 Not Found` if requested. Pattern
must be for a file name only, not including directory names. Example:

    libhttp -hide_files_patterns secret.txt|*.hide

### index\_files `index.html`
Comma-separated list of files to be treated as directory index files.
If more than one matching file is present in a directory, the one listed to the left
is used as a directory index.

### listening\_ports `8080`
Comma-separated list of ports to listen on. If the port is SSL, a
letter `s` must be appended, for example, `80,443s` will open
port 80 and port 443, and connections on port 443 will be SSL-ed.
For non-SSL ports, it is allowed to append letter `r`, meaning 'redirect'.
Redirect ports will redirect all their traffic to the first configured
SSL port. For example, if `listening_ports` is `80r,443s`, then all
HTTP traffic coming at port 80 will be redirected to HTTPS port 443.

It is possible to specify an IP address to bind to. In this case,
an IP address and a colon must be pre-pended to the port number.
For example, to bind to a loopback interface on port 80 and to
all interfaces on HTTPS port 443, use `127.0.0.1:80,443s`.

If the server is built with IPv6 support, `[::]:8080` can be used to
listen to IPv6 connections to port 8080. IPv6 addresses of network
interfaces can be specified as well,
e.g. `[::1]:8080` for the IPv6 loopback interface.

[::]:80 will bind to port 80 IPv6 only. In order to use port 80 for
all interfaces, both IPv4 and IPv6, use either the configuration
`80,[::]:80` (create one socket for IPv4 and one for IPv6 only),
or `+80` (create one socket for both, IPv4 and IPv6).
The `+`-notation to use IPv4 and IPv6 will only work in no network
interface is specified. Depending on your IPv6 network environment,
some configurations might not work (properly), so you have to test
to find the configuration most suitable for your needs.

It is possible to use network interface addresses (e.g., `192.0.2.3:80`,
`[2001:0db8::1234]:80`). To get a list of available network interface
addresses, use `ipconfig` (in a `cmd` window in Windows) or `ifconfig`
(in a Linux shell).

### num\_threads `50`
Number of worker threads. LibHTTP handles each incoming connection in a
separate thread. Therefore, the value of this option is effectively the number
of concurrent HTTP connections LibHTTP can handle.

### protect\_uri
Comma separated list of URI=PATH pairs, specifying that given
URIs must be protected with password files specified by PATH.
All Paths must be full file paths.

### put\_delete\_auth\_file
Passwords file for PUT and DELETE requests. Without a password file, it will not
be possible to PUT new files to the server or DELETE existing ones.

### request\_timeout `10000`
Timeout for network read and network write operations, in milliseconds.
If a client intends to keep long-running connection, either increase this
value or (better) use keep-alive messages.

### run\_as\_user
Switch to given user credentials after startup. Usually, this option is
required when LibHTTP needs to bind on privileged ports on UNIX. To do
that, LibHTTP needs to be started as root. From a security point of view,
running as root is not advisable, therefore this option can be used to drop
privileges. Example:

    libhttp -listening_ports 80 -run_as_user webserver

### ssl\_ca\_file
Path to a .pem file containing trusted certificates. The file may contain
more than one certificate.

### ssl\_ca\_path
Name of a directory containing trusted CA certificates. Each file in the
directory must contain only a single CA certificate. The files must be named
by the subject name’s hash and an extension of “.0”. If there is more than one
certificate with the same subject name they should have extensions ".0", ".1",
".2" and so on respectively.

### ssl\_certificate
Path to the SSL certificate file. This option is only required when at least
one of the `listening\_ports` is SSL. The file must be in PEM format,
and it must have both, private key and certificate, see for example
[ssl_cert.pem](https://github.com/lammertb/libhttp/blob/master/resources/ssl_cert.pem)
A description how to create a certificate can be found in doc/OpenSSL.md

### ssl\_cipher\_list
List of ciphers to present to the client. Entries should be separated by
colons, commas or spaces.

    ALL           All available ciphers
    ALL:!eNULL    All ciphers excluding NULL ciphers
    AES128:!MD5   AES 128 with digests other than MD5

See [this entry](https://www.openssl.org/docs/manmaster/apps/ciphers.html) in
OpenSSL documentation for full list of options and additional examples.

### ssl\_protocol\_version `0`
Sets the minimal accepted version of SSL/TLS protocol according to the table:

Protocols | Value
------------ | -------------
SSL2+SSL3+TLS1.0+TLS1.1+TLS1.2  | 0
SSL3+TLS1.0+TLS1.1+TLS1.2  | 1
TLS1.0+TLS1.1+TLS1.2 | 2
TLS1.1+TLS1.2 | 3
TLS1.2 | 4

### ssl\_short\_trust `no`
Enables the use of short lived certificates. This will allow for the certificates
and keys specified in `ssl_certificate`, `ssl_ca_file` and `ssl_ca_path` to be
exchanged and reloaded while the server is running.

In an automated environment it is advised to first write the new pem file to
a different filename and then to rename it to the configured pem file name to
increase performance while swapping the certificate.

Disk IO performance can be improved when keeping the certificates and keys stored
on a tmpfs (linux) on a system with very high throughput.

### ssl\_verify\_depth `9`
Sets maximum depth of certificate chain. If client's certificate chain is longer
than the depth set here connection is refused.

### ssl\_verify\_paths `yes`
Loads default trusted certificates locations set at openssl compile time.

### ssl\_verify\_peer `no`
Enable client's certificate verification by the server.

### static\_file\_max\_age `3600`
Set the maximum time (in seconds) a cache may store a static files.

This option will set the `Cache-Control: max-age` value for static files.
Dynamically generated content, i.e., content created by a script or callback,
must send cache control headers by themselfes.

A value >0 corresponds to a maximum allowed caching time in seconds.
This value should not exceed one year (RFC 2616, Section 14.21).
A value of 0 will send "do not cache" headers for all static files.
For values <0 and values >31622400, the behavior is undefined.

### tcp\_nodelay `0`
Enable TCP_NODELAY socket option on client connections.

If set the socket option will disable Nagle's algorithm on the connection
which means that packets will be sent as soon as possible instead of waiting
for a full buffer or timeout to occur.

    0    Keep the default: Nagel's algorithm enabled
    1    Disable Nagel's algorithm for all sockets

### url\_rewrite\_patterns
Comma-separated list of URL rewrites in the form of
`uri_pattern=file_or_directory_path`. When LibHTTP receives any request,
it constructs the file name to show by combining `document_root` and the URI.
However, if the rewrite option is used and `uri_pattern` matches the
requested URI, then `document_root` is ignored. Instead,
`file_or_directory_path` is used, which should be a full path name or
a path relative to the web server's current working directory. Note that
`uri_pattern`, as all LibHTTP patterns, is a prefix pattern.

This makes it possible to serve many directories outside from `document_root`,
redirect all requests to scripts, and do other tricky things. For example,
to imitate support for user home directories, do:

    libhttp -url_rewrite_patterns /~joe/=/home/joe/,/~bill=/home/bill/

### websocket\_root
Since websockets use a different URL scheme (ws, wss) than other http pages
(http, https), the files for websockets may also be served from a different
directory. By default, the document_root is used as websocket_root as well.

### websocket\_timeout `360000`
Timeout for network read and network write operations, in milliseconds.
If a client intends to keep long-running connection, either increase this
value or (better) use keep-alive messages. (`0` means "use the
`request_timeout`".)


# Common Problems
- LibHTTP fails to start. If LibHTTP exits immediately when started, this
  usually indicates a syntax error in the configuration file
  (named `libhttp.conf` by default) or the command-line arguments.
  Syntax checking is omitted from LibHTTP to keep its size low. However,
  the Manual should be of help. Note: the syntax changes from time to time,
  so updating the config file might be necessary after executable update.

- Embedding with OpenSSL on Windows might fail because of calling convention.
  To force LibHTTP to use `__stdcall` convention, add `/Gz` compilation
  flag in Visual Studio compiler.
