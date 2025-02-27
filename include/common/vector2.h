#pragma once

// from https://bitbucket.org/rude/love/src/default/src/common/Vector.h
struct Vector2
{
    float x, y;

    Vector2() : x(0.0f), y(0.0f) {}

    Vector2(float x, float y) : x(x), y(y) {}

    Vector2(const Vector2 &v) : x(v.x), y(v.y) {}

    const inline Vector2 operator - (const Vector2 & v)
    {
        return Vector2(x - v.x, y - v.y);
    }

    static inline float cross(const Vector2 & a, const Vector2 & b)
    {
        return a.x * b.y - a.y * b.x;
    }
};
