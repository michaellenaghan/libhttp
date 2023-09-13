cmake_minimum_required(VERSION 3.24)

project(httplib LANGUAGES C)

enable_testing()

option(HTTPLIB_ENABLE_SSL "Enable SSL" ON)

if (HTTPLIB_ENABLE_SSL)
    set(OPENSSL_ROOT_DIR /opt/local/libexec/openssl11/)
    set(OPENSSL_USE_STATIC_LIBS ON)

    find_package(OpenSSL 1.1 REQUIRED)

    message(STATUS "httplib: OPENSSL_VERSION = ${OPENSSL_VERSION}")
    message(STATUS "httplib: OPENSSL_INCLUDE_DIR = ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "httplib: OPENSSL_LIBRARIES = ${OPENSSL_LIBRARIES}")
endif()

if (HTTPLIB_ENABLE_SSL)
    set(ZLIB_USE_STATIC_LIBS ON)

    find_package(ZLIB REQUIRED)  # Required for statically linking OpenSSL

    message(STATUS "httplib: ZLIB_VERSION = ${ZLIB_VERSION}")
    message(STATUS "httplib: ZLIB_INCLUDE_DIRS = ${ZLIB_INCLUDE_DIRS}")
    message(STATUS "httplib: ZLIB_LIBRARIES = ${ZLIB_LIBRARIES}")
endif()

add_subdirectory(examples/embedded_c)
add_subdirectory(src)
add_subdirectory(tests)
