#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "player.h"
#include "item.h"
#include "offset.h"
#include "class_id.h"
#include "mem/memory_accessor.h"
#include "mem/buffer_pool.h"
#include "game.h"

struct EntityNode {
    uint32_t entity;
    uint32_t serialNumber;
    uint32_t prev;
    uint32_t next;
};

std::vector<uint32_t> Game::getEntityList() {
    std::vector<uint32_t> result;
    int maxTry = 1000;

    uint32_t begin = BufferPool::getInstance().getModuleAddress("client.dll")
                     + hazedumper::signatures::dwEntityList;
    uint32_t curr = begin;

    while (maxTry--) {
        EntityNode node = MemoryAccessor<EntityNode>{curr};
        if (node.entity && !isDormant(node.entity)) {
            result.emplace_back(node.entity);
        }
        if (node.next == begin ||
            node.prev == 0 ||
            node.next == 0 ||
            node.prev == node.next) {
            break;
        }
        curr = node.next;
    }

    return result;
}

uint32_t Game::getClassID(uint32_t entity) {
    return MemoryAccessor<uint32_t>{
            entity + 8,
            {8, 1, 20}
    };
}

void Game::getLocalPlayer(EntityContainer &container) {
    container.localPlayer = std::make_shared<LocalPlayer>(
            MemoryAccessor<gameptr_t>{
                    "client.dll", hazedumper::signatures::dwLocalPlayer,
            }
    );
}

void Game::getPlayersAndItems(EntityContainer &container) {
    for (uint32_t entity: getEntityList()) {
        uint32_t classID = getClassID(entity);
        if (classID == static_cast<uint32_t>(ClassID::CCSPlayer)) {
            container.players.emplace_back(std::make_shared<Player>(entity));
        } else if (std::string name = getItemName(classID); !name.empty()) {
            if (classID == static_cast<uint32_t>(ClassID::CPlantedC4)) {
                container.items.emplace_back(std::make_shared<PlantedC4>(entity, name));
                continue;
            }
            container.items.emplace_back(std::make_shared<Item>(entity, name));
        }
    }
}

EntityContainer Game::getEntities() {
    EntityContainer container;
    getLocalPlayer(container);
    getPlayersAndItems(container);

    return container;
}

glm::mat4 Game::getVPMatrix() {
    glm::mat4 mat = MemoryAccessor<glm::mat4>{
            "client.dll",
            hazedumper::signatures::dwViewMatrix
    };
    return glm::transpose(mat);
}

glm::vec3 Game::viewAngleToOrientation(glm::vec3 viewAngle) {
    glm::vec3 Z = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 X = glm::vec3(1.0f, 0.0f, 0.0f);
    float alpha = glm::radians(viewAngle.y);
    float beta = -glm::radians(viewAngle.x);
    glm::vec3 vXY = glm::normalize(glm::rotate(X, alpha, Z));
    glm::vec3 vCross = glm::normalize(glm::cross(vXY, Z));
    glm::vec3 vXYZ = glm::normalize(glm::rotate(vXY, beta, vCross));
    return vXYZ;
}

glm::vec3 Game::orientationToViewAngle(glm::vec3 orientation) {
    glm::vec3 Z = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 X = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 vXY = glm::normalize(glm::vec3(orientation.x, orientation.y, 0.0f));
    glm::vec3 vXYZ = glm::normalize(orientation);
    float alpha = glm::orientedAngle(X, vXY, Z);
    float beta = glm::angle(vXYZ, Z) - glm::pi<float>() / 2.0f;
    alpha = glm::clamp(glm::degrees(alpha), -180.0f, 180.0f);
    beta = glm::clamp(glm::degrees(beta), -89.0f, 89.0f);
    return {beta, alpha, 0.0f};
}

std::string Game::getItemName(uint32_t id) {
    const static std::map<ClassID, std::string> itemName = {
            // bomb
            {ClassID::CC4,                      "C4"},
            {ClassID::CPlantedC4,               "C4(planted)"},

            // weapon
            {ClassID::CAK47,                    "AK-47"},
            {ClassID::CDEagle,                  "Desert Eagle"},
            {ClassID::CWeaponAug,               "AUG"},
            {ClassID::CWeaponAWP,               "AWP"},
            {ClassID::CWeaponBizon,             "PP-Bizon"},
            {ClassID::CWeaponCycler,            "Cycler"},
            {ClassID::CWeaponElite,             "Dual Berettas"},
            {ClassID::CWeaponFamas,             "FAMAS"},
            {ClassID::CWeaponFiveSeven,         "Five-Seven"},
            {ClassID::CWeaponG3SG1,             "G3SG1"},
            {ClassID::CWeaponGalil,             "Galil"},
            {ClassID::CWeaponGalilAR,           "Galil AR"},
            {ClassID::CWeaponGlock,             "Glock-18"},
            {ClassID::CWeaponHKP2000,           "P2000"},
            {ClassID::CWeaponM249,              "M249"},
            {ClassID::CWeaponM3,                "M3"},
            {ClassID::CWeaponM4A1,              "M4A1"},
            {ClassID::CWeaponMAC10,             "MAC-10"},
            {ClassID::CWeaponMag7,              "MAG-7"},
            {ClassID::CWeaponMP5Navy,           "MP5-SD"},
            {ClassID::CWeaponMP7,               "MP7"},
            {ClassID::CWeaponMP9,               "MP9"},
            {ClassID::CWeaponNegev,             "Negev"},
            {ClassID::CWeaponNOVA,              "Nova"},
            {ClassID::CWeaponP228,              "P228"},
            {ClassID::CWeaponP250,              "P250"},
            {ClassID::CWeaponP90,               "P90"},
            {ClassID::CWeaponSawedoff,          "Sawed-Off"},
            {ClassID::CWeaponSCAR20,            "SCAR-20"},
            {ClassID::CWeaponScout,             "SSG 08"},
            {ClassID::CWeaponSG550,             "SG 550"},
            {ClassID::CWeaponSG552,             "SG 552"},
            {ClassID::CWeaponSG556,             "SG 556"},
            {ClassID::CWeaponShield,            "Shield"},
            {ClassID::CWeaponSSG08,             "Scout"},
            {ClassID::CWeaponTaser,             "Zeus x27"},
            {ClassID::CWeaponTec9,              "Tec-9"},
            {ClassID::CWeaponTMP,               "TMP"},
            {ClassID::CWeaponUMP45,             "UMP-45"},
            {ClassID::CWeaponUSP,               "USP-S"},
            {ClassID::CWeaponXM1014,            "XM1014"},
            {ClassID::CWeaponZoneRepulsor,      "ZoneRepulsor"},

            // grenade
            {ClassID::CBaseCSGrenade,           "Grenade"},
            {ClassID::CBaseCSGrenadeProjectile, "Grenade"},
            {ClassID::CDecoyGrenade,            "Decoy"},
            {ClassID::CDecoyProjectile,         "Decoy"},
            {ClassID::CFlashbang,               "Flashbang"},
            {ClassID::CHEGrenade,               "HE Grenade"},
            {ClassID::CIncendiaryGrenade,       "Incendiary"},
            {ClassID::CMolotovGrenade,          "Molotov"},
            {ClassID::CMolotovProjectile,       "Molotov"},
            {ClassID::CSmokeGrenade,            "Smoke"},
            {ClassID::CSmokeGrenadeProjectile,  "Smoke"},

            // hostage
            {ClassID::CHostage,                 "Hostage"},
    };

    if (auto it = itemName.find(static_cast<ClassID>(id));
            it != itemName.end()) {
        return it->second;
    } else {
        return {};
    }
}

bool Game::isDormant(uint32_t entity) {
    return MemoryAccessor<uint8_t>{entity + hazedumper::signatures::m_bDormant} != 0;
}
