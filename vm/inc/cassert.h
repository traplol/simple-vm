#ifndef _COMPILE_TIME_ASSERTION__CASSERT_H
#define _COMPILE_TIME_ASSERTION__CASSERT_H

/*
 * These helpers serve as compile time assertions to stop the compiler
 * from building if one of these fails. They can be used to check constants
 * such as the sizeof types or the value of a enum and alert the user upon
 * failure.
 * Example:
 * 
 *   compile_assert(sizeof(int) == 5)
 *
 * Output:
 *
 *   cassert.h:11:1: error: 'compile_assertion_failed_at_line_11' declared as an array with a negative size.
 *
 * These macros can be disabled by defining NDEBUG or compiling with the
 * -DNDEBUG flag.
 */

#ifndef NDEBUG

  #define _impl_PASTE(a,b) __impl__PASTE(a,b)
  #define __impl__PASTE(a,b) a ## b
  #define _impl_FAILED_AT_LINE _impl_PASTE(failed_at_line_, __LINE__)
  
  #define CASSERT_MSG(predicate, msg) \
      typedef char _impl_PASTE(compiler_assertion_, msg) [(predicate) ? (+1) : (-1)]
  
  #define CASSERT(predicate) CASSERT_MSG(predicate, _impl_FAILED_AT_LINE)
  #define compile_assert(predicate) CASSERT(predicate)

#else

  #define CASSERT_MSG(predicate, msg)
  #define CASSERT(predicate)
  #define compile_assert(predicate)

#endif /* ifndef NDEBUG */
#endif /* ifndef _COMPILE_TIME_ASSERTION__CASSERT_H */
