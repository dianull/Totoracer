#include <iostream>

struct Colour {
    float r;
    float g;
    float b;

    Colour& operator+=(const Colour& rhs) { 
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        return *this; 
    }

    Colour& operator/=(const float rhs) { 
        r /= rhs;
        g /= rhs;
        b /= rhs;
        return *this; 
    }

    Colour& operator*=(const float rhs) { 
        r *= rhs;
        g *= rhs;
        b *= rhs;
        return *this; 
    }
};

struct Vec2 {
    float x;
    float y;

    float dot(const Vec2& lhs, const Vec2& rhs) {
        return 0;
    }

    Vec2& operator+=(const Vec2& rhs) { 
        x += rhs.x;
        y += rhs.y;
        return *this; 
    }

    Vec2& operator-=(const Vec2& rhs) { 
        x -= rhs.x;
        y -= rhs.y;
        return *this; 
    }

    Vec2& operator*=(const Vec2& rhs) { 
        x *= rhs.x;
        y *= rhs.y;
        return *this; 
    }
   
};

struct Circle {
    Vec2 centre;
    Colour colour;
    int radius;
};

// From https://github.com/skeeto/hash-prospector
constexpr inline uint32_t lowbias32(uint32_t x) {
    x ^= x >> 16; x *= 0x21f0aaad;
    x ^= x >> 15; x *= 0xd35a2d97;
    x ^= x >> 15;
    return x;
}