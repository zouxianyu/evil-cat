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
    const static std::unordered_set<std::string> list{
            "AIPawn_Base_Female_C",
            "AIPawn_Base_Male_C",
            "UltAIPawn_Base_Female_C",
            "UltAIPawn_Base_Male_C",
            "AIPawn_Base_Male_Pillar_C",
    };

    return list.find(objectName) != list.end();
}

bool PUBG::Helper::isRealPlayer(const std::string &objectName) {
    const static std::unordered_set<std::string> list{
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

static float normalizeAxis(float v) {
    while (v > 180.f) {
        v -= 360.f;
    }
    while (v < -180.f) {
        v += 360.f;
    }
    return v;
}

glm::vec3 PUBG::Helper::normalizeViewAngle(glm::vec3 viewAngle) {
    viewAngle.x = std::clamp(normalizeAxis(viewAngle.x), -75.f, 75.f);
    viewAngle.y = normalizeAxis(viewAngle.y);
    viewAngle.z = normalizeAxis(viewAngle.z);
    return viewAngle;
}

glm::vec2 PUBG::Helper::getMouseSensitivity() {
    uint64_t playerController = getPlayerController();
    uint64_t playerInput = MemoryAccessor<uint64_t>(
            playerController + Offset_PlayerInput
    );
    TArray axisProperties = MemoryAccessor<TArray>(
            playerInput + Offset_AxisProperties
    );
    glm::vec2 result{};
    using Element = std::array<float, 16>;
    for (int i = 0; i < axisProperties.max; ++i) {
        Element element = MemoryAccessor<Element>(
                axisProperties.ptr + i * 64
        );
        std::string name = PUBG::getName(*reinterpret_cast<int *>(element.data()));
        if (name == "MouseX") {
            result.x = element[4];
        } else if (name == "MouseY") {
            result.y = element[4];
        }
    }
    return result;
}

void PUBG::Helper::setCameraRotation(glm::vec3 rotation) {
    CameraInfo cameraInfo = getCameraInfo();
    glm::vec3 delta = normalizeViewAngle(rotation - cameraInfo.viewAngle);
    glm::vec2 sensitivity = getMouseSensitivity();
    float FOVRatio = 90.f / cameraInfo.FOV;
    glm::vec2 move = {
            delta.y / sensitivity.x * 10000.f * FOVRatio,
            -delta.x / sensitivity.y * 10000.f * FOVRatio
    };

    RECT rect;
    GetClientRect(GetForegroundWindow(), &rect);
    INPUT input{};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dx = static_cast<LONG>(move.x);
    input.mi.dy = static_cast<LONG>(move.y);
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

std::optional<ItemInfo> PUBG::Helper::getItemInfo(const std::string &objectName) {
    const static std::unordered_map<std::string, ItemInfo> map{
            ////////////////////////////////////////////////////////////////////
            // item

            //Ammo
            {"Item_Ammo_Mortar_C",                                          {"Shell",      4}},
            {"Item_Ammo_Bolt_C",                                            {"Bolt",       4}},
            {"Item_Ammo_Flare_C",                                           {"Flare",      4}},
            {"Item_Ammo_57mm_C",                                            {"5.7",        4}},
            {"Item_Ammo_300Magnum_C",                                       {"300",        4}},
            {"Item_Ammo_556mm_C",                                           {"5",          3}},
            {"Item_Ammo_762mm_C",                                           {"7",          3}},
            {"Item_Ammo_40mm_C",                                            {"40",         3}},
            {"Item_Ammo_9mm_C",                                             {"9",          2}},
            {"Item_Ammo_12Guage_C",                                         {"12",         2}},
            {"Item_Ammo_45ACP_C",                                           {"45",         2}},

            //Attach
            {"Item_Attach_Weapon_Lower_Foregrip_C",                         {"L_Fore",     3}},
            {"Item_Attach_Weapon_Lower_AngledForeGrip_C",                   {"L_Angle",    2}},
            {"Item_Attach_Weapon_Lower_HalfGrip_C",                         {"L_Half",     2}},
            {"Item_Attach_Weapon_Lower_LightweightForeGrip_C",              {"L_Light",    2}},
            {"Item_Attach_Weapon_Lower_ThumbGrip_C",                        {"L_Thumb",    2}},
            {"Item_Attach_Weapon_Lower_LaserPointer_C",                     {"L_Laser",    1}},

            {"Item_Attach_Weapon_Lower_QuickDraw_Large_Crossbow_C",         {"Q_CBow",     3}},

            {"Item_Attach_Weapon_Magazine_ExtendedQuickDraw_Large_C",       {"EQ_AR",      4}},
            {"Item_Attach_Weapon_Magazine_Extended_Large_C",                {"E_AR",       3}},
            {"Item_Attach_Weapon_Magazine_QuickDraw_Large_C",               {"Q_AR",       2}},

            {"Item_Attach_Weapon_Magazine_ExtendedQuickDraw_SniperRifle_C", {"EQ_SR",      4}},
            {"Item_Attach_Weapon_Magazine_Extended_SniperRifle_C",          {"E_SR",       3}},
            {"Item_Attach_Weapon_Magazine_QuickDraw_SniperRifle_C",         {"Q_SR",       2}},

            {"Item_Attach_Weapon_Magazine_ExtendedQuickDraw_Medium_C",      {"EQ_SMG",     3}},
            {"Item_Attach_Weapon_Magazine_Extended_Medium_C",               {"E_SMG",      4}},
            {"Item_Attach_Weapon_Magazine_QuickDraw_Medium_C",              {"Q_SMG",      1}},

            {"Item_Attach_Weapon_Muzzle_Compensator_Large_C",               {"C_AR",       3}},
            {"Item_Attach_Weapon_Muzzle_Compensator_Medium_C",              {"C_SMG",      2}},
            {"Item_Attach_Weapon_Muzzle_Compensator_SniperRifle_C",         {"C_SR",       2}},

            {"Item_Attach_Weapon_Muzzle_FlashHider_Large_C",                {"F_AR",       1}},
            {"Item_Attach_Weapon_Muzzle_FlashHider_Medium_C",               {"F_SMG",      1}},
            {"Item_Attach_Weapon_Muzzle_FlashHider_SniperRifle_C",          {"F_SR",       1}},

            {"Item_Attach_Weapon_Muzzle_Suppressor_SniperRifle_C",          {"S_SR",       4}},
            {"Item_Attach_Weapon_Muzzle_Suppressor_Large_C",                {"S_AR",       4}},
            {"Item_Attach_Weapon_Muzzle_Suppressor_Medium_C",               {"S_SMG",      3}},

            {"Item_Attach_Weapon_Muzzle_Choke_C",                           {"Choke",      2}},
            {"Item_Attach_Weapon_Muzzle_Duckbill_C",                        {"Duckbill",   2}},

            {"Item_Attach_Weapon_SideRail_DotSight_RMR_C",                  {"Canted",     4}},

            {"Item_Attach_Weapon_Stock_AR_Composite_C",                     {"ST_AR",      3}},
            {"Item_Attach_Weapon_Stock_SniperRifle_CheekPad_C",             {"ST_SR",      3}},
            {"Item_Attach_Weapon_Stock_UZI_C",                              {"ST_UZI",     2}},
            {"Item_Attach_Weapon_Stock_SniperRifle_BulletLoops_C",          {"Belt",       2}},

            {"Item_Attach_Weapon_Upper_PM2_01_C",                           {"15X",        4}},
            {"Item_Attach_Weapon_Upper_CQBSS_C",                            {"8X",         4}},
            {"Item_Attach_Weapon_Upper_Scope6x_C",                          {"6X",         3}},
            {"Item_Attach_Weapon_Upper_ACOG_01_C",                          {"4X",         3}},
            {"Item_Attach_Weapon_Upper_Scope3x_C",                          {"3X",         3}},
            {"Item_Attach_Weapon_Upper_Aimpoint_C",                         {"2X",         3}},
            {"Item_Attach_Weapon_Upper_DotSight_01_C",                      {"U_Dot",      3}},
            {"Item_Attach_Weapon_Upper_Holosight_C",                        {"U_Holo",     3}},

            {"Item_Back_BackupParachute_C",                                 {"Parachute",  3}},
            {"Item_Armor_C_01_Lv3_C",                                       {"Vest3",      4}},
            {"Item_Armor_D_01_Lv2_C",                                       {"Vest2",      3}},
            {"Item_Armor_E_01_Lv1_C",                                       {"Vest1",      2}},
            {"Item_Back_B_08_Lv3_C",                                        {"Back3",      4}},
            {"Item_Back_C_01_Lv3_C",                                        {"Back3",      4}},
            {"Item_Back_C_02_Lv3_C",                                        {"Back3",      4}},
            {"Item_Back_F_01_Lv2_C",                                        {"Back2",      3}},
            {"Item_Back_F_02_Lv2_C",                                        {"Back2",      3}},
            {"Item_Back_E_01_Lv1_C",                                        {"Back1",      2}},
            {"Item_Back_E_02_Lv1_C",                                        {"Back1",      2}},
            {"Item_Back_BlueBlocker",                                       {"JammerPack", 4}},
            {"Item_Ghillie_01_C",                                           {"GhillieG",   4}},
            {"Item_Ghillie_02_C",                                           {"GhillieB",   4}},
            {"Item_Ghillie_03_C",                                           {"GhillieB",   4}},
            {"Item_Ghillie_04_C",                                           {"GhillieW",   4}},
            {"Item_Ghillie_05_C",                                           {"CamoSuit",   4}},
            {"Item_Ghillie_06_C",                                           {"GhillieD",   4}},
            {"Item_Head_E_01_Lv1_C",                                        {"Head1",      2}},
            {"Item_Head_E_02_Lv1_C",                                        {"Head1",      2}},
            {"Item_Head_F_01_Lv2_C",                                        {"Head2",      3}},
            {"Item_Head_F_02_Lv2_C",                                        {"Head2",      3}},
            {"Item_Head_G_01_Lv3_C",                                        {"Head3",      4}},

            //Throw
            {"Item_Weapon_C4_C",                                            {"C4",         4}},
            {"Item_Weapon_BluezoneGrenade_C",                               {"BlueBomb",   3}},
            {"Item_Weapon_Grenade_C",                                       {"Grenade",    3}},
            {"Item_Weapon_Grenade_Warmode_C",                               {"Grenade",    3}},
            {"Item_Weapon_FlashBang_C",                                     {"Flash",      3}},
            {"Item_Weapon_StickyGrenade_C",                                 {"Bomb",       3}},
            {"Item_Weapon_Molotov_C",                                       {"Molotov",    3}},
            {"Item_Weapon_SmokeBomb_C",                                     {"Smoke",      3}},
            {"Item_Weapon_SpikeTrap_C",                                     {"Trap",       3}},
            {"Item_Weapon_DecoyGrenade_C",                                  {"Decoy",      1}},

            //Melee
            {"Item_Weapon_Pan_C",                                           {"Pan",        3}},
            {"Item_Weapon_Cowbar_C",                                        {"Crawbar",    1}},
            {"Item_Weapon_Machete_C",                                       {"Machete",    1}},
            {"Item_Weapon_Sickle_C",                                        {"Sickle",     1}},

            //AR
            {"Item_Weapon_Groza_C",                                         {"Groza",      4}},
            {"Item_Weapon_BerylM762_C",                                     {"Beryl",      4}},
            {"Item_Weapon_ACE32_C",                                         {"ACE",        4}},
            {"Item_Weapon_HK416_C",                                         {"M4",         4}},
            {"Item_Weapon_Duncans_M416_C",                                  {"M4",         4}},
            {"Item_Weapon_AUG_C",                                           {"AUG",        4}},
            {"Item_Weapon_AK47_C",                                          {"AK",         3}},
            {"Item_Weapon_Lunchmeats_AK47_C",                               {"AK",         3}},
            {"Item_Weapon_SCAR-L_C",                                        {"SCAR",       3}},
            {"Item_Weapon_G36C_C",                                          {"G36C",       3}},
            {"Item_Weapon_QBZ95_C",                                         {"QBZ",        3}},
            {"Item_Weapon_K2_C",                                            {"K2",         3}},
            {"Item_Weapon_Mk47Mutant_C",                                    {"Mutant",     2}},
            {"Item_Weapon_M16A4_C",                                         {"M16",        2}},

            //LMG
            {"Item_Weapon_MG3_C",                                           {"MG3",        4}},
            {"Item_Weapon_DP28_C",                                          {"DP28",       2}},
            {"Item_Weapon_M249_C",                                          {"M249",       2}},

            //SR
            {"Item_Weapon_L6_C",                                            {"Lynx",       4}},
            {"Item_Weapon_AWM_C",                                           {"AWM",        4}},
            {"Item_Weapon_M24_C",                                           {"M24",        3}},
            {"Item_Weapon_Julies_Kar98k_C",                                 {"K98",        3}},
            {"Item_Weapon_Kar98k_C",                                        {"K98",        3}},
            {"Item_Weapon_Mosin_C",                                         {"Mosin",      3}},
            {"Item_Weapon_Win1894_C",                                       {"Win94",      1}},

            //DMR
            {"Item_Weapon_Mk14_C",                                          {"Mk14",       4}},
            {"Item_Weapon_FNFal_C",                                         {"SLR",        4}},
            {"Item_Weapon_Mk12_C",                                          {"Mk12",       3}},
            {"Item_Weapon_SKS_C",                                           {"SKS",        3}},
            {"Item_Weapon_Mads_QBU88_C",                                    {"QBU",        3}},
            {"Item_Weapon_QBU88_C",                                         {"QBU",        3}},
            {"Item_Weapon_Mini14_C",                                        {"Mini",       3}},
            {"Item_Weapon_VSS_C",                                           {"VSS",        3}},

            //SG
            {"Item_Weapon_DP12_C",                                          {"DBS",        4}},
            {"Item_Weapon_Saiga12_C",                                       {"S12K",       3}},
            {"Item_Weapon_Winchester_C",                                    {"S1897",      2}},
            {"Item_Weapon_Berreta686_C",                                    {"S686",       2}},

            //Pistol
            {"Item_Weapon_G18_C",                                           {"P18C",       4}},
            {"Item_Weapon_DesertEagle_C",                                   {"Deagle",     3}},
            {"Item_Weapon_Rhino_C",                                         {"R45",        3}},
            {"Item_Weapon_NagantM1895_C",                                   {"R1895",      3}},
            {"Item_Weapon_vz61Skorpion_C",                                  {"Skorpion",   3}},
            {"Item_Weapon_M1911_C",                                         {"P1911",      2}},
            {"Item_Weapon_M9_C",                                            {"P92",        2}},
            {"Item_Weapon_Sawnoff_C",                                       {"Sawnoff",    1}},

            //SMG
            {"Item_Weapon_P90_C",                                           {"P90",        4}},
            {"Item_Weapon_Vector_C",                                        {"Vec",        3}},
            {"Item_Weapon_UZI_C",                                           {"UZI",        3}},
            {"Item_Weapon_UMP_C",                                           {"UMP",        3}},
            {"Item_Weapon_Thompson_C",                                      {"Tom",        2}},
            {"Item_Weapon_BizonPP19_C",                                     {"Bizon",      2}},
            {"Item_Weapon_MP5K_C",                                          {"MP5K",       2}},

            //Special Weapon
            {"Item_Weapon_Mortar_C",                                        {"Mortar",     4}},
            {"Item_Weapon_Crossbow_C",                                      {"CBow",       4}},
            {"Item_Weapon_FlareGun_C",                                      {"FlareGun",   4}},
            {"Item_Weapon_M79_C",                                           {"M79",        4}},
            {"Item_Weapon_PanzerFaust100M_C",                               {"Rocket",     4}},

            //Boost
            {"Item_Boost_AdrenalineSyringe_C",                              {"Syringe",    3}},
            {"Item_Boost_EnergyDrink_C",                                    {"Drink",      3}},
            {"Item_Boost_PainKiller_C",                                     {"Drug",       3}},

            //Heal
            {"Item_Heal_MedKit_C",                                          {"MedKit",     4}},
            {"Item_Heal_FirstAid_C",                                        {"FirstAid",   3}},
            {"Item_Heal_Bandage_C",                                         {"Band",       2}},

            //Key
            {"Item_Heaven_Key_C",                                           {"Key",        4}},
            {"Item_Chimera_Key_C",                                          {"Key",        4}},
            {"Item_Tiger_Key_C",                                            {"Key",        4}},

            {"Item_JerryCan_C",                                             {"Fuel",       4}},
            {"Item_EmergencyPickup_C",                                      {"Pickup",     4}},
            {"InstantRevivalKit_C",                                         {"Revival",    4}},
            {"Item_Tiger_SelfRevive_C",                                     {"AED",        4}},
            {"Item_Mountainbike_C",                                         {"Bike",       4}},
            {"Item_Weapon_Drone_C",                                         {"Drone",      4}},
            {"Item_Weapon_TraumaBag_C",                                     {"TraumaBag",  4}},
            {"Item_Weapon_Spotter_Scope_C",                                 {"Spotter",    4}},
            {"Item_Weapon_TacPack_C",                                       {"TacPack",    4}},

            ////////////////////////////////////////////////////////////////////
            // object

            {"ProjGrenade_C",                                               {"Grenade",    4}},
            {"ProjFlashBang_C",                                             {"Flash",      4}},
            {"ProjMolotov_C",                                               {"Molotov",    4}},
            {"ProjSmokeBomb_v2_C",                                          {"Smoke",      4}},
            {"ProjSpiketrap_C",                                             {"Spike",      4}},
            {"BP_Spiketrap_C",                                              {"Spike",      4}},
            {"ProjStickyGrenade_C",                                         {"Bomb",       4}},
            {"ProjC4_C",                                                    {"C4",         4}},
            {"ProjDecoyGrenade_C",                                          {"Decoy",      4}},
            {"BP_Projectile_40mm_Smoke_C",                                  {"Smoke",      4}},
            {"ProjBluezoneBomb_C",                                          {"BlueBomb",   4}},
            {"BP_Drone_C",                                                  {"Drone",      4}},
            {"WeapMortar_C",                                                {"Mortar",     4}},

            {"WeapMacheteProjectile_C",                                     {"Machete",    4}},
            {"WeapCowbarProjectile_C",                                      {"Cowbar",     4}},
            {"WeapPanProjectile_C",                                         {"Pan",        4}},
            {"WeapSickleProjectile_C",                                      {"Sickle",     4}},

            {"PanzerFaust100M_Projectile_C",                                {"Rocket",     4}},
            {"Mortar_Projectile_C",                                         {"Shell",      4}},
            {"Flare_Projectile_C",                                          {"Flare",      4}},

            {"BP_KillTruck_C",                                              {"KillTruck",  4}},
            {"BP_Helicopter_C",                                             {"Helicopter", 4}},

            {"BP_Vending_machine_1_C",                                      {"Vendor",     4}},

            ////////////////////////////////////////////////////////////////////
            // vehicle

            {"BP_EmPickup_Aircraft_C",                                      {"Pickup",     4}},
            {"BP_EmergencyPickupVehicle_C",                                 {"Pickup",     4}},
            {"TransportAircraft_Chimera_C",                                 {"Aircraft",   4}},
            {"BP_Bicycle_C",                                                {"Bike",       4}},

            {"BP_BRDM_C",                                                   {"BRDM",       4}},
            {"Uaz_Armored_C",                                               {"UAZ",        4}},
            {"BP_Mirado_Open_05_C",                                         {"MiradoG",    4}},
            {"BP_Motorglider_C",                                            {"Glider",     4}},
            {"BP_Motorglider_Blue_C",                                       {"Glider",     4}},
            {"BP_Motorglider_Green_C",                                      {"Glider",     4}},
            {"BP_Motorglider_Orange_C",                                     {"Glider",     4}},
            {"BP_Motorglider_Red_C",                                        {"Glider",     4}},
            {"BP_Motorglider_Teal_C",                                       {"Glider",     4}},
            {"BP_LootTruck_C",                                              {"LootTruck",  4}},

            {"AquaRail_A_01_C",                                             {"Aquarail",   4}},
            {"AquaRail_A_02_C",                                             {"Aquarail",   4}},
            {"AquaRail_A_03_C",                                             {"Aquarail",   4}},
            {"Boat_PG117_C",                                                {"Boat",       4}},
            {"PG117_A_01_C",                                                {"Boat",       4}},

            {"BP_M_Rony_A_01_C",                                            {"Rony",       4}},
            {"BP_M_Rony_A_02_C",                                            {"Rony",       4}},
            {"BP_M_Rony_A_03_C",                                            {"Rony",       4}},
            {"BP_Mirado_C",                                                 {"Mirado",     4}},
            {"BP_Mirado_A_01_C",                                            {"Mirado",     4}},
            {"BP_Mirado_A_02_C",                                            {"Mirado",     4}},
            {"BP_Mirado_A_03_C",                                            {"Mirado",     4}},
            {"BP_Mirado_A_03_Esports_C",                                    {"Mirado",     4}},
            {"BP_Mirado_A_04_C",                                            {"Mirado",     4}},
            {"BP_Mirado_Open_C",                                            {"MiradoO",    4}},
            {"BP_Mirado_Open_01_C",                                         {"MiradoO",    4}},
            {"BP_Mirado_Open_02_C",                                         {"MiradoO",    4}},
            {"BP_Mirado_Open_03_C",                                         {"MiradoO",    4}},
            {"BP_Mirado_Open_04_C",                                         {"MiradoO",    4}},
            {"BP_Motorbike_04_C",                                           {"Motor",      4}},
            {"BP_Motorbike_04_Desert_C",                                    {"Motor",      4}},
            {"BP_Motorbike_Solitario_C",                                    {"Motor",      4}},
            {"BP_Motorbike_04_SideCar_C",                                   {"MotorS",     4}},
            {"BP_Motorbike_04_SideCar_Desert_C",                            {"MotorS",     4}},
            {"BP_Niva_01_C",                                                {"Zima",       4}},
            {"BP_Niva_02_C",                                                {"Zima",       4}},
            {"BP_Niva_03_C",                                                {"Zima",       4}},
            {"BP_Niva_04_C",                                                {"Zima",       4}},
            {"BP_Niva_05_C",                                                {"Zima",       4}},
            {"BP_Niva_06_C",                                                {"Zima",       4}},
            {"BP_Niva_07_C",                                                {"Zima",       4}},
            {"BP_Niva_Esports_C",                                           {"Zima",       4}},
            {"BP_PickupTruck_A_01_C",                                       {"Truck",      4}},
            {"BP_PickupTruck_A_02_C",                                       {"Truck",      4}},
            {"BP_PickupTruck_A_03_C",                                       {"Truck",      4}},
            {"BP_PickupTruck_A_04_C",                                       {"Truck",      4}},
            {"BP_PickupTruck_A_05_C",                                       {"Truck",      4}},
            {"BP_PickupTruck_A_esports_C",                                  {"Truck",      4}},
            {"BP_PickupTruck_B_01_C",                                       {"TruckO",     4}},
            {"BP_PickupTruck_B_02_C",                                       {"TruckO",     4}},
            {"BP_PickupTruck_B_03_C",                                       {"TruckO",     4}},
            {"BP_PickupTruck_B_04_C",                                       {"TruckO",     4}},
            {"BP_PickupTruck_B_05_C",                                       {"TruckO",     4}},
            {"BP_TukTukTuk_A_01_C",                                         {"Tukshai",    4}},
            {"BP_TukTukTuk_A_02_C",                                         {"Tukshai",    4}},
            {"BP_TukTukTuk_A_03_C",                                         {"Tukshai",    4}},
            {"BP_Van_A_01_C",                                               {"Bus",        4}},
            {"BP_Van_A_02_C",                                               {"Bus",        4}},
            {"BP_Van_A_03_C",                                               {"Bus",        4}},
            {"BP_MiniBus_C",                                                {"Bus",        4}},
            {"BP_Scooter_01_A_C",                                           {"Scooter",    4}},
            {"BP_Scooter_02_A_C",                                           {"Scooter",    4}},
            {"BP_Scooter_03_A_C",                                           {"Scooter",    4}},
            {"BP_Scooter_04_A_C",                                           {"Scooter",    4}},
            {"BP_Snowbike_01_C",                                            {"Snowmobile", 4}},
            {"BP_Snowbike_02_C",                                            {"Snowmobile", 4}},
            {"BP_Snowmobile_01_C",                                          {"Snowmobile", 4}},
            {"BP_Snowmobile_02_C",                                          {"Snowmobile", 4}},
            {"BP_Snowmobile_03_C",                                          {"Snowmobile", 4}},
            {"Buggy_A_01_C",                                                {"Buggy",      4}},
            {"Buggy_A_02_C",                                                {"Buggy",      4}},
            {"Buggy_A_03_C",                                                {"Buggy",      4}},
            {"Buggy_A_04_C",                                                {"Buggy",      4}},
            {"Buggy_A_05_C",                                                {"Buggy",      4}},
            {"Buggy_A_06_C",                                                {"Buggy",      4}},
            {"Dacia_A_01_v2_C",                                             {"Dacia",      4}},
            {"Dacia_A_01_v2_snow_C",                                        {"Dacia",      4}},
            {"Dacia_A_02_v2_C",                                             {"Dacia",      4}},
            {"Dacia_A_03_v2_C",                                             {"Dacia",      4}},
            {"Dacia_A_03_v2_Esports_C",                                     {"Dacia",      4}},
            {"Dacia_A_04_v2_C",                                             {"Dacia",      4}},
            {"Uaz_A_01_C",                                                  {"UAZO",       4}},
            {"Uaz_B_01_C",                                                  {"UAZ",        4}},
            {"Uaz_B_01_esports_C",                                          {"UAZ",        4}},
            {"Uaz_C_01_C",                                                  {"UAZ",        4}},
            {"BP_Dirtbike_C",                                               {"DBike",      4}},
            {"BP_CoupeRB_C",                                                {"Coupe",      4}},
            {"BP_ATV_C",                                                    {"Quad",       4}},
            {"BP_PonyCoupe_C",                                              {"Pony",       4}},
            {"BP_Porter_C",                                                 {"Porter",     4}},

            ////////////////////////////////////////////////////////////////////
            // package

            {"DeathDropItemPackage_C",                                      {"Box",        4}},
            {"Carapackage_RedBox_C",                                        {"Supply",     4}},
            {"Carapackage_FlareGun_C",                                      {"Flare",      4}},
            {"BP_Loot_AmmoBox_C",                                           {"Ammo",       4}},
            {"BP_Loot_MilitaryCrate_C",                                     {"Crate",      4}},
            {"BP_Loot_TruckHaul_C",                                         {"LootTruck",  4}},
            {"Carapackage_SmallPackage_C",                                  {"Small",      4}},
            {"BP_ItemPackage_GuardedLoot_Heaven_C",                         {"Pillar",     4}},
    };

    if (auto it = map.find(objectName); it != map.end()) {
        return it->second;
    } else {
        return std::nullopt;
    }
}
