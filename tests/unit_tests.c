#include <stdbool.h>

#include "greatest.h"
#include "greatest_ex.h"

#include "libhttp.h"
#include "httplib_main.h"
#include "httplib_utils.h"


TEST
test_httplib_base64_decode(void) {
	char buf[128];
	size_t buf_len = sizeof(buf);

	const char *alpha = "abcdefghijklmnopqrstuvwxyz";
	const char *nonalpha = " !\"#$%&'()*+,-./0123456789:;<=>?@";

	const char *alpha_b64_enc = "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo=";
	const char *nonalpha_b64_enc = "ICEiIyQlJicoKSorLC0uLzAxMjM0NTY3ODk6Ozw9Pj9A";

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_decode((unsigned char const *)"YQ==", 4, buf, &buf_len);
	ASSERT_STRN_EQ( "a", buf, 1 );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_decode((unsigned char const *)"YWI=", 4, buf, &buf_len);
	ASSERT_STRN_EQ( "ab", buf, 2 );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_decode((unsigned char const *)"YWJj", 4, buf, &buf_len);
	ASSERT_STRN_EQ( "abc", buf, 3 );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_decode((unsigned char const *)"YWJjZA==", 8, buf, &buf_len);
	ASSERT_STRN_EQ( "abcd", buf, 4 );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_decode((unsigned char const *)"YQ==", 4, buf, &buf_len);
	ASSERT_STRN_EQ( "abcd", buf, 1 );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_decode((unsigned char const *)"YWJj", 4, buf, &buf_len);
	ASSERT_STRN_EQ( alpha, buf, 3 );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_decode((unsigned char const *)"YWJjZA==", 8, buf, &buf_len);
	ASSERT_STRN_EQ( alpha, buf, 4 );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_decode((unsigned char const *)"YWJjZGU=", 8, buf, &buf_len);
	ASSERT_STRN_EQ( alpha, buf, 5 );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_decode((unsigned char const *)"YWJjZGVm", 8, buf, &buf_len);
	ASSERT_STRN_EQ( alpha, buf, 6 );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_decode((unsigned char const *)alpha_b64_enc, strlen(alpha_b64_enc), buf, &buf_len);
	ASSERT_STR_EQ( alpha, buf );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_decode((unsigned char const *)nonalpha_b64_enc, strlen(nonalpha_b64_enc), buf, &buf_len);
	ASSERT_STR_EQ( nonalpha, buf );

	PASS();
}


TEST
test_httplib_base64_encode(void) {
	char buf[128];
	size_t buf_len = sizeof(buf);

	const char *alpha = "abcdefghijklmnopqrstuvwxyz";
	const char *nonalpha = " !\"#$%&'()*+,-./0123456789:;<=>?@";

	const char *alpha_b64_enc = "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo=";
	const char *nonalpha_b64_enc = "ICEiIyQlJicoKSorLC0uLzAxMjM0NTY3ODk6Ozw9Pj9A";

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_encode((unsigned char const *)"a", 1, buf, &buf_len);
	ASSERT_STR_EQ( "YQ==", buf );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_encode((unsigned char const *)"ab", 2, buf, &buf_len);
	ASSERT_STR_EQ( "YWI=", buf );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_encode((unsigned char const *)"abc", 3, buf, &buf_len);
	ASSERT_STR_EQ( "YWJj", buf );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_encode((unsigned char const *)"abcd", 4, buf, &buf_len);
	ASSERT_STR_EQ( "YWJjZA==", buf );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_encode((unsigned char const *)"abcd", 1, buf, &buf_len);
	ASSERT_STR_EQ( "YQ==", buf );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_encode((unsigned char const *)alpha, 3, buf, &buf_len);
	ASSERT_STR_EQ( "YWJj", buf );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_encode((unsigned char const *)alpha, 4, buf, &buf_len);
	ASSERT_STR_EQ( "YWJjZA==", buf );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_encode((unsigned char const *)alpha, 5, buf, &buf_len);
	ASSERT_STR_EQ( "YWJjZGU=", buf );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_encode((unsigned char const *)alpha, 6, buf, &buf_len);
	ASSERT_STR_EQ( "YWJjZGVm", buf );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_encode((unsigned char const *)alpha, strlen(alpha), buf, &buf_len);
	ASSERT_STR_EQ( alpha_b64_enc, buf );

	memset(buf, 77, sizeof(buf));
	buf_len = sizeof(buf);
	httplib_base64_encode((unsigned char const *)nonalpha, strlen(nonalpha), buf, &buf_len);
	ASSERT_STR_EQ( nonalpha_b64_enc, buf );

	PASS();
}


TEST
test_httplib_check_feature(void) {
#ifdef NO_SSL
	ASSERT_FALSE( httplib_check_feature(FEATURE_SSL) );
#else
	ASSERT( httplib_check_feature(FEATURE_SSL) );
#endif
	PASS();
}


TEST
test_httplib_get_builtin_mime_type(void) {
	ASSERT_STR_EQ( "text/plain", httplib_get_builtin_mime_type("x.txt") );
	ASSERT_STR_EQ( "text/html", httplib_get_builtin_mime_type("x.html") );
	ASSERT_STR_EQ( "text/html", httplib_get_builtin_mime_type("x.HTML") );
	ASSERT_STR_EQ( "text/html", httplib_get_builtin_mime_type("x.hTmL") );
	ASSERT_STR_EQ( "text/html", httplib_get_builtin_mime_type("/abc/def/ghi.htm") );
	ASSERT_STR_EQ( "text/plain", httplib_get_builtin_mime_type("x.unknown_extention_xyz") );

	PASS();
}


TEST
test_httplib_get_cookie(void) {
	char buf[20];

	ASSERT_EQ( -2, httplib_get_cookie("", "foo", NULL, sizeof(buf)) );
	ASSERT_EQ( -2, httplib_get_cookie("", "foo", buf, 0) );
	ASSERT_EQ( -1, httplib_get_cookie("", "foo", buf, sizeof(buf)) );
	ASSERT_EQ( -1, httplib_get_cookie("", NULL, buf, sizeof(buf)) );

	ASSERT_EQ( 1, httplib_get_cookie("a=1; b=2; c; d", "a", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "1", buf );

	ASSERT_EQ( 1, httplib_get_cookie("a=1; b=2; c; d", "b", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "2", buf );

	ASSERT_EQ( 3, httplib_get_cookie("a=1; b=123", "b", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "123", buf );

	ASSERT_EQ( -1, httplib_get_cookie("a=1; b=2; c; d", "c", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "", buf );

	const char *longcookie = "key1=1; key2=2; key3; key4=4; key5=; key6; "
	                         "key7=this+is+it; key8=8; key9";

	/* invalid result buffer */
	ASSERT_EQ( -2, httplib_get_cookie("", "notfound", NULL, 999) );

	/* zero size result buffer */
	ASSERT_EQ( -2, httplib_get_cookie("", "notfound", buf, 0) );

	/* too small result buffer */
	ASSERT_EQ( -3, httplib_get_cookie("key=toooooooooolong", "key", buf, 4) );

	/* key not found in string */
	ASSERT_EQ( -1, httplib_get_cookie("", "notfound", buf, sizeof(buf)) );

	ASSERT_EQ( -1, httplib_get_cookie(longcookie, "notfound", buf, sizeof(buf)) );

	/* key not found in string */
	ASSERT_EQ( -1, httplib_get_cookie("key1=1; key2=2; key3=3", "notfound", buf, sizeof(buf)) );

	/* keys are found as first, middle and last key */
	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 1, httplib_get_cookie("key1=1; key2=2; key3=3", "key1", buf, sizeof(buf)) );
	ASSERT_STR_EQ("1", buf);

	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 1, httplib_get_cookie("key1=1; key2=2; key3=3", "key2", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "2", buf );

	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 1, httplib_get_cookie("key1=1; key2=2; key3=3", "key3", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "3", buf );

	/* longer value in the middle of a longer string */
	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 10, httplib_get_cookie(longcookie, "key7", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "this+is+it", buf );

	/* key with = but without value in the middle of a longer string */
	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 0, httplib_get_cookie(longcookie, "key5", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "", buf );

	/* key without = and without value in the middle of a longer string */
	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( -1, httplib_get_cookie(longcookie, "key6", buf, sizeof(buf)) );

	PASS();
}


TEST
test_httplib_get_response_code_text(void) {
	for (int i = 100; i < 600; i++) {
		const char *resp = httplib_get_response_code_text(NULL, NULL, i);
		ASSERT_NEQ( NULL, resp );
		size_t len = strlen(resp);
		ASSERT_GT_EX( 1, len );
		ASSERT_LT_EX( 32, len );
		for (size_t j = 0; j < len; j++) {
			if (resp[j] == ' ') {
				/* space is valid */
			} else if (resp[j] == '-') {
				/* hyphen is valid */
			} else if (resp[j] >= 'A' && resp[j] <= 'Z') {
				/* A-Z is valid */
			} else if (resp[j] >= 'a' && resp[j] <= 'z') {
				/* a-z is valid */
			} else {
				char msg[256];
				snprintf(
					msg, sizeof(msg),
					"Found letter %c (%02xh) in %s",
				             resp[j],
				             resp[j],
				             resp);
				FAILm(msg);
			}
		}
	}

	PASS();
}


TEST
test_httplib_get_var(void) {
	char buf[32];

	static const char *post[] = {
		"a=1&&b=2&d&=&c=3%20&e=",
	        "q=&st=2012%2F11%2F13+17%3A05&et=&team_id=",
		NULL
	};

	ASSERT_EQ( 1, httplib_get_var(post[0], strlen(post[0]), "a", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "1", buf );

	ASSERT_EQ( 1, httplib_get_var(post[0], strlen(post[0]), "b", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "2", buf );

	ASSERT_EQ( 2, httplib_get_var(post[0], strlen(post[0]), "c", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "3 ", buf );

	ASSERT_EQ( 0, httplib_get_var(post[0], strlen(post[0]), "e", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "", buf );

	ASSERT_EQ( -1, httplib_get_var(post[0], strlen(post[0]), "d", buf, sizeof(buf)) );
	ASSERT_EQ( -2, httplib_get_var(post[0], strlen(post[0]), "c", buf, 2) );

	ASSERT_EQ( -2, httplib_get_var(post[0], strlen(post[0]), "x", NULL, 10) );
	ASSERT_EQ( -2, httplib_get_var(post[0], strlen(post[0]), "x", buf, 0) );
	ASSERT_EQ( -2, httplib_get_var(post[1], strlen(post[1]), "st", buf, 16) );
	ASSERT_EQ( 16, httplib_get_var(post[1], strlen(post[1]), "st", buf, 17) );

	const char *shortquery = "key1=1&key2=2&key3=3";
	const char *longquery = "key1=1&key2=2&key3&key4=4&key5=&key6&"
	                        "key7=this+is+it&key8=8&key9&&key10=&&"
	                        "key7=that+is+it&key12=12";

	/* invalid result buffer */
	ASSERT_EQ( -2, httplib_get_var2("", 0, "notfound", NULL, 999, 0) );

	/* zero size result buffer */
	ASSERT_EQ( -2, httplib_get_var2("", 0, "notfound", buf, 0, 0) );

	/* too small result buffer */
	ASSERT_EQ( -2, httplib_get_var2("key=toooooooooolong", 19, "key", buf, 4, 0) );

	/* key not found in string */
	ASSERT_EQ( -1, httplib_get_var2("", 0, "notfound", buf, sizeof(buf), 0) );

	ASSERT_EQ( -1, httplib_get_var2( longquery, strlen(longquery), "notfound", buf, sizeof(buf), 0) );

	/* key not found in string */
	ASSERT_EQ( -1, httplib_get_var2(shortquery, strlen(shortquery), "notfound", buf, sizeof(buf), 0) );

	/* key not found in string */
	ASSERT_EQ( -1, httplib_get_var2("key1=1&key2=2&key3=3&notfound=here", strlen(shortquery), "notfound", buf, sizeof(buf), 0) );

	/* key not found in string */
	ASSERT_EQ( -1, httplib_get_var2(shortquery, strlen(shortquery), "key1", buf, sizeof(buf), 1) );

	/* keys are found as first, middle and last key */
	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 1, httplib_get_var2(shortquery, strlen(shortquery), "key1", buf, sizeof(buf), 0) );
	ASSERT_STR_EQ( "1", buf );

	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 1, httplib_get_var2(shortquery, strlen(shortquery), "key2", buf, sizeof(buf), 0) );
	ASSERT_STR_EQ( "2", buf );

	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 1, httplib_get_var2(shortquery, strlen(shortquery), "key3", buf, sizeof(buf), 0) );
	ASSERT_STR_EQ( "3", buf );

	/* httplib_get_var call httplib_get_var2 with last argument 0 */
	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 1, httplib_get_var(shortquery, strlen(shortquery), "key1", buf, sizeof(buf)) );
	ASSERT_STR_EQ( "1", buf );

	/* longer value in the middle of a longer string */
	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 10, httplib_get_var2(longquery, strlen(longquery), "key7", buf, sizeof(buf), 0) );
	ASSERT_STR_EQ( "this is it", buf );

	/* longer value in the middle of a longer string - seccond occurance of key
	 */
	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 10, httplib_get_var2(longquery, strlen(longquery), "key7", buf, sizeof(buf), 1) );
	ASSERT_STR_EQ( "that is it", buf );

	/* key with = but without value in the middle of a longer string */
	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( 0, httplib_get_var2(longquery, strlen(longquery), "key5", buf, sizeof(buf), 0) );
	ASSERT_STR_EQ( buf, "" );

	/* key without = and without value in the middle of a longer string */
	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( -1, httplib_get_var2(longquery, strlen(longquery), "key6", buf, sizeof(buf), 0) );
	ASSERT_STR_EQ( buf, "" );

	PASS();
}


TEST
test_httplib_strncasecmp(void) {
	ASSERT_EQ( 0, httplib_strncasecmp("abc", "abc", 3) );
	ASSERT_EQ( 0, httplib_strncasecmp("abc", "abcd", 3) );
	ASSERT_NEQ( 0, httplib_strncasecmp("abc", "abcd", 4) );
	ASSERT_EQ( 0, httplib_strncasecmp("a", "A", 1) );

	ASSERT_LT_EX( 0, httplib_strncasecmp("A", "B", 1) );
	ASSERT_LT_EX( 0, httplib_strncasecmp("A", "b", 1) );
	ASSERT_LT_EX( 0, httplib_strncasecmp("a", "B", 1) );
	ASSERT_LT_EX( 0, httplib_strncasecmp("a", "b", 1) );
	ASSERT_GT_EX( 0, httplib_strncasecmp("b", "A", 1) );
	ASSERT_GT_EX( 0, httplib_strncasecmp("B", "A", 1) );
	ASSERT_GT_EX( 0, httplib_strncasecmp("b", "a", 1) );
	ASSERT_GT_EX( 0, httplib_strncasecmp("B", "a", 1) );

	ASSERT_LT_EX( 0, httplib_strncasecmp("xAx", "xBx", 3) );
	ASSERT_LT_EX( 0, httplib_strncasecmp("xAx", "xbx", 3) );
	ASSERT_LT_EX( 0, httplib_strncasecmp("xax", "xBx", 3) );
	ASSERT_LT_EX( 0, httplib_strncasecmp("xax", "xbx", 3) );
	ASSERT_GT_EX( 0, httplib_strncasecmp("xbx", "xAx", 3) );
	ASSERT_GT_EX( 0, httplib_strncasecmp("xBx", "xAx", 3) );
	ASSERT_GT_EX( 0, httplib_strncasecmp("xbx", "xax", 3) );
	ASSERT_GT_EX( 0, httplib_strncasecmp("xBx", "xax", 3) );

	PASS();
}


TEST
test_httplib_strcasestr(void) {
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	static char const *big1 = "abcdef";

	ASSERT_EQ( NULL, httplib_strcasestr("Y", "X") );
	ASSERT_NEQ( NULL, httplib_strcasestr("Y", "y") );
	ASSERT_EQ( NULL, httplib_strcasestr(big1, "X") );
	ASSERT_EQ( big1 + 2, httplib_strcasestr(big1, "CD") );
	ASSERT_EQ( NULL, httplib_strcasestr("aa", "AAB") );

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

	ASSERT_EQ( 3, httplib_url_decode("abc", 3, buf, sizeof(buf), 0) );
	ASSERT_STR_EQ( "abc", buf );

	ASSERT_EQ( 3, httplib_url_decode("abcdef", 3, buf, sizeof(buf), 0) );
	ASSERT_STR_EQ( "abc", buf );

	ASSERT_EQ( 3, httplib_url_decode("x+y", 3, buf, sizeof(buf), 0) );
	ASSERT_STR_EQ( "x+y", buf );

	ASSERT_EQ( 3, httplib_url_decode("x+y", 3, buf, sizeof(buf), 1) );
	ASSERT_STR_EQ( "x y", buf );

	ASSERT_EQ( 1, httplib_url_decode("%25", 3, buf, sizeof(buf), 1) );
	ASSERT_STR_EQ( "%", buf );

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

	memset(buf, 77, sizeof(buf));
	ret = httplib_url_encode("abc", buf, sizeof(buf));
	ASSERT_EQ( 3, ret );
	ASSERT_STR_EQ( "abc", buf );

	memset(buf, 77, sizeof(buf));
	ret = httplib_url_encode("a%b/c&d.e", buf, sizeof(buf));
	ASSERT_EQ( 15, ret );
	ASSERT_STR_EQ( "a%25b%2fc%26d.e", buf );

	memset(buf, 77, sizeof(buf));
	ret = httplib_url_encode("%%%", buf, 4);
	ASSERT_EQ( -1, ret );
	ASSERT_STR_EQ( "%25", buf );

	PASS();
}


TEST
test_httplib_version(void) {
	ASSERT_STR_EQ( LIBHTTP_VERSION, httplib_version() );

	PASS();
}


TEST
test_md5(void) {
	md5_state_t md5_state;
	unsigned char md5_val[16 + 1];
	char md5_str[32 + 1];
	const char *test_str = "The quick brown fox jumps over the lazy dog";

	md5_val[16] = 0;
	md5_init(&md5_state);
	md5_finish(&md5_state, md5_val);
	ASSERT_STR_EQ( "\xd4\x1d\x8c\xd9\x8f\x00\xb2\x04\xe9\x80\x09\x98\xec\xf8\x42\x7e", (const char *)md5_val );
	sprintf(md5_str,
	        "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
	        md5_val[0],
	        md5_val[1],
	        md5_val[2],
	        md5_val[3],
	        md5_val[4],
	        md5_val[5],
	        md5_val[6],
	        md5_val[7],
	        md5_val[8],
	        md5_val[9],
	        md5_val[10],
	        md5_val[11],
	        md5_val[12],
	        md5_val[13],
	        md5_val[14],
	        md5_val[15]);
	ASSERT_STR_EQ( "d41d8cd98f00b204e9800998ecf8427e", md5_str );

	md5_init(&md5_state);
	md5_append(&md5_state, (const md5_byte_t *)test_str, strlen(test_str));
	md5_finish(&md5_state, md5_val);
	sprintf(md5_str,
	        "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
	        md5_val[0],
	        md5_val[1],
	        md5_val[2],
	        md5_val[3],
	        md5_val[4],
	        md5_val[5],
	        md5_val[6],
	        md5_val[7],
	        md5_val[8],
	        md5_val[9],
	        md5_val[10],
	        md5_val[11],
	        md5_val[12],
	        md5_val[13],
	        md5_val[14],
	        md5_val[15]);
	ASSERT_STR_EQ( "9e107d9d372bb6826bd81d3542a419d6", md5_str );

	httplib_md5(md5_str, "", NULL);
	ASSERT_STR_EQ( "d41d8cd98f00b204e9800998ecf8427e", md5_str );

	httplib_md5(md5_str, test_str, NULL);
	ASSERT_STR_EQ( "9e107d9d372bb6826bd81d3542a419d6", md5_str );

	httplib_md5(md5_str, "civetweb", NULL);
	ASSERT_STR_EQ( "95c098bd85b619b24a83d9cea5e8ba54", md5_str );

	httplib_md5(md5_str,
	       "The",
	       " ",
	       "quick brown fox",
	       "",
	       " jumps ",
	       "over the lazy dog",
	       "",
	       "",
	       NULL);
	ASSERT_STR_EQ( "9e107d9d372bb6826bd81d3542a419d6", md5_str );

	char buf[33];
	const char *long_str =
	    "_123456789A123456789B123456789C123456789D123456789E123456789F123456789"
	    "G123456789H123456789I123456789J123456789K123456789L123456789M123456789"
	    "N123456789O123456789P123456789Q123456789R123456789S123456789T123456789"
	    "U123456789V123456789W123456789X123456789Y123456789Z";

	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( buf, httplib_md5(buf, NULL) );
	ASSERT_STR_EQ( "d41d8cd98f00b204e9800998ecf8427e", buf );

	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( buf, httplib_md5(buf, "The quick brown fox jumps over the lazy dog.", NULL) );
	ASSERT_STR_EQ( "e4d909c290d0fb1ca068ffaddf22cbd0", buf );

	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( buf, httplib_md5(buf, "", "The qu", "ick bro", "", "wn fox ju", "m", "ps over the la", "", "", "zy dog.", "", NULL) );
	ASSERT_STR_EQ( "e4d909c290d0fb1ca068ffaddf22cbd0", buf );

	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( buf, httplib_md5(buf, long_str, NULL) );
	ASSERT_STR_EQ( "1cb13cf9f16427807f081b2138241f08", buf );

	memset(buf, 77, sizeof(buf));
	ASSERT_EQ( buf, httplib_md5(buf, long_str + 1, NULL) );
	ASSERT_STR_EQ( "cf62d3264334154f5779d3694cc5093f", buf );

	PASS();
}


TEST
test_strtoll(void) {
	ASSERT_EQ( 0, strtoll("0", NULL, 10) );
	ASSERT_EQ( 123, strtoll("123", NULL, 10) );
	ASSERT_EQ( -34, strtoll("-34", NULL, 10) );
	ASSERT_EQ( 3566626116, strtoll("3566626116", NULL, 10) );

	PASS();
}


TEST
test_XX_httplib_alloc_vprintf(void) {
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	char buf[MG_BUF_LEN], *p = buf;

	ASSERT_EQ( 2, XX_httplib_alloc_printf(&p, buf, sizeof(buf), "%s", "hi") );
	ASSERT_EQ( buf, p );

	ASSERT_EQ( 0, XX_httplib_alloc_printf(&p, buf, sizeof(buf), "%s", "") );
	ASSERT_EQ( buf, p );

	ASSERT_EQ( 0, XX_httplib_alloc_printf(&p, buf, sizeof(buf), "") );
	ASSERT_EQ( buf, p );

	/* Pass small buffer, make sure XX_httplib_alloc_printf allocates */
	ASSERT_EQ( 5, XX_httplib_alloc_printf(&p, buf, 1, "%s", "hello") );
	ASSERT_NEQ( buf, p );
	httplib_free(p);
	p = buf;

	/* Test alternative implementation */
	ASSERT_EQ( 5, XX_httplib_alloc_printf2(&p, "%s", "hello") );
	ASSERT_NEQ( buf, p );
	httplib_free(p);
	p = buf;

	PASS();
}


TEST
test_XX_httplib_builtin_mime_ext(void) {
	int i = 0;
	const char *p1;
	const char *p2;

	while (true) {
		p1 = XX_httplib_builtin_mime_ext( i + 0 );
		if (p1 == NULL) break;

		p2 = XX_httplib_builtin_mime_ext( i + 1 );
		if (p2 == NULL) break;

		// Are they properly sorted?
		ASSERT_LT_EX( 0, httplib_strcasecmp( p1, p2 ) );

		i++;
	}

	PASS();
}


TEST
test_XX_httplib_builtin_mime_type(void) {
	int i = 0;
	const char *p_ext;
	const char *p_type;
	char buffer[128];

	while (true) {
		p_ext = XX_httplib_builtin_mime_ext( i );
		if (p_ext == NULL) break;
		p_type = XX_httplib_builtin_mime_type( i );
		if (p_type == NULL) break;

		snprintf( buffer, sizeof(buffer), "filename%s", p_ext );

		// Is binary search working correctly?
		ASSERT_STR_EQ( p_type, httplib_get_builtin_mime_type( buffer ) );

		i++;
	}

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
test_XX_httplib_mask_data(void) {
	char in[1024];
	char out[1024];
	int i;

	uint32_t mask = 0x61626364;
	/* TODO: adapt test for big endian */
	ASSERT_EQ( 0x64u, (*(unsigned char *)&mask) );

	memset(in, 0, sizeof(in));
	memset(out, 99, sizeof(out));

	XX_httplib_mask_data(in, sizeof(out), 0, out);
	ASSERT_EQ( 0, memcmp(out, in, sizeof(out)) );

	for (i = 0; i < 1024; i++) {
		in[i] = (char)((unsigned char)i);
	}
	XX_httplib_mask_data(in, 107, 0, out);
	ASSERT_EQ( 0, memcmp(out, in, 107) );

	XX_httplib_mask_data(in, 256, 0x01010101, out);
	for (i = 0; i < 256; i++) {
		ASSERT_EQ( (((unsigned char)in[i]) ^ (char)1u), ((unsigned char)out[i]) );
	}
	for (i = 256; i < (int)sizeof(out); i++) {
		ASSERT_EQ( 0, ((unsigned char)out[i]) );
	}

	/* TODO: check this for big endian */
	XX_httplib_mask_data(in, 5, 0x01020304, out);
	ASSERT_EQ( 0u ^ 4u, (unsigned char)out[0] );
	ASSERT_EQ( 1u ^ 3u, (unsigned char)out[1] );
	ASSERT_EQ( 2u ^ 2u, (unsigned char)out[2] );
	ASSERT_EQ( 3u ^ 1u, (unsigned char)out[3] );
	ASSERT_EQ( 4u ^ 4u, (unsigned char)out[4] );

	PASS();
}


TEST
test_XX_httplib_match_prefix(void) {
    	// ASSERT_GT_EX( 0, XX_httplib_match_prefix("", 0, "") );  // Expected success?

    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("", 0, "x") );  // Empty pattern matches anything.
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("x", 1, "") );

    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x", 1, "x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("X", 1, "X") );

    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x", 1, "X") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("X", 1, "x") );

    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("?", 1, "") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("?", 1, "x") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("x?", 2, "x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x?", 2, "xx") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x?", 2, "xxx") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("?x", 2, "x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("?x", 2, "xx") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("?x", 2, "xxx") );

    	// ASSERT_GT_EX( 0, XX_httplib_match_prefix("*", 1, "") );  // Expected success?
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("*", 1, "x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x*", 2, "x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x*", 2, "xx") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x*", 2, "xxx") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("*x", 2, "x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("*x", 2, "xx") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("*x", 2, "xxx") );

    	// ASSERT_GT_EX( 0, XX_httplib_match_prefix("**", 1, "") );  // Crash! (Note `len`: 1)

    	// ASSERT_GT_EX( 0, XX_httplib_match_prefix("**", 2, "") );  // Expected success?
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**", 2, "x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x**", 3, "x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x**", 3, "xx") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x**", 3, "xxx") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**x", 3, "x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**x", 3, "xx") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**x", 3, "xxx") );

    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**", 2, "/") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**", 2, "/x") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("x**", 3, "/x") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("x**", 3, "/xx") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("x**", 3, "/xxx") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**x", 3, "/x") );  // Success?
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**x", 3, "/xx") );  // Success?
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**x", 3, "/xxx") );  // Success?

    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**", 2, "/") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**", 2, "x/") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x**", 3, "x/") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x**", 3, "xx/") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x**", 3, "xxx/") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**x", 3, "x/") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**x", 3, "xx/") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("**x", 3, "xxx/") );

    	// ASSERT_GT_EX( 0, XX_httplib_match_prefix("$", 1, "") );  // Expected success?
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("$", 1, "x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("x$", 2, "x") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("x$", 2, "xx") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("x$", 2, "xxx") );

    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("/$", 2, "/") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/$", 2, "/x") );

    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x", 2, "/") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("/x", 2, "/x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("/x", 2, "/xxx") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("/x", 2, "/xxx/xxx") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x", 2, "/z") );

    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x/x/", 5, "/") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x/x/", 5, "/x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("/x/x/", 5, "/x/x/") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("/x/x/", 5, "/x/x/x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("/x/x/", 5, "/x/x/xxx") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("/x/x/", 5, "/x/x/xxx/xxx") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x/x/", 5, "/x/z/") );

    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x/*/x/", 7, "/") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x/*/x/", 7, "/x") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x/*/x/", 7, "/x/x/") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x/*/x/", 7, "/x/x/x") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x/*/x/", 7, "/x/x/x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("/x/*/x/", 7, "/x/x/x/xxx") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x/*/x/", 7, "/x/z/") );
    	ASSERT_LTE_EX( 0, XX_httplib_match_prefix("/x/*/x/", 7, "/x/z/x") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("/x/*/x/", 7, "/x/z/x/") );
    	ASSERT_GT_EX( 0, XX_httplib_match_prefix("/x/*/x/", 7, "/x/z/x/xxx") );

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

	ASSERT_EQ( NULL, XX_httplib_next_option(NULL, &a, &b) );
	for (i = 0, p = list; (p = XX_httplib_next_option(p, &a, &b)) != NULL; i++) {
		ASSERT( i != 0 || (a.ptr == list && a.len == 3 && b.len == 0) );
		ASSERT( i != 1 || (a.ptr == list + 4 && a.len == 4 && b.ptr == list + 9 && b.len == 4) );
		ASSERT( i != 2 || (a.ptr == list + 14 && a.len == 1 && b.len == 0) );
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
	ASSERT_NEQ( NULL, p );
	ASSERT_STR_EQ( "a=1", p );

	p = XX_httplib_skip_quoted(&s, ", ", ", ", 0);
	ASSERT_NEQ( NULL, p );
	ASSERT_STR_EQ( "b=2", p );

	p = XX_httplib_skip_quoted(&s, ",", " ", 0);
	ASSERT_NEQ( NULL, p );
	ASSERT_STR_EQ( "c='hi \' there'", p );

	p = XX_httplib_skip_quoted(&s, ",", " ", '\\');
	ASSERT_NEQ( NULL, p );
	ASSERT_STR_EQ( "d='here, there'", p );
	ASSERT_EQ( '\0', *s );

	PASS();
}


TEST
test_XX_httplib_stat(void) {
	static struct httplib_context ctx = {0};
	static struct httplib_connection conn = {0};
	struct file file = STRUCT_FILE_INITIALIZER;

	ASSERT_FALSE( XX_httplib_stat(NULL, NULL, " does not exist ", &file) );
	ASSERT_FALSE( XX_httplib_stat(&ctx, &conn, " does not exist ", &file) );

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

	RUN_TEST(test_httplib_base64_decode);
	RUN_TEST(test_httplib_base64_encode);
	RUN_TEST(test_httplib_check_feature);
	RUN_TEST(test_httplib_get_builtin_mime_type);
	RUN_TEST(test_httplib_get_cookie);
	RUN_TEST(test_httplib_get_response_code_text);
	RUN_TEST(test_httplib_get_var);
	RUN_TEST(test_httplib_strncasecmp);
	RUN_TEST(test_httplib_strcasestr);
	RUN_TEST(test_httplib_url_decode);
	RUN_TEST(test_httplib_url_encode);
	RUN_TEST(test_httplib_version);
	RUN_TEST(test_md5);
	RUN_TEST(test_strtoll);
	RUN_TEST(test_XX_httplib_alloc_vprintf);
	RUN_TEST(test_XX_httplib_builtin_mime_ext);
	RUN_TEST(test_XX_httplib_builtin_mime_type);
	RUN_TEST(test_XX_httplib_get_uri_type);
	RUN_TEST(test_XX_httplib_mask_data);
	RUN_TEST(test_XX_httplib_match_prefix);
	RUN_TEST(test_XX_httplib_next_option);
	RUN_TEST(test_XX_httplib_parse_date_string);
	RUN_TEST(test_XX_httplib_parse_http_message);
	RUN_TEST(test_XX_httplib_parse_port_string);
	RUN_TEST(test_XX_httplib_remove_double_dots_and_double_slashes);
	RUN_TEST(test_XX_httplib_should_keep_alive);
	RUN_TEST(test_XX_httplib_skip_quoted);
	RUN_TEST(test_XX_httplib_stat);

	GREATEST_MAIN_END();
}
