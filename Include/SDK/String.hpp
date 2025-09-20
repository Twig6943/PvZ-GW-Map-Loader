#pragma once

#include "Types.hpp"
#include <string.h>

namespace fb {

/*
    Underlying data is just a char*, but modifications is handled through calling
    game functions to use Frostbite's allocators.
*/
class String {
public:
    String() { m_string = nullptr; }

    const char* asCString() { return m_string; }
    bool equals(const char* str) { return strcmp(m_string, str) == 0; }

    void set(const char* str);

private:
    /* 0x00 */ char* m_string;

}; /* Size: 0x08 */

}
