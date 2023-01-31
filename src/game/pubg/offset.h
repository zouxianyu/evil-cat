#ifndef PUBG_OFFSET_H
#define PUBG_OFFSET_H

constexpr auto Offset_GObjects = 0x9237920;
constexpr auto Offset_GWorld = 0x9278F60;
constexpr auto Offset_XenuineDecrypt = 0x7C4A628;
constexpr auto Offset_FNameEntry = 0x945D738;
constexpr auto Offset_ChunkSize = 0x3EF4;
constexpr auto Offset_ObjID = 0x8;

constexpr auto Offset_XorKey1 = 0x1A018DF6;
constexpr auto Offset_XorKey2 = 0xF7978B7A;
constexpr auto Offset_RorValue = 0xA;
constexpr auto Offset_IsingRor = true;

constexpr auto Offset_CurrentLevel = 0x9A8;
constexpr auto Offset_Actors = 0x218;
constexpr auto Offset_ActorsForGC = 0x230;
constexpr auto Offset_GameInstance = 0x338;
constexpr auto Offset_LocalPlayers = 0xB0;
constexpr auto Offset_PlayerController = 0x38;
constexpr auto Offset_ControlRotation = 0x458;
constexpr auto Offset_AcknowledgedPawn = 0x488;
constexpr auto Offset_PlayerCameraManager = 0x4A8;

constexpr auto Offset_PlayerState = 0x428;
constexpr auto Offset_PlayerStatistics = 0x870;
constexpr auto Offset_PubgIdData = 0xCB0;
constexpr auto Offset_SpectatedCount = 0x1B14;
constexpr auto Offset_LastSubmitTime = 0x768;
constexpr auto Offset_LastRenderTimeOnScreen = 0x770;
constexpr auto Offset_Health = 0x870;
constexpr auto Offset_GroggyHealth = 0x1BEC;
constexpr auto Offset_Team = 0x1AE0;
constexpr auto Offset_LastTeamNum = 0x1AE8;
constexpr auto Offset_LastSquadMemberIndex = 0x1AF0;
constexpr auto Offset_CharacterName = 0xE50;
constexpr auto Offset_AimOffsets = 0x1700;

constexpr auto Offset_RootComponent = 0x340;
constexpr auto Offset_ComponentLocation = 0x220;

constexpr auto Offset_Mesh = 0x458;
constexpr auto Offset_StaticMesh = 0xAE0;
constexpr auto Offset_ComponentToWorld = 0x210;
constexpr auto Offset_BoneSpaceTransforms = 0xAE0;
constexpr auto Offset_CachedBoneSpaceTransforms = 0xCE8;

constexpr auto Offset_CameraLocation = 0x464;
constexpr auto Offset_CameraRotation = 0xA00;
constexpr auto Offset_CameraFov = 0xA10;
constexpr auto Offset_ViewTargetRotation = 0x16E0;

constexpr auto Offset_ItemPackage_Items = 0x568;
constexpr auto Offset_ItemTable = 0xB0;
constexpr auto Offset_ItemID = 0x248;
constexpr auto Offset_DroppedItem_Item = 0x428;
constexpr auto Offset_OwnedComponents = 0x138;
constexpr auto Offset_DroppedItemGroup_Item = 0x748;

constexpr auto Offset_WeaponProcessor = 0x888;
constexpr auto Offset_EquippedWeapons = 0x208;
constexpr auto Offset_CurrentWeaponIndex = 0x45B; // or 0x458 ?
constexpr auto Offset_WeaponTrajectoryData = 0x1000;
constexpr auto Offset_TrajectoryGravityZ = 0xFF4;
constexpr auto Offset_TrajectoryConfig = 0x108;
constexpr auto Offset_BallisticCurve = 0x10;
constexpr auto Offset_FloatCurves = 0x38;

constexpr auto Offset_AnimScriptInstance = 0xC80;
constexpr auto Offset_ControlRotation_CP = 0x774;
constexpr auto Offset_RecoilADSRotation_CP = 0x9CC;
constexpr auto Offset_LeanLeftAlpha_CP = 0xDE4;
constexpr auto Offset_LeanRightAlpha_CP = 0xDE8;
constexpr auto Offset_bLocalFPP_CP = 0xF28;
constexpr auto Offset_bIsScoping_CP = 0xCF6;
constexpr auto Offset_bIsReloading_CP = 0x8C0;

#endif //PUBG_OFFSET_H
