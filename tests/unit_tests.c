#include <stdbool.h>

#include "greatest.h"

#include "libhttp.h"
#include "httplib_main.h"
#include "httplib_utils.h"


TEST
test_httplib_base64_encode(void) {
	char buf[128];

	const char *alpha = "abcdefghijklmnopqrstuvwxyz";
	const char *nonalpha = " !\"#$%&'()*+,-./0123456789:;<=>?@";

	const char *alpha_b64_enc = "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo=";
	const char *nonalpha_b64_enc = "ICEiIyQlJicoKSorLC0uLzAxMjM0NTY3ODk6Ozw9Pj9A";

	memset(buf, 77, sizeof(buf));
	httplib_base64_encode((unsigned char const *)"a", 1, buf, sizeof(buf));
	ASSERT_STR_EQ( "YQ==", buf );

	memset(buf, 77, sizeof(buf));
	httplib_base64_encode((unsigned char const *)"ab", 2, buf, sizeof(buf));
	ASSERT_STR_EQ( "YWI=", buf );

	memset(buf, 77, sizeof(buf));
	httplib_base64_encode((unsigned char const *)"abc", 3, buf, sizeof(buf));
	ASSERT_STR_EQ( "YWJj", buf );

	memset(buf, 77, sizeof(buf));
	httplib_base64_encode((unsigned char const *)"abcd", 4, buf, sizeof(buf));
	ASSERT_STR_EQ( "YWJjZA==", buf );

	memset(buf, 77, sizeof(buf));
	httplib_base64_encode((unsigned char const *)"abcd", 1, buf, sizeof(buf));
	ASSERT_STR_EQ( "YQ==", buf );

	memset(buf, 77, sizeof(buf));
	httplib_base64_encode((unsigned char const *)alpha, 3, buf, sizeof(buf));
	ASSERT_STR_EQ( "YWJj", buf );

	memset(buf, 77, sizeof(buf));
	httplib_base64_encode((unsigned char const *)alpha, 4, buf, sizeof(buf));
	ASSERT_STR_EQ( "YWJjZA==", buf );

	memset(buf, 77, sizeof(buf));
	httplib_base64_encode((unsigned char const *)alpha, 5, buf, sizeof(buf));
	ASSERT_STR_EQ( "YWJjZGU=", buf );

	memset(buf, 77, sizeof(buf));
	httplib_base64_encode((unsigned char const *)alpha, 6, buf, sizeof(buf));
	ASSERT_STR_EQ( "YWJjZGVm", buf );

	memset(buf, 77, sizeof(buf));
	httplib_base64_encode((unsigned char const *)alpha, (int)strlen(alpha), buf, sizeof(buf));
	ASSERT_STR_EQ( alpha_b64_enc, buf );

	memset(buf, 77, sizeof(buf));
	httplib_base64_encode((unsigned char const *)nonalpha, (int)strlen(nonalpha), buf, sizeof(buf));
	ASSERT_STR_EQ( nonalpha_b64_enc, buf );

	PASS();
}


TEST
test_httplib_url_decode(void) {
	char buf[128];
	const char *alpha = "abcdefghijklmnopqrstuvwxyz";
	const char *nonalpha = " !\"#$%&'()*+,-./0123456789:;<=>?@";
	const char *nonalpha_url_enc1 =
	    "%20%21%22%23$%25%26%27()%2a%2b,-.%2f0123456789%3a;%3c%3d%3e%3f%40";
	const char *nonalpha_url_enc2 =
	    "%20!%22%23%24%25%26'()*%2B%2C-.%2F0123456789%3A%3B%3C%3D%3E%3F%40";
	int ret;

	memset(buf, 77, sizeof(buf));
	ret = httplib_url_decode(alpha, (int)strlen(alpha), buf, sizeof(buf), 0);
	ASSERT_EQ(ret, (int)strlen(buf));
	ASSERT_EQ(ret, (int)strlen(alpha));
	ASSERT_STR_EQ(buf, alpha);

	memset(buf, 77, sizeof(buf));
	ret = httplib_url_decode(nonalpha_url_enc1, (int)strlen(nonalpha_url_enc1), buf, sizeof(buf), 0);
	ASSERT_EQ( (int)strlen(buf), ret );
	ASSERT_EQ( (int)strlen(nonalpha), ret );
	ASSERT_STR_EQ( nonalpha, buf );

	memset(buf, 77, sizeof(buf));
	ret = httplib_url_decode(nonalpha_url_enc2, (int)strlen(nonalpha_url_enc2), buf, sizeof(buf), 0);
	ASSERT_EQ( (int)strlen(buf), ret );
	ASSERT_EQ( (int)strlen(nonalpha), ret );
	ASSERT_STR_EQ( nonalpha, buf );

	ASSERT_EQ( -1, httplib_url_decode("foo", 3, buf, 3, 0) );  // No space for \0
	ASSERT_EQ(  3, httplib_url_decode("foo", 3, buf, 4, 0) );
	ASSERT_STR_EQ( "foo", buf );

	ASSERT_EQ( 2, httplib_url_decode("a+", 2, buf, sizeof(buf), 0) );
	ASSERT_STR_EQ( "a+", buf );

	ASSERT_EQ( 2, httplib_url_decode("a+", 2, buf, sizeof(buf), 1) );
	ASSERT_STR_EQ( "a ", buf );

	ASSERT_EQ( 1, httplib_url_decode("%61", 1, buf, sizeof(buf), 1) );
	ASSERT_STR_EQ( "%", buf );

	ASSERT_EQ( 2, httplib_url_decode("%61", 2, buf, sizeof(buf), 1) );
	ASSERT_STR_EQ( "%6", buf );

	ASSERT_EQ( 1, httplib_url_decode("%61", 3, buf, sizeof(buf), 1) );
	ASSERT_STR_EQ( "a", buf );

	PASS();
}


TEST
test_httplib_url_encode(void) {
	char buf[128];
	const char *alpha = "abcdefghijklmnopqrstuvwxyz";
	const char *nonalpha = " !\"#$%&'()*+,-./0123456789:;<=>?@";
	const char *nonalpha_url_enc1 =
	    "%20%21%22%23$%25%26%27()%2a%2b,-.%2f0123456789%3a;%3c%3d%3e%3f%40";
	int ret;

	memset(buf, 77, sizeof(buf));
	ret = httplib_url_encode(alpha, buf, sizeof(buf));
	ASSERT_EQ( (int)strlen(buf), ret );
	ASSERT_EQ( (int)strlen(alpha), ret );
	ASSERT_STR_EQ( alpha, buf );

	memset(buf, 77, sizeof(buf));
	ret = httplib_url_encode(nonalpha, buf, sizeof(buf));
	ASSERT_EQ( (int)strlen(buf), ret );
	ASSERT_EQ( (int)strlen(nonalpha_url_enc1), ret );
	ASSERT_STR_EQ( nonalpha_url_enc1, buf );

	PASS();
}


TEST
test_XX_httplib_get_uri_type(void) {

	ASSERT_EQ( URI_TYPE_RELATIVE, XX_httplib_get_uri_type("/api") );
	ASSERT_EQ( URI_TYPE_RELATIVE, XX_httplib_get_uri_type("/api/") );
	ASSERT_EQ( URI_TYPE_RELATIVE, XX_httplib_get_uri_type("/some/long/path%20with%20space/file.xyz") );
	ASSERT_EQ( URI_TYPE_UNKNOWN, XX_httplib_get_uri_type("api") );
	ASSERT_EQ( URI_TYPE_ASTERISK, XX_httplib_get_uri_type("*") );
	ASSERT_EQ( URI_TYPE_UNKNOWN, XX_httplib_get_uri_type("*xy") );
	ASSERT_EQ( URI_TYPE_ABS_NOPORT, XX_httplib_get_uri_type("http://somewhere/") );
	ASSERT_EQ( URI_TYPE_ABS_NOPORT, XX_httplib_get_uri_type("https://somewhere/some/file.html") );
	ASSERT_EQ( URI_TYPE_ABS_PORT, XX_httplib_get_uri_type("http://somewhere:8080/") );
	ASSERT_EQ( URI_TYPE_ABS_PORT, XX_httplib_get_uri_type("https://somewhere:8080/some/file.html") );

	PASS();
}


TEST
test_XX_httplib_match_prefix(void) {
    	// ASSERT_GT( 0, XX_httplib_match_prefix("", 0, "") );

    	// ASSERT_GT( 0, XX_httplib_match_prefix("", 0, "x") );
    	ASSERT_GT( 0, XX_httplib_match_prefix("x", 1, "") );

    	ASSERT_LTE( 0, XX_httplib_match_prefix("x", 1, "x") );
    	ASSERT_LTE( 0, XX_httplib_match_prefix("X", 1, "X") );

	printf(" XX_httplib_match_prefix('x', 1, 'X') = %d\n", XX_httplib_match_prefix("x", 1, "X") );
    	ASSERT_LTE( 0, XX_httplib_match_prefix("x", 1, "X") );
    	ASSERT_LTE( 0, XX_httplib_match_prefix("X", 1, "x") );

    	ASSERT_GT( 0, XX_httplib_match_prefix("?", 1, "") );
    	ASSERT_LTE( 0, XX_httplib_match_prefix("?", 1, "x") );
    	ASSERT_GT( 0, XX_httplib_match_prefix("x?", 2, "x") );
    	ASSERT_LTE( 0, XX_httplib_match_prefix("x?", 2, "xx") );
    	ASSERT_LTE( 0, XX_httplib_match_prefix("x?", 2, "xxx") );
    	ASSERT_GT( 0, XX_httplib_match_prefix("?x", 2, "x") );
    	ASSERT_LTE( 0, XX_httplib_match_prefix("?x", 2, "xx") );
    	ASSERT_LTE( 0, XX_httplib_match_prefix("?x", 2, "xxx") );

    	ASSERT_LTE( 0, XX_httplib_match_prefix("$", 1, "") );

	// Adapted from unit_test.c
	// Copyright (c) 2013-2015 the Civetweb developers
	// Copyright (c) 2004-2013 Sergey Lyubka
	ASSERT_EQ(  4, XX_httplib_match_prefix("/api", 4, "/api") );
	ASSERT_EQ(  3, XX_httplib_match_prefix("/a/", 3, "/a/b/c") );
	ASSERT_EQ( -1, XX_httplib_match_prefix("/a/", 3, "/ab/c") );
	ASSERT_EQ(  4, XX_httplib_match_prefix("/*/", 3, "/ab/c") );
	ASSERT_EQ(  6, XX_httplib_match_prefix("**", 2, "/a/b/c") );
	ASSERT_EQ(  2, XX_httplib_match_prefix("/*", 2, "/a/b/c") );
	ASSERT_EQ(  2, XX_httplib_match_prefix("*/*", 3, "/a/b/c") );
	ASSERT_EQ(  5, XX_httplib_match_prefix("**/", 3, "/a/b/c") );
	ASSERT_EQ(  5, XX_httplib_match_prefix("**.foo|**.bar", 13, "a.bar") );
	ASSERT_EQ(  2, XX_httplib_match_prefix("a|b|cd", 6, "cdef") );
	ASSERT_EQ(  2, XX_httplib_match_prefix("a|b|c?", 6, "cdef") );
	ASSERT_EQ(  1, XX_httplib_match_prefix("a|?|cd", 6, "cdef") );
	ASSERT_EQ( -1, XX_httplib_match_prefix("/a/**.bar", 9, "/foo/bar/x.bar") );
	ASSERT_EQ( 12, XX_httplib_match_prefix("/a/**.bar", 9, "/a/bar/x.bar") );
	ASSERT_EQ(  5, XX_httplib_match_prefix("**/", 3, "/a/b/c") );
	ASSERT_EQ( -1, XX_httplib_match_prefix("**/$", 4, "/a/b/c") );
	ASSERT_EQ(  5, XX_httplib_match_prefix("**/$", 4, "/a/b/") );
	ASSERT_EQ(  0, XX_httplib_match_prefix("$", 1, "") );
	ASSERT_EQ( -1, XX_httplib_match_prefix("$", 1, "x") );
	ASSERT_EQ(  1, XX_httplib_match_prefix("*$", 2, "x") );
	ASSERT_EQ(  1, XX_httplib_match_prefix("/$", 2, "/") );
	ASSERT_EQ( -1, XX_httplib_match_prefix("**/$", 4, "/a/b/c") );
	ASSERT_EQ(  5, XX_httplib_match_prefix("**/$", 4, "/a/b/") );
	ASSERT_EQ(  0, XX_httplib_match_prefix("*", 1, "/hello/") );
	ASSERT_EQ( -1, XX_httplib_match_prefix("**.a$|**.b$", 11, "/a/b.b/") );
	ASSERT_EQ(  6, XX_httplib_match_prefix("**.a$|**.b$", 11, "/a/b.b") );
	ASSERT_EQ(  6, XX_httplib_match_prefix("**.a$|**.b$", 11, "/a/B.A") );
	ASSERT_EQ(  5, XX_httplib_match_prefix("**o$", 4, "HELLO") );

	PASS();
}


TEST
test_XX_httplib_next_option(void) {
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	const char *p, *list = "x/8,/y**=1;2k,z";
	struct vec a, b;
	int i;

	ASSERT(XX_httplib_next_option(NULL, &a, &b) == NULL);
	for (i = 0, p = list; (p = XX_httplib_next_option(p, &a, &b)) != NULL; i++) {
		ASSERT(i != 0 || (a.ptr == list && a.len == 3 && b.len == 0));
		ASSERT(i != 1 || (a.ptr == list + 4 && a.len == 4 && b.ptr == list + 9 && b.len == 4));
		ASSERT(i != 2 || (a.ptr == list + 14 && a.len == 1 && b.len == 0));
	}

	PASS();
}


TEST
test_XX_httplib_parse_date_string(void) {
	time_t now = time(0);
	struct tm *tm = gmtime(&now);
	char date[64] = {0};
	const char *month_names[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	ASSERT_EQ(  62, XX_httplib_parse_date_string("1/Jan/1970 00:01:02") );
	ASSERT_EQ( 123, XX_httplib_parse_date_string("1 Jan 1970 00:02:03") );
	ASSERT_EQ( 184, XX_httplib_parse_date_string("1-Jan-1970 00:03:04") );
	ASSERT_EQ( 245, XX_httplib_parse_date_string("Xyz, 1 Jan 1970 00:04:05") );

	XX_httplib_gmt_time_string(date, sizeof(date), &now);
	ASSERT_EQ( now, XX_httplib_parse_date_string(date) );

	sprintf(date,
	        "%02u %s %04u %02u:%02u:%02u",
	        tm->tm_mday,
	        month_names[tm->tm_mon],
	        tm->tm_year + 1900,
	        tm->tm_hour,
	        tm->tm_min,
	        tm->tm_sec);
	ASSERT_EQ( now, XX_httplib_parse_date_string(date) );

	XX_httplib_gmt_time_string(date, 1, NULL);
	ASSERT_STR_EQ( "", date );

	XX_httplib_gmt_time_string(date, 6, NULL);
	ASSERT_STR_EQ( "Thu, ", date ); /* part of "Thu, 01 Jan 1970 00:00:00 GMT" */

	XX_httplib_gmt_time_string(date, sizeof(date), NULL);
	ASSERT_STR_EQ( "Thu, 01 Jan 1970 00:00:00 GMT", date );

	for (unsigned long i = 2ul; i < 0x8000000ul; i += i / 2) {
		now = (time_t)i;

		XX_httplib_gmt_time_string(date, sizeof(date), &now);
		ASSERT_EQ( now, XX_httplib_parse_date_string(date) );

		tm = gmtime(&now);
		sprintf(date,
		        "%02u-%s-%04u %02u:%02u:%02u",
		        tm->tm_mday,
		        month_names[tm->tm_mon],
		        tm->tm_year + 1900,
		        tm->tm_hour,
		        tm->tm_min,
		        tm->tm_sec);
		ASSERT_EQ( now, XX_httplib_parse_date_string(date) );
	}

	PASS();
}


TEST
test_XX_httplib_parse_http_message(void) {
	// Adapted from unit_test.c
	// Copyright (c) 2013-2015 the Civetweb developers
	// Copyright (c) 2004-2013 Sergey Lyubka
	struct httplib_request_info ri;

	char empty[] = "";

	char req1[] = "GET / HTTP/1.1\r\n\r\n";
	char req2[] = "BLAH / HTTP/1.1\r\n\r\n";
	char req3[] = "GET / HTTP/1.1\r\nBah\r\n";
	char req4[] = "GET / HTTP/1.1\r\nA: foo bar\r\nB: bar\r\nskip\r\nbaz:\r\n\r\n";
	char req5[] = "GET / HTTP/1.1\r\n\r\n";
	char req6[] = "G";
	char req7[] = " blah ";
	char req8[] = " HTTP/1.1 200 OK \n\n";
	char req9[] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";
	char req10[] = "GET / HTTP/1.1\r\nA: foo bar\r\nB: bar\r\n\r\n";

	ASSERT_EQ( 0, XX_httplib_parse_http_message(empty, 0, &ri) );

	ASSERT_EQ( sizeof(req1) - 1, XX_httplib_parse_http_message(req1, sizeof(req1), &ri) );
	ASSERT_STR_EQ( "1.1", ri.http_version );
	ASSERT_EQ( 0, ri.num_headers );

	ASSERT_EQ( -1, XX_httplib_parse_http_message(req2, sizeof(req2), &ri) );

	ASSERT_EQ( 0, XX_httplib_parse_http_message(req3, sizeof(req3), &ri) );

	/* Multiline header are obsolete, so return an error
	 * (https://tools.ietf.org/html/rfc7230#section-3.2.4). */
	ASSERT_EQ( -1, XX_httplib_parse_http_message(req4, sizeof(req4), &ri) );

	ASSERT_EQ( sizeof(req5) - 1, XX_httplib_parse_http_message(req5, sizeof(req5), &ri) );
	ASSERT_STR_EQ( "GET", ri.request_method );
	ASSERT_STR_EQ( "1.1", ri.http_version );

	ASSERT_EQ( 0, XX_httplib_parse_http_message(req6, sizeof(req6), &ri) );

	ASSERT_EQ( 0, XX_httplib_parse_http_message(req7, sizeof(req7), &ri) );

	ASSERT_EQ( sizeof(req8) - 1, XX_httplib_parse_http_message(req8, sizeof(req8), &ri) );

	ASSERT_EQ( sizeof(req9) - 1, XX_httplib_parse_http_message(req9, sizeof(req9), &ri) );
	ASSERT_EQ( 1, ri.num_headers );

	ASSERT_EQ( sizeof(req10) - 1, XX_httplib_parse_http_message(req10, sizeof(req10), &ri) );
	ASSERT_STR_EQ( "1.1", ri.http_version );
	ASSERT_EQ( 2, ri.num_headers );
	ASSERT_STR_EQ( "A", ri.http_headers[0].name );
	ASSERT_STR_EQ( "foo bar", ri.http_headers[0].value );
	ASSERT_STR_EQ( "B", ri.http_headers[1].name );
	ASSERT_STR_EQ( "bar", ri.http_headers[1].value );

	PASS();
}


TEST
test_XX_httplib_parse_port_string(void) {
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	static const char *valid[] = {
		"0",
		"1",
		"1s",
		"1r",
		"1.2.3.4:1",
		"1.2.3.4:1s",
		"1.2.3.4:1r",
		"[::1]:123",
		"[::]:80",
		"[3ffe:2a00:100:7031::1]:900",
		"+80",
		NULL
	};
	for (int i = 0; valid[i] != NULL; i++) {
		struct vec vec = {.ptr = valid[i], .len = strlen(valid[i]) };
		struct socket so = {0};
		int ip_family = 123;

		ASSERT_NEQ( 0, XX_httplib_parse_port_string(&vec, &so, &ip_family) );

		if (i < 7) {
			ASSERT_EQ( 4, ip_family );
		} else if (i < 10) {
			ASSERT_EQ( 6, ip_family );
		} else {
			ASSERT_EQ( 4 + 6, ip_family );
		}
	}

	static const char *invalid[] = {
	    	"99999", "1k", "1.2.3", "1.2.3.4:", "1.2.3.4:2p", NULL
	};
	for (int i = 0; invalid[i] != NULL; i++) {
		struct vec vec = {.ptr = invalid[i], .len = strlen(invalid[i]) };
		struct socket so = {0};
		int ip_family = 123;

		ASSERT_EQ( 0, XX_httplib_parse_port_string(&vec, &so, &ip_family) );
		ASSERT_EQ( 0, ip_family );
	}

	PASS();
}


TEST
test_XX_httplib_remove_double_dots_and_double_slashes(void) {
	// Adapted from unit_test.c
	// Copyright (c) 2013-2015 the Civetweb developers
	// Copyright (c) 2004-2013 Sergey Lyubka
	struct {
		char before[20], after[20];
	} data[] = {
	    {"////a", "/a"},
	    {"/.....", "/."},
	    {"/......", "/"},
	    {"..", "."},
	    {"...", "."},
	    {"/...///", "/./"},
	    {"/a...///", "/a.../"},
	    {"/.x", "/.x"},
	    {"/\\", "/"},
	    {"/a\\", "/a\\"},
	    {"/a\\\\...", "/a\\."},
	};

	for (size_t i = 0; i < ARRAY_SIZE(data); i++) {
		XX_httplib_remove_double_dots_and_double_slashes(data[i].before);
		ASSERT_STR_EQ( data[i].after, data[i].before );
	}

	PASS();
}

TEST
test_XX_httplib_should_keep_alive(void) {
	// Adapted from unit_test.c
	// Copyright (c) 2013-2015 the Civetweb developers
	// Copyright (c) 2004-2013 Sergey Lyubka
	struct httplib_connection conn = {};
	struct httplib_context ctx = {};

	char req1[] = "GET / HTTP/1.1\r\n\r\n";
	char req2[] = "GET / HTTP/1.0\r\n\r\n";
	char req3[] = "GET / HTTP/1.1\r\nConnection: close\r\n\r\n";
	char req4[] = "GET / HTTP/1.1\r\nConnection: keep-alive\r\n\r\n";

	ASSERT_EQ( sizeof(req1) - 1, XX_httplib_parse_http_message(req1, sizeof(req1), &conn.request_info) );

	ctx.enable_keep_alive = false;
	ASSERT_EQ( 0, XX_httplib_should_keep_alive(&ctx, &conn) );

	ctx.enable_keep_alive = true;
	ASSERT_EQ( 1, XX_httplib_should_keep_alive(&ctx, &conn) );

	conn.must_close = 1;
	ASSERT_EQ( 0, XX_httplib_should_keep_alive(&ctx, &conn) );

	conn.must_close = 0;
	XX_httplib_parse_http_message(req2, sizeof(req2), &conn.request_info);
	ASSERT_EQ( 0, XX_httplib_should_keep_alive(&ctx, &conn) );

	XX_httplib_parse_http_message(req3, sizeof(req3), &conn.request_info);
	ASSERT_EQ( 0, XX_httplib_should_keep_alive(&ctx, &conn) );

	XX_httplib_parse_http_message(req4, sizeof(req4), &conn.request_info);
	ASSERT_EQ( 1, XX_httplib_should_keep_alive(&ctx, &conn) );

	conn.status_code = 401;
	ASSERT_EQ( 0, XX_httplib_should_keep_alive(&ctx, &conn) );

	conn.status_code = 200;
	conn.must_close = 1;
	ASSERT_EQ( 0, XX_httplib_should_keep_alive(&ctx, &conn) );

	PASS();
}


TEST
test_XX_httplib_skip_quoted(void) {
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	char x[] = "a=1, b=2, c='hi \' there', d='here\\, there'", *s = x, *p;

	p = XX_httplib_skip_quoted(&s, ", ", ", ", 0);
	ASSERT(p != NULL && !strcmp(p, "a=1"));

	p = XX_httplib_skip_quoted(&s, ", ", ", ", 0);
	ASSERT(p != NULL && !strcmp(p, "b=2"));

	p = XX_httplib_skip_quoted(&s, ",", " ", 0);
	ASSERT(p != NULL && !strcmp(p, "c='hi \' there'"));

	p = XX_httplib_skip_quoted(&s, ",", " ", '\\');
	ASSERT(p != NULL && !strcmp(p, "d='here, there'"));
	ASSERT(*s == 0);

	PASS();
}


TEST
test_httplib_strcasestr(void) {
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	static char const *big1 = "abcdef";

	ASSERT( httplib_strcasestr("Y", "X") == NULL );
	ASSERT( httplib_strcasestr("Y", "y") != NULL );
	ASSERT( httplib_strcasestr(big1, "X") == NULL );
	ASSERT( httplib_strcasestr(big1, "CD") == big1 + 2 );
	ASSERT( httplib_strcasestr("aa", "AAB") == NULL );

	PASS();
}



GREATEST_MAIN_DEFS();

int
main(int argc, char **argv) {
	GREATEST_MAIN_BEGIN();

#if defined(_WIN32)
	/* test_XX_httplib_parse_net requires WSAStartup for IPv6 */
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

	RUN_TEST(test_httplib_base64_encode);
	RUN_TEST(test_httplib_url_decode);
	RUN_TEST(test_httplib_url_encode);
	RUN_TEST(test_XX_httplib_get_uri_type);
	RUN_TEST(test_XX_httplib_match_prefix);
	RUN_TEST(test_XX_httplib_next_option);
	RUN_TEST(test_XX_httplib_parse_date_string);
	RUN_TEST(test_XX_httplib_parse_http_message);
	RUN_TEST(test_XX_httplib_parse_port_string);
	RUN_TEST(test_XX_httplib_remove_double_dots_and_double_slashes);
	RUN_TEST(test_XX_httplib_should_keep_alive);
	RUN_TEST(test_XX_httplib_skip_quoted);
	RUN_TEST(test_httplib_strcasestr);

	GREATEST_MAIN_END();
}
