#ifndef PUBG_OFFSET_H
#define PUBG_OFFSET_H

constexpr auto Offset_GObjects = 0x91CD920;
constexpr auto Offset_GWorld = 0x920EF60;
constexpr auto Offset_XenuineDecrypt = 0x7BFF628;
constexpr auto Offset_FNameEntry = 0x93F3838;
constexpr auto Offset_ChunkSize = 0x3E88;
constexpr auto Offset_ObjID = 0x10;

constexpr auto Offset_XorKey1 = 0x8926549;
constexpr auto Offset_XorKey2 = 0xDB6DBD5D;
constexpr auto Offset_RorValue = 0x3;
constexpr auto Offset_IsingRor = true;

constexpr auto Offset_CurrentLevel = 0x7C8;
constexpr auto Offset_Actors = 0x168;
constexpr auto Offset_ActorsForGC = 0x170;
constexpr auto Offset_GameInstance = 0x3A0;
constexpr auto Offset_LocalPlayers = 0x70;
constexpr auto Offset_PlayerController = 0x38;
constexpr auto Offset_ControlRotation = 0x41C;
constexpr auto Offset_AcknowledgedPawn = 0x480;
constexpr auto Offset_PlayerCameraManager = 0x4A0;

constexpr auto Offset_PlayerState = 0x428;
constexpr auto Offset_PlayerStatistics = 0xA58;
constexpr auto Offset_PubgIdData = 0xCB8;
constexpr auto Offset_SpectatedCount = 0x1ACC;
constexpr auto Offset_LastSubmitTime = 0x768;
constexpr auto Offset_LastRenderTimeOnScreen = 0x770;
constexpr auto Offset_Health = 0x870;
constexpr auto Offset_GroggyHealth = 0x23CC;
constexpr auto Offset_Team = 0x1038;
constexpr auto Offset_LastTeamNum = 0x1040;
constexpr auto Offset_LastSquadMemberIndex = 0x1048;
constexpr auto Offset_CharacterName = 0x2220;
constexpr auto Offset_AimOffsets = 0x16D0;

constexpr auto Offset_RootComponent = 0x360;
constexpr auto Offset_ComponentLocation = 0x330;

constexpr auto Offset_Mesh = 0x540;
constexpr auto Offset_StaticMesh = 0xAE0;
constexpr auto Offset_ComponentToWorld = 0x320;
constexpr auto Offset_BoneSpaceTransforms = 0xAE0;

constexpr auto Offset_CameraLocation = 0x454;
constexpr auto Offset_CameraRotation = 0x440;
constexpr auto Offset_CameraFov = 0x44C;
constexpr auto Offset_ViewTargetRotation = 0xA00;

constexpr auto Offset_ItemPackage_Items = 0x568;
constexpr auto Offset_ItemTable = 0xB0;
constexpr auto Offset_ItemID = 0x248;
constexpr auto Offset_DroppedItem_Item = 0x428;
constexpr auto Offset_OwnedComponents = 0x1E0;
constexpr auto Offset_DroppedItemGroup_Item = 0x748;

constexpr auto Offset_WeaponProcessor = 0x888;
constexpr auto Offset_EquippedWeapons = 0x208;
constexpr auto Offset_CurrentWeaponIndex = 0x309;
constexpr auto Offset_WeaponTrajectoryData = 0x1000;
constexpr auto Offset_TrajectoryGravityZ = 0xFC4;
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
