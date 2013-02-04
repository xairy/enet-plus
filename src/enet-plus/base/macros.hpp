#ifndef ENET_PLUS_BASE_MACROS_HPP_
#define ENET_PLUS_BASE_MACROS_HPP_

#include <cstdio>
#include <cstdlib>

// The CHECK() macro is used for checking assertions, and will cause
// an immediate crash if its assertion is not met. DCHECK() is like
// CHECK() but is only compiled in on debug builds.
#define CHECK(x)                                                  \
  do {                                                            \
    if(!(x)) {                                                    \
      fprintf(stderr, "Assertion failed: %s, file %s, line %u\n", \
        #x, __FILE__, __LINE__);                                  \
      abort();                                                    \
    }                                                             \
  } while(0)

#ifdef NDEBUG
# define DCHECK(x)
#else
# define DCHECK(x) CHECK(x)
#endif

// The SCHECK() macro is used for checking compile-time assertions,
// and will cause a compilation error if its assertion is not met.
template<bool x> struct __SCHECK_F;
template<      > struct __SCHECK_F <true> {};
template<int  x> struct __SCHECK_P        {};
#define SCHECK(B) \
  typedef __SCHECK_P<sizeof(__SCHECK_F<((B)? true : false)>)> __SCHECK_ASSERT##__LINE__

// A macro to disallow the copy constructor and operator= functions.
// This should be used in the private: declarations for a class.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)

#endif // ENET_PLUS_BASE_MACROS_HPP_
