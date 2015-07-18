/* Copyright (c) 2015 the Civetweb developers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * We include the source file so that we have access to the internal private
 * static functions
 */
#ifdef _MSC_VER
#define CIVETWEB_API static
#endif
#include "../src/civetweb.c"

#include <stdlib.h>

#include "private.h"

/* This unit test file uses the excellent Check unit testing library.
 * The API documentation is available here:
 * http://check.sourceforge.net/doc/check_html/index.html
 */

START_TEST (test_parse_http_message)
{
  struct mg_request_info ri;
  char empty[] = "";
  char req1[] = "GET / HTTP/1.1\r\n\r\n";
  char req2[] = "BLAH / HTTP/1.1\r\n\r\n";
  char req3[] = "GET / HTTP/1.1\r\nBah\r\n";
  char req4[] = "GET / HTTP/1.1\r\nA: foo bar\r\nB: bar\r\nbaz\r\n\r\n";
  char req5[] = "GET / HTTP/1.1\r\n\r\n";
  char req6[] = "G";
  char req7[] = " blah ";
  char req8[] = " HTTP/1.1 200 OK \n\n";
  char req9[] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";

  ck_assert_int_eq(sizeof(req9) - 1, parse_http_message(req9, sizeof(req9), &ri));
  ck_assert_int_eq(1, ri.num_headers);

  ck_assert_int_eq(sizeof(req1) - 1, parse_http_message(req1, sizeof(req1), &ri));
  ck_assert_str_eq("1.1", ri.http_version);
  ck_assert_int_eq(0, ri.num_headers);

  ck_assert_int_eq(-1, parse_http_message(req2, sizeof(req2), &ri));
  ck_assert_int_eq(0, parse_http_message(req3, sizeof(req3), &ri));
  ck_assert_int_eq(0, parse_http_message(req6, sizeof(req6), &ri));
  ck_assert_int_eq(0, parse_http_message(req7, sizeof(req7), &ri));
  ck_assert_int_eq(0, parse_http_message(empty, 0, &ri));
  ck_assert_int_eq(sizeof(req8) - 1, parse_http_message(req8, sizeof(req8), &ri));

  /* TODO(lsm): Fix this. Header value may span multiple lines. */
  ck_assert_int_eq(sizeof(req4) - 1, parse_http_message(req4, sizeof(req4), &ri));
  ck_assert_str_eq("1.1", ri.http_version);
  ck_assert_int_eq(3, ri.num_headers);
  ck_assert_str_eq("A", ri.http_headers[0].name);
  ck_assert_str_eq("foo bar", ri.http_headers[0].value);
  ck_assert_str_eq("B", ri.http_headers[1].name);
  ck_assert_str_eq("bar", ri.http_headers[1].value);
  ck_assert_str_eq("baz\r\n\r", ri.http_headers[2].name);
  ck_assert_str_eq("", ri.http_headers[2].value);

  ck_assert_int_eq(sizeof(req5) - 1, parse_http_message(req5, sizeof(req5), &ri));
  ck_assert_str_eq("GET", ri.request_method);
  ck_assert_str_eq("1.1", ri.http_version);
}
END_TEST


START_TEST (test_match_prefix)
{
  ck_assert_int_eq(4, match_prefix("/api", 4, "/api"));
  ck_assert_int_eq(3, match_prefix("/a/", 3, "/a/b/c"));
  ck_assert_int_eq(-1, match_prefix("/a/", 3, "/ab/c"));
  ck_assert_int_eq(4, match_prefix("/*/", 3, "/ab/c"));
  ck_assert_int_eq(6, match_prefix("**", 2, "/a/b/c"));
  ck_assert_int_eq(2, match_prefix("/*", 2, "/a/b/c"));
  ck_assert_int_eq(2, match_prefix("*/*", 3, "/a/b/c"));
  ck_assert_int_eq(5, match_prefix("**/", 3, "/a/b/c"));
  ck_assert_int_eq(5, match_prefix("**.foo|**.bar", 13, "a.bar"));
  ck_assert_int_eq(2, match_prefix("a|b|cd", 6, "cdef"));
  ck_assert_int_eq(2, match_prefix("a|b|c?", 6, "cdef"));
  ck_assert_int_eq(1, match_prefix("a|?|cd", 6, "cdef"));
  ck_assert_int_eq(-1, match_prefix("/a/**.cgi", 9, "/foo/bar/x.cgi"));
  ck_assert_int_eq(12, match_prefix("/a/**.cgi", 9, "/a/bar/x.cgi"));
  ck_assert_int_eq(5, match_prefix("**/", 3, "/a/b/c"));
  ck_assert_int_eq(-1, match_prefix("**/$", 4, "/a/b/c"));
  ck_assert_int_eq(5, match_prefix("**/$", 4, "/a/b/"));
  ck_assert_int_eq(0, match_prefix("$", 1, ""));
  ck_assert_int_eq(-1, match_prefix("$", 1, "x"));
  ck_assert_int_eq(1, match_prefix("*$", 2, "x"));
  ck_assert_int_eq(1, match_prefix("/$", 2, "/"));
  ck_assert_int_eq(-1, match_prefix("**/$", 4, "/a/b/c"));
  ck_assert_int_eq(5, match_prefix("**/$", 4, "/a/b/"));
  ck_assert_int_eq(0, match_prefix("*", 1, "/hello/"));
  ck_assert_int_eq(-1, match_prefix("**.a$|**.b$", 11, "/a/b.b/"));
  ck_assert_int_eq(6, match_prefix("**.a$|**.b$", 11, "/a/b.b"));
  ck_assert_int_eq(6, match_prefix("**.a$|**.b$", 11, "/a/B.A"));
  ck_assert_int_eq(5, match_prefix("**o$", 4, "HELLO"));
}
END_TEST


START_TEST (test_remove_double_dots_and_double_slashes)
{
  struct {
    char before[20], after[20];
  } data[] = {
      {"////a", "/a"},
      {"/.....", "/."},
      {"/......", "/"},
      {"...", "..."},
      {"/...///", "/./"},
      {"/a...///", "/a.../"},
      {"/.x", "/.x"},
      {"/\\", "/"},
      {"/a\\", "/a\\"},
      {"/a\\\\...", "/a\\."},
  };
  size_t i;

  for (i = 0; i < ARRAY_SIZE(data); i++) {
    remove_double_dots_and_double_slashes(data[i].before);
    ck_assert_str_eq(data[i].before, data[i].after);
  }
}
END_TEST


START_TEST (test_is_valid_uri)
{
  ck_assert_int_eq(1, is_valid_uri("/api"));
  ck_assert_int_eq(0, is_valid_uri("api"));
  ck_assert_int_eq(1, is_valid_uri("*"));
  ck_assert_int_eq(0, is_valid_uri("*xy"));
}
END_TEST


Suite * make_private_suite (void) {

  Suite * const suite = suite_create("Private");
  TCase * const http_message = tcase_create("HTTP Message");
  TCase * const url_parsing = tcase_create("URL Parsing");

  tcase_add_test(http_message, test_parse_http_message);
  suite_add_tcase(suite, http_message);

  tcase_add_test(url_parsing, test_match_prefix);
  tcase_add_test(url_parsing, test_remove_double_dots_and_double_slashes);
  tcase_add_test(url_parsing, test_is_valid_uri);
  suite_add_tcase(suite, url_parsing);

  return suite;
}
