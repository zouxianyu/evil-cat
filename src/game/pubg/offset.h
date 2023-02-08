#ifndef PUBG_OFFSET_H
#define PUBG_OFFSET_H

constexpr auto Offset_GObjects = 0x92C9C20;
constexpr auto Offset_GWorld = 0x930B260;
constexpr auto Offset_XenuineDecrypt = 0x7CE0928;
constexpr auto Offset_FNameEntry = 0x94EFAA8;
constexpr auto Offset_ChunkSize = 0x3F94;
constexpr auto Offset_ObjID = 0x10;

constexpr auto Offset_XorKey1 = 0xEFDD1981;
constexpr auto Offset_XorKey2 = 0x5CF601D7;
constexpr auto Offset_RorValue = 0xF;
constexpr auto Offset_IsingRor = false;

constexpr auto Offset_CurrentLevel = 0x878;
constexpr auto Offset_Actors = 0x118;
constexpr auto Offset_ActorsForGC = 0x250;
constexpr auto Offset_GameInstance = 0x7F8;
constexpr auto Offset_LocalPlayers = 0x40;
constexpr auto Offset_PlayerController = 0x30;
constexpr auto Offset_ControlRotation = 0x430;
constexpr auto Offset_AcknowledgedPawn = 0x490;
constexpr auto Offset_PlayerCameraManager = 0x4B0;
constexpr auto Offset_PlayerInput = 0x528;
constexpr auto Offset_AxisProperties = 0x170;

constexpr auto Offset_PlayerState = 0x430;
constexpr auto Offset_PlayerStatistics = 0x828;
constexpr auto Offset_PubgIdData = 0xC98;
constexpr auto Offset_SpectatedCount = 0x21E0;
constexpr auto Offset_LastSubmitTime = 0x758;
constexpr auto Offset_LastRenderTimeOnScreen = 0x760;
constexpr auto Offset_Health = 0x870;
constexpr auto Offset_GroggyHealth = 0x197C;
constexpr auto Offset_Team = 0x1258;
constexpr auto Offset_LastTeamNum = 0x1260;
constexpr auto Offset_LastSquadMemberIndex = 0x1268;
constexpr auto Offset_CharacterName = 0x2178;
constexpr auto Offset_AimOffsets = 0x1570;

constexpr auto Offset_RootComponent = 0x128;
constexpr auto Offset_ComponentLocation = 0x310;

constexpr auto Offset_Mesh = 0x558;
constexpr auto Offset_StaticMesh = 0xAD0;
constexpr auto Offset_ComponentToWorld = 0x300;
constexpr auto Offset_BoneSpaceTransforms = 0xAD0;

constexpr auto Offset_CameraLocation = 0xA58;
constexpr auto Offset_CameraRotation = 0xA64;
constexpr auto Offset_CameraFov = 0xA4C;
constexpr auto Offset_ViewTargetRotation = 0x494;

constexpr auto Offset_ItemPackage_Items = 0x570;
constexpr auto Offset_ItemTable = 0xA8;
constexpr auto Offset_ItemID = 0x248;
constexpr auto Offset_DroppedItem_Item = 0x430;
constexpr auto Offset_OwnedComponents = 0x2A0;
constexpr auto Offset_DroppedItemGroup_Item = 0x738;

constexpr auto Offset_WeaponProcessor = 0x888;
constexpr auto Offset_EquippedWeapons = 0x1F8;
constexpr auto Offset_CurrentWeaponIndex = 0x2F9;
constexpr auto Offset_WeaponTrajectoryData = 0x1000;
constexpr auto Offset_TrajectoryGravityZ = 0xFD4;
constexpr auto Offset_TrajectoryConfig = 0x100;
constexpr auto Offset_BallisticCurve = 0x10;
constexpr auto Offset_FloatCurves = 0x30;
constexpr auto Offset_RichCurve_Keys = 0x60;

constexpr auto Offset_AnimScriptInstance = 0xC70;
constexpr auto Offset_ControlRotation_CP = 0x76C;
constexpr auto Offset_RecoilADSRotation_CP = 0x9CC;
constexpr auto Offset_LeanLeftAlpha_CP = 0xDE4;
constexpr auto Offset_LeanRightAlpha_CP = 0xDE8;
constexpr auto Offset_bLocalFPP_CP = 0xF28;
constexpr auto Offset_bIsScoping_CP = 0xCF6;
constexpr auto Offset_bIsReloading_CP = 0x8B8;

#endif //PUBG_OFFSET_H
