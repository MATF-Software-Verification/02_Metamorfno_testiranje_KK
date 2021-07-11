#ifndef ASSERT_H
#define ASSERT_H

template<bool> struct CompileTimeAssert
{
    CompileTimeAssert(...);
};


template<> struct CompileTimeAssert<false> {};

#define STATIC_ASSERT(expr, msg)\
{\
    class COMPILE_TIME_ERROR_##msg {};\
    (void)sizeof(CompileTimeAssert<expr>(COMPILE_TIME_ERROR_##msg()));\
}

#if DEBUG
#include<iostream>
#ifdef __MSVC__
#include<intrin.h>
#define debug_trap()  __debug_break()
#else
#define debug_trap()  __builtin_trap()
#endif


#define Assert(condition, message) Assert_(condition, #condition, __FILE__, __func__, __LINE__, message)
void Assert_(bool mustBeTrue, const char* conditionText, const char* filename, const char* funcname,
             int lineNum, const char* message);


#else

#define Assert(...)
#endif

#endif // ASSERT_H
