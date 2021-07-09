#include "Assert.hpp"
void Assert_(bool mustBeTrue, const char* conditionText, const char* filename, const char* funcname,
             int lineNum, const char* message)
{
    if (!mustBeTrue) {
        std::cerr << "Assertion: " << conditionText
                  << "\nFailed!\n"
                  << "Message: " << message << "\n"
                  << "File: " << filename << "\n"
                  << "Function: " << funcname << "\n"
                  << "Line: " << lineNum << "\n";
        debug_trap();
    }
}
