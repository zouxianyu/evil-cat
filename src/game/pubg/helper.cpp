#include <unordered_set>
#include <memory>
#include <algorithm>
#include <array>
#include <glm/glm.hpp>
#include "magic_enum.h"
#include "mem/memory_accessor.h"
#include "structure.h"
#include "offset.h"
#include "decrypt.h"
#include "name_pool.h"
#include "helper.h"

bool PUBG::Helper::isAIPlayer(const std::string &objectName) {
    const static std::unordered_set<std::string> list {
            "AIPawn_Base_Female_C",
            "AIPawn_Base_Male_C",
            "UltAIPawn_Base_Female_C",
            "UltAIPawn_Base_Male_C",
            "AIPawn_Base_Male_Pillar_C",
    };

    return list.find(objectName) != list.end();
}

bool PUBG::Helper::isRealPlayer(const std::string &objectName) {
    const static std::unordered_set<std::string> list {
            "PlayerFemale_A_C",
            "PlayerMale_A_C",
    };

    return list.find(objectName) != list.end();
}

bool PUBG::Helper::isPlayer(const std::string &objectName) {
    return isRealPlayer(objectName) || isAIPlayer(objectName);
}

std::string PUBG::Helper::toString(FString str) {
    auto mem = std::make_unique<wchar_t[]>(str.cnt);
    MemoryAccessor<wchar_t>(str.ptr).get(mem.get(), str.cnt);
    std::wstring wstr = std::wstring(mem.get(), str.cnt);
    return {wstr.begin(), wstr.end()};
}

uint64_t PUBG::Helper::getPlayerController() {
    uint64_t gworld = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            "TslGame.exe",
            Offset_GWorld
    ));

    uint64_t gameInstance = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            gworld + Offset_GameInstance
    ));

    TArray localPlayers = MemoryAccessor<TArray>(
            gameInstance + Offset_LocalPlayers
    );

    uint64_t localPlayer0 = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            localPlayers.ptr + 0
    ));

    uint64_t playerController = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            localPlayer0 + Offset_PlayerController
    ));

    return playerController;
}

CameraInfo PUBG::Helper::getCameraInfo() {
    uint64_t playerController = getPlayerController();
    uint64_t playerCameraManager = MemoryAccessor<uint64_t>(
            playerController + Offset_PlayerCameraManager
    );
    glm::vec3 cameraLocation = MemoryAccessor<glm::vec3>(
            playerCameraManager + Offset_CameraLocation
    );
    glm::vec3 cameraRotation = MemoryAccessor<glm::vec3>(
            playerCameraManager + Offset_CameraRotation
    );
    float cameraFov = MemoryAccessor<float>(
            playerCameraManager + Offset_CameraFov
    );

    return {cameraLocation, cameraRotation, cameraFov};
}

void PUBG::Helper::setCameraRotation(glm::vec3 rotation) {
    glm::vec3 delta = rotation - getCameraInfo().viewAngle;
    delta *= 4.f;

    RECT rect;
    GetClientRect(GetForegroundWindow(), &rect);
    INPUT input{};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = static_cast<LONG>(delta.y * static_cast<float>(rect.right) / 180.f);
    input.mi.dy = static_cast<LONG>(-delta.x * static_cast<float>(rect.bottom) / 180.f);
    SendInput(1, &input, sizeof(INPUT));
}

// from lily
enum class EBoneIndex : int {
    root = 0,
    pelvis = 1,
    spine_01 = 2,
    spine_02 = 3,
    spine_03 = 4,
    neck_01 = 5,
    Head = 6,
    face_root = 7,
    eyebrows_pos_root = 8,
    eyebrows_root = 9,
    eyebrows_r = 10,
    eyebrows_l = 11,
    eyebrow_l = 12,
    eyebrow_r = 13,
    forehead_root = 14,
    forehead = 15,
    jaw_pos_root = 16,
    jaw_root = 17,
    jaw = 18,
    mouth_down_pos_root = 19,
    mouth_down_root = 20,
    lip_bm_01 = 21,
    lip_bm_02 = 22,
    lip_br = 23,
    lip_bl = 24,
    jaw_01 = 25,
    jaw_02 = 26,
    cheek_pos_root = 27,
    cheek_root = 28,
    cheek_r = 29,
    cheek_l = 30,
    nose_side_root = 31,
    nose_side_r_01 = 32,
    nose_side_r_02 = 33,
    nose_side_l_01 = 34,
    nose_side_l_02 = 35,
    eye_pos_r_root = 36,
    eye_r_root = 37,
    eye_rot_r_root = 38,
    eye_lid_u_r = 39,
    eye_r = 40,
    eye_lid_b_r = 41,
    eye_pos_l_root = 42,
    eye_l_root = 43,
    eye_rot_l_root = 44,
    eye_lid_u_l = 45,
    eye_l = 46,
    eye_lid_b_l = 47,
    nose_pos_root = 48,
    nose = 49,
    mouth_up_pos_root = 50,
    mouth_up_root = 51,
    lip_ul = 52,
    lip_um_01 = 53,
    lip_um_02 = 54,
    lip_ur = 55,
    lip_l = 56,
    lip_r = 57,
    hair_root = 58,
    hair_b_01 = 59,
    hair_b_02 = 60,
    hair_l_01 = 61,
    hair_l_02 = 62,
    hair_r_01 = 63,
    hair_r_02 = 64,
    hair_f_02 = 65,
    hair_f_01 = 66,
    hair_b_pt_01 = 67,
    hair_b_pt_02 = 68,
    hair_b_pt_03 = 69,
    hair_b_pt_04 = 70,
    hair_b_pt_05 = 71,
    hair_l_pt_01 = 72,
    hair_l_pt_02 = 73,
    hair_l_pt_03 = 74,
    hair_l_pt_04 = 75,
    hair_l_pt_05 = 76,
    hair_r_pt_01 = 77,
    hair_r_pt_02 = 78,
    hair_r_pt_03 = 79,
    hair_r_pt_04 = 80,
    hair_r_pt_05 = 81,
    camera_fpp = 82,
    GunReferencePoint = 83,
    GunRef = 84,
    breast_l = 85,
    breast_r = 86,
    clavicle_l = 87,
    upperarm_l = 88,
    lowerarm_l = 89,
    hand_l = 90,
    thumb_01_l = 91,
    thumb_02_l = 92,
    thumb_03_l = 93,
    thumb_04_l_MBONLY = 94,
    index_01_l = 95,
    index_02_l = 96,
    index_03_l = 97,
    index_04_l_MBONLY = 98,
    middle_01_l = 99,
    middle_02_l = 100,
    middle_03_l = 101,
    middle_04_l_MBONLY = 102,
    ring_01_l = 103,
    ring_02_l = 104,
    ring_03_l = 105,
    ring_04_l_MBONLY = 106,
    pinky_01_l = 107,
    pinky_02_l = 108,
    pinky_03_l = 109,
    pinky_04_l_MBONLY = 110,
    item_l = 111,
    lowerarm_twist_01_l = 112,
    upperarm_twist_01_l = 113,
    clavicle_r = 114,
    upperarm_r = 115,
    lowerarm_r = 116,
    hand_r = 117,
    thumb_01_r = 118,
    thumb_02_r = 119,
    thumb_03_r = 120,
    thumb_04_r_MBONLY = 121,
    index_01_r = 122,
    index_02_r = 123,
    index_03_r = 124,
    index_04_r_MBONLY = 125,
    middle_01_r = 126,
    middle_02_r = 127,
    middle_03_r = 128,
    middle_04_r_MBONLY = 129,
    ring_01_r = 130,
    ring_02_r = 131,
    ring_03_r = 132,
    ring_04_r_MBONLY = 133,
    pinky_01_r = 134,
    pinky_02_r = 135,
    pinky_03_r = 136,
    pinky_04_r_MBONLY = 137,
    item_r = 138,
    lowerarm_twist_01_r = 139,
    upperarm_twist_01_r = 140,
    Backpack = 141,
    backpack_01 = 142,
    backpack_02 = 143,
    slot_primary = 144,
    slot_secondary = 145,
    slot_melee = 146,
    slot_throwable = 147,
    coat_l_01 = 148,
    coat_l_02 = 149,
    coat_l_03 = 150,
    coat_l_04 = 151,
    coat_fl_01 = 152,
    coat_fl_02 = 153,
    coat_fl_03 = 154,
    coat_fl_04 = 155,
    coat_b_01 = 156,
    coat_b_02 = 157,
    coat_b_03 = 158,
    coat_b_04 = 159,
    coat_r_01 = 160,
    coat_r_02 = 161,
    coat_r_03 = 162,
    coat_r_04 = 163,
    coat_fr_01 = 164,
    coat_fr_02 = 165,
    coat_fr_03 = 166,
    coat_fr_04 = 167,
    thigh_l = 168,
    calf_l = 169,
    foot_l = 170,
    ball_l = 171,
    calf_twist_01_l = 172,
    thigh_twist_01_l = 173,
    thigh_r = 174,
    calf_r = 175,
    foot_r = 176,
    ball_r = 177,
    calf_twist_01_r = 178,
    thigh_twist_01_r = 179,
    slot_sidearm = 180,
    skirt_l_01 = 181,
    skirt_l_02 = 182,
    skirt_l_03 = 183,
    skirt_f_01 = 184,
    skirt_f_02 = 185,
    skirt_f_03 = 186,
    skirt_b_01 = 187,
    skirt_b_02 = 188,
    skirt_b_03 = 189,
    skirt_r_01 = 190,
    skirt_r_02 = 191,
    skirt_r_03 = 192,
    ik_hand_root = 193,
    ik_hand_gun = 194,
    ik_hand_r = 195,
    ik_hand_l = 196,
    ik_aim_root = 197,
    ik_aim_l = 198,
    ik_aim_r = 199,
    ik_foot_root = 200,
    ik_foot_l = 201,
    ik_foot_r = 202,
    camera_tpp = 203,
    ik_target_root = 204,
    ik_target_l = 205,
    ik_target_r = 206,
    VB_spine_03_spine_03 = 207,
    VB_upperarm_r_lowerarm_r = 208,
    VB_GunRef_spine_03 = 209,
    VB_camera_fpp_hand_l = 210,
    VB_camera_fpp_hand_r = 211,
    VB_camera_fpp_clavicle_l = 212,
    VB_camera_fpp_clavicle_r = 213,
    VB_lowerarm_l_hand_l = 214,
};

static std::array<int, magic_enum::enum_count<Bone>()> getBoneMapping() {
    std::array<int, magic_enum::enum_count<Bone>()> map{};
    map[static_cast<int>(Bone::head)] = static_cast<int>(EBoneIndex::forehead);
    map[static_cast<int>(Bone::neck)] = static_cast<int>(EBoneIndex::neck_01);
    map[static_cast<int>(Bone::leftShoulder)] = static_cast<int>(EBoneIndex::upperarm_l);
    map[static_cast<int>(Bone::rightShoulder)] = static_cast<int>(EBoneIndex::upperarm_r);
    map[static_cast<int>(Bone::leftElbow)] = static_cast<int>(EBoneIndex::lowerarm_l);
    map[static_cast<int>(Bone::rightElbow)] = static_cast<int>(EBoneIndex::lowerarm_r);
    map[static_cast<int>(Bone::leftHand)] = static_cast<int>(EBoneIndex::hand_l);
    map[static_cast<int>(Bone::rightHand)] = static_cast<int>(EBoneIndex::hand_r);
    map[static_cast<int>(Bone::spine)] = static_cast<int>(EBoneIndex::spine_01);
    map[static_cast<int>(Bone::hip)] = static_cast<int>(EBoneIndex::pelvis);
    map[static_cast<int>(Bone::leftHip)] = static_cast<int>(EBoneIndex::thigh_l);
    map[static_cast<int>(Bone::rightHip)] = static_cast<int>(EBoneIndex::thigh_r);
    map[static_cast<int>(Bone::leftKnee)] = static_cast<int>(EBoneIndex::calf_l);
    map[static_cast<int>(Bone::rightKnee)] = static_cast<int>(EBoneIndex::calf_r);
    map[static_cast<int>(Bone::leftFoot)] = static_cast<int>(EBoneIndex::foot_l);
    map[static_cast<int>(Bone::rightFoot)] = static_cast<int>(EBoneIndex::foot_r);

    return map;
}

int PUBG::Helper::getBoneIndex(Bone bone) {
    static auto boneMapping = getBoneMapping();
    return boneMapping[static_cast<int>(bone)];
}
