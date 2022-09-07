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
};

struct Mat4 {
    float m[4][4];
};

#endif //EVIL_CAT_TYPE_H
