#pragma once

#include <Types.hpp>

namespace fb {

struct Vec3 {
    f32 x, y, z;
};

struct Vec4 {
    Vec4(const f32 x = 0.0f, const f32 y = 0.0f, const f32 z = 0.0f, const f32 w = 0.0f)
        : x(x), y(y), z(z), w(w) {}

    void zero() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }

    f32 x, y, z, w;
};

struct Matrix4x4 {
    void zero() {
        x.zero();
        y.zero();
        z.zero();
        w.zero();
    }

    static Matrix4x4& identity() {
        
    }

    Vec4 x;
    Vec4 y;
    Vec4 z;
    Vec4 w;
};

} // namespace fb
