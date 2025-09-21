#pragma once

#include <stddef.h>
#include <Types.hpp>

#include <stdexcept>

namespace fb {

class DataContainer {
public:

private:
    /* 0x00 */ void** vftptr;
    /* 0x08*/ u32 m_data;

};

template <typename T>
class Array {
public:
    const s32 size() { return *reinterpret_cast<s32*>((reinterpret_cast<char*>(m_data) - 0x04)); }

    T& operator[](size_t index) { 
        if (isIndexOutOfBounds(index)) { 
            throw std::out_of_range("Array index out of bounds.");
        }

        return m_data[index]; 
    }
    const T& operator[](size_t index) const { 
        if (isIndexOutOfBounds(index)) {
            throw std::out_of_range("Array index out of bounds.");
        }

        return m_data[index]; 
    }

    T* begin() { return m_data; }
    T* end() { return m_data + size(); }
    const T* begin() const { return m_data; }
    const T* end() const { return m_data + size(); }

private:
    bool isIndexOutOfBounds(size_t index) { return index >= size(); }

private:
    /* 0x00 */ T* m_data;

}; /* Size: 0x08 */

template <typename T>
class RefArray {
public:
    const s32 size() { return *reinterpret_cast<s32*>((reinterpret_cast<char*>(m_data) - 0x04)); }

    T* operator[](size_t index) { return m_data[index]; }
    const T* operator[](size_t index) const { return m_data[index]; }
    
    T** begin() { return m_data; }
    T** end() { return m_data + size(); }
    const T** begin() const { return m_data; }
    const T** end() const { return m_data + size(); }

private:
    /* 0x00 */ T** m_data;

}; /* Size: 0x08 */

template <typename T>
class WeakPtr {
public:
    T* getPtr() {
        return reinterpret_cast<T*>(m_ptr - 8);
    }

private:
    /* 0x00 */ void* m_ptr;

}; /* Size: 0x08 */

}
