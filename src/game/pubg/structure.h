#ifndef EVIL_CAT_GAME_PUBG_STRUCTURE_H
#define EVIL_CAT_GAME_PUBG_STRUCTURE_H

#include <array>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct TArray {
    uint64_t ptr;
    uint32_t cnt;
    uint32_t max;
};

using FString = TArray;

struct FNamePool {
    uint64_t chunks;
    uint64_t numElements;
    uint64_t numChunks;
};

struct FNameEntry {
    uint64_t id; // real id = id >> 1, the LSB is wide char flag
    uint64_t next;
    std::array<uint8_t, 128> buf;
};

struct FRotator {
    /** Rotation around the right axis (around Y axis),
     * Looking up and down (0=Straight Ahead, +Up, -Down) */
    float pitch;

    /** Rotation around the up axis (around Z axis),
     * Running in circles 0=East, +North, -South. */
    float yaw;

    /** Rotation around the forward axis (around X axis),
     * Tilting your head, 0=Straight, +Clockwise, -CCW. */
    float roll;

    FRotator operator+(const FRotator &other) const {
        return {pitch + other.pitch, yaw + other.yaw, roll + other.roll};
    }

    FRotator operator+=(const FRotator &other) {
        pitch += other.pitch;
        yaw += other.yaw;
        roll += other.roll;
        return *this;
    }

    operator glm::vec3() const {
        return {pitch, yaw, roll};
    }
};

struct FTransform {
    glm::quat rotation;
    glm::vec3 translation;
    uint8_t pad1[4];
    glm::vec3 scale;
    uint8_t pad2[4];

    // copied from UE4 FTransform::ToMatrixWithScale
    glm::mat4 toMatrix() const {
        glm::mat4 matrix;

        matrix[3][0] = translation.x;
        matrix[3][1] = translation.y;
        matrix[3][2] = translation.z;

        const float x2 = rotation.x + rotation.x;
        const float y2 = rotation.y + rotation.y;
        const float z2 = rotation.z + rotation.z;
        {
            const float xx2 = rotation.x * x2;
            const float yy2 = rotation.y * y2;
            const float zz2 = rotation.z * z2;
            matrix[0][0] = (1.0f - (yy2 + zz2)) * scale.x;
            matrix[1][1] = (1.0f - (xx2 + zz2)) * scale.y;
            matrix[2][2] = (1.0f - (xx2 + yy2)) * scale.z;
        }
        {
            const float yz2 = rotation.y * z2;
            const float wx2 = rotation.w * x2;
            matrix[2][1] = (yz2 - wx2) * scale.z;
            matrix[1][2] = (yz2 + wx2) * scale.y;
        }
        {
            const float xy2 = rotation.x * y2;
            const float wz2 = rotation.w * z2;
            matrix[1][0] = (xy2 - wz2) * scale.y;
            matrix[0][1] = (xy2 + wz2) * scale.x;
        }
        {
            const float xz2 = rotation.x * z2;
            const float wy2 = rotation.w * y2;
            matrix[2][0] = (xz2 + wy2) * scale.z;
            matrix[0][2] = (xz2 - wy2) * scale.x;
        }

        matrix[0][3] = 0.0f;
        matrix[1][3] = 0.0f;
        matrix[2][3] = 0.0f;
        matrix[3][3] = 1.0f;

        return matrix;
    }

    FTransform operator*(const FTransform &other) const {
        FTransform result;
        result.rotation = other.rotation * rotation;
        result.scale = scale * other.scale;
        result.translation = other.rotation * (other.scale * translation) + other.translation;
        return result;
    }

};

template<typename T1, typename T2>
struct TPair {
    T1 Key;
    T2 Value;
};

struct FName {
    uint32_t id;
    uint32_t id2;
};

// ScriptStruct Engine.RichCurveKey
// Size: 0x1c (Inherited: 0x00)
struct FRichCurveKey {
    uint8_t InterpMode; // 0x00(0x01)
    uint8_t TangentMode; // 0x01(0x01)
    uint8_t TangentWeightMode; // 0x02(0x01)
    char pad_3[0x1]; // 0x03(0x01)
    float Time; // 0x04(0x04)
    float Value; // 0x08(0x04)
    float ArriveTangent; // 0x0c(0x04)
    float ArriveTangentWeight; // 0x10(0x04)
    float LeaveTangent; // 0x14(0x04)
    float LeaveTangentWeight; // 0x18(0x04)
};

// ScriptStruct Engine.InputAxisProperties
// Size: 0x20 (Inherited: 0x00)
struct FInputAxisProperties {
    float DEADZONE; // 0x00(0x04)
    float OuterDeadZone; // 0x04(0x04)
    float Sensitivity; // 0x08(0x04)
    float Exponent; // 0x0c(0x04)
    char bInvert : 1; // 0x10(0x01)
    char pad_10_1 : 7; // 0x10(0x01)
    char pad_11[0x7]; // 0x11(0x07)
    void* MultiplierAxisValueCurve; // 0x18(0x08)
};

struct CameraInfo {
    glm::vec3 location;
    union {
        glm::vec3 viewAngle;
        FRotator rotation;
    };
    float FOV;
};

struct ItemInfo {
    std::string name;
    int level;
};

#endif //EVIL_CAT_GAME_PUBG_STRUCTURE_H
