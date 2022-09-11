#ifndef EVIL_CAT_TYPE_H
#define EVIL_CAT_TYPE_H

struct Vec2 {
    float x, y;
    // add
    Vec2 operator+(const Vec2& v) const {
        return Vec2{x + v.x, y + v.y};
    }
    // sub
    Vec2 operator-(const Vec2& v) const {
        return Vec2{x - v.x, y - v.y};
    }
    // mul
    Vec2 operator*(float v) const {
        return Vec2{x * v, y * v};
    }
    // div
    Vec2 operator/(float v) const {
        return Vec2{x / v, y / v};
    }
};

struct Vec3 {
    float x, y, z;
    // add
    Vec3 operator+(const Vec3& v) const {
        return { x + v.x, y + v.y, z + v.z };
    }
    // sub
    Vec3 operator-(const Vec3& v) const {
        return { x - v.x, y - v.y, z - v.z };
    }
    // mul
    Vec3 operator*(float v) const {
        return { x * v, y * v, z * v };
    }
    // div
    Vec3 operator/(float v) const {
        return { x / v, y / v, z / v };
    }
};

struct Vec4 {
    float x, y, z, w;
    // add
    Vec4 operator+(const Vec4& v) const {
        return { x + v.x, y + v.y, z + v.z, w + v.w };
    }
    // sub
    Vec4 operator-(const Vec4& v) const {
        return { x - v.x, y - v.y, z - v.z, w - v.w };
    }
    // mul
    Vec4 operator*(float v) const {
        return { x * v, y * v, z * v, w * v };
    }
    // div
    Vec4 operator/(float v) const {
        return { x / v, y / v, z / v, w / v };
    }
};

struct Mat4 {
    float m[4][4];
};

#endif //EVIL_CAT_TYPE_H
