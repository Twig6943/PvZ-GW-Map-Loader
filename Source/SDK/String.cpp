#include <SDK/String.hpp>

#include <Types.hpp>
#include <SDK/Offsets.hpp>

void fb::String::set(const char* str) {
    using tStringSet = fb::String*(*)(fb::String*, const char*, size_t);
    auto pStringSetFunc = reinterpret_cast<tStringSet>(offsets::g_setStringFunction);
    pStringSetFunc(this, str, strlen(str));
}
