cmake_minimum_required(VERSION 3.24)

project(httplib LANGUAGES C)

# (Old notes from the Makefile that should be preserved until the CMake build improves.)
#
# If statically linking SSL you might need to:
#
#   * set the include path for the lib — e.g., `-I/opt/local/include/openssl-1.0/`
#   * set the library path for the bin — e.g., `-L/opt/local/lib/openssl-1.0/`
#
# Compiling with `-DNO_SSL_DL` doesn't currently work, and dynamically loaded libs
# take precedence over statically linked libs; on Mac you may need to:
#
#   * set the dynamic load path for the bin — e.g., `DYLD_LIBRARY_PATH=/opt/local/lib/openssl-1.0/ ./embedded_c`
#
# If you do those things SSL works, but given those things the default is `-DNO_SSL`.

add_subdirectory(examples/embedded_c)
add_subdirectory(src)
