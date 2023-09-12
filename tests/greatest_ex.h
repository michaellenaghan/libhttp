#ifndef GREATEST_EX_H
#define GREATEST_EX_H


#include "greatest.h"


#if defined(__cplusplus) && !defined(GREATEST_NO_EXTERN_CPLUSPLUS)
extern "C" {
#endif


// Let's say that a function returns > 0 on success, and <= 0 on failure.
// With the given macros you'd need to test for success like this:
//
//     ASSERT_LTE( 0, XX_httplib_match_prefix("x", 1, "x") );
//
// and for failure like this:
//
//     ASSERT_GT( 0, XX_httplib_match_prefix("x", 1, "z") );
//
// My head keeps telling me that's backwards -- that success should be
// "GT 0" and failure should be "LTE 0". So that's what these macros do:
//
//     // Failure
//     ASSERT_LTE_EX( 0, XX_httplib_match_prefix("x", 1, "x") );
//
// and for failure like this:
//
//     // Success
//     ASSERT_GT_EX( 0, XX_httplib_match_prefix("x", 1, "z") );



#define GREATEST_ASSERT_GT_EX(EXP, GOT)                                    \
    GREATEST_ASSERT_GT_EXm(#EXP " <= " #GOT, EXP, GOT)
#define GREATEST_ASSERT_GTE_EX(EXP, GOT)                                   \
    GREATEST_ASSERT_GTE_EXm(#EXP " < " #GOT, EXP, GOT)
#define GREATEST_ASSERT_LT_EX(EXP, GOT)                                    \
    GREATEST_ASSERT_LT_EXm(#EXP " >= " #GOT, EXP, GOT)
#define GREATEST_ASSERT_LTE_EX(EXP, GOT)                                   \
    GREATEST_ASSERT_LTE_EXm(#EXP " > " #GOT, EXP, GOT)


#define GREATEST_ASSERT_GT_EXm(MSG,E,G) GREATEST__REL(>, MSG,G,E)  // E,G -> G,E
#define GREATEST_ASSERT_GTE_EXm(MSG,E,G) GREATEST__REL(>=, MSG,G,E)  // E,G -> G,E
#define GREATEST_ASSERT_LT_EXm(MSG,E,G) GREATEST__REL(<, MSG,G,E)  // E,G -> G,E
#define GREATEST_ASSERT_LTE_EXm(MSG,E,G) GREATEST__REL(<=, MSG,G,E)  // E,G -> G,E


#if GREATEST_USE_ABBREVS
#define ASSERT_GT_EX      GREATEST_ASSERT_GT_EX
#define ASSERT_GTE_EX     GREATEST_ASSERT_GTE_EX
#define ASSERT_LT_EX      GREATEST_ASSERT_LT_EX
#define ASSERT_LTE_EX     GREATEST_ASSERT_LTE_EX
#define ASSERT_GT_EXm     GREATEST_ASSERT_GT_EXm
#define ASSERT_GTE_EXm    GREATEST_ASSERT_GTE_EXm
#define ASSERT_LT_EXm     GREATEST_ASSERT_LT_EXm
#define ASSERT_LTE_EXm    GREATEST_ASSERT_LTE_EXm
#endif /* USE_ABBREVS */


#if defined(__cplusplus) && !defined(GREATEST_NO_EXTERN_CPLUSPLUS)
}
#endif


#endif
