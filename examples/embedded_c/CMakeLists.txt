add_executable(embedded_c)

set_target_properties(
    embedded_c PROPERTIES
    C_EXTENSIONS ON
    C_STANDARD 11
    C_STANDARD_REQUIRED ON)

target_compile_options(
    embedded_c PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wcast-align -Wcast-qual -Wconversion -Werror -Wextra -Wformat-security -Winit-self -Wnested-externs -Wno-unused-parameter -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -funsigned-char>)
target_link_libraries(
    embedded_c PRIVATE
    httplib)
target_sources(
    embedded_c PRIVATE
	embedded_c.c)
