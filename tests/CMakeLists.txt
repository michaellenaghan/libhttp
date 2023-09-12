enable_testing()

add_executable(unit_tests)
set_target_properties(
    unit_tests PROPERTIES
    C_EXTENSIONS ON
    C_STANDARD 11
    C_STANDARD_REQUIRED ON)
target_compile_options(
    unit_tests PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wcast-align -Wcast-qual -Wconversion -Werror -Wextra -Wformat-security -Winit-self -Wnested-externs -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings -funsigned-char>)
target_include_directories(
    unit_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(
    unit_tests PRIVATE
    httplib)
target_sources(
    unit_tests PRIVATE
	unit_tests.c)

add_test(NAME unit_tests COMMAND unit_tests)

