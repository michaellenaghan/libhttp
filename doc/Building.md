Building LibHTTP
=========

This guide covers the build instructions for the stand-alone web server.
See [Embedding.md](Embedding.md) for information on extending an existing C application. A brief overview of the source code files can be found in [Embedding.md](Embedding.md) as well.

#### Where to get the source code?

The latest version can be found at
https://github.com/lammertb/libhttp

Released versions can be found at
https://github.com/lammertb/libhttp/releases


Building for Windows, Linux, BSD, OSX
---------
Builds for all operating systems are created with GNU Make.

## Using Make

```
make help
```
Get a list of all supported make option

```
make build
```
compile the code

```
make install
```
Install on the system, Linux only.

```
make lib
```
Build the static library.
The additional make options configure the library just as it would the application.

The *slib* option should be done on a separate clean build as position
independent code (PIC) is required for it.  Trying to run it after
building the static library or the server will result in a link error.

```
make clean
```
Clean up files generated during the build

## Setting build options

Make options can be set on the command line with the make command like so.
```
make build
```


| Make Options              | Description                              |
| ------------------------- | ---------------------------------------- |
| CRYPTO_LIB=libcrypto.so.0 | system versioned CRYPTO library          |
| SSL_LIB=libssl.so.0       | use versioned SSL library                |
| PREFIX=/usr/local         | sets the install directory               |
| COPT='-DNO_SSL'           | method to insert compile flags           |

Note that the WITH_* options used for *make* are not identical to the
preprocessor defines in the source code - usually USE_* is used there.

## Setting compile flags

Compile flags can be set using the *COPT* make option like so.
```
make build COPT="-DNDEBUG"
```

| Compile Flags             | Description                          |
| ------------------------- | ------------------------------------ |
| NDEBUG                    | strip off all debug code             |
| DEBUG                     | build debug version (very noisy)     |
| NO_SSL                    | disable SSL functionality            |
| NO_SSL_DL                 | link against system libssl library   |
| SSL_ALREADY_INITIALIZED   | do not initialize libcrypto          |

## Cross Compiling

Take total control with *CC*, *COPT* and *TARGET_OS* as make options.
TARGET_OS is used to determine some compile details as will as code function.
TARGET_OS values should be be one found in *resources/Makefile.in-os*.

```
make CC=arm-none-linux-gnueabi-gcc COPT="-march=armv7-a -mfpu=vfp -mfloat-abi=softfp" TARGET_OS=FROG
```
