#ifndef EVIL_CAT_WEAPON_H
#define EVIL_CAT_WEAPON_H

#include <utility>
#include <vector>

using CurveKey = std::pair<float, float>;

using Curve = std::vector<CurveKey>;

struct Weapon {
    float zeroDistance;
    Curve speedCurve;

    Weapon(float zeroDistance, Curve speedCurve)
            : zeroDistance(zeroDistance), speedCurve(std::move(speedCurve)) {}

    explicit Weapon(Curve speedCurve) : Weapon(0.f, std::move(speedCurve)) {}

    explicit Weapon(float speed) : Weapon(0.f, {{0.f, speed}}) {}

    Weapon() : Weapon({}, {}) {}
};

#endif //EVIL_CAT_WEAPON_H
