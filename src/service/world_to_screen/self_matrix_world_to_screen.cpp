#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <imgui/imgui.h>
#include "service/esp/esp.h"
#include "entity/entity_manager.h"
#include "self_matrix_world_to_screen.h"


SelfMatrixWorldToScreen &SelfMatrixWorldToScreen::getInstance() {
    static SelfMatrixWorldToScreen instance;
    return instance;
}

glm::mat4 SelfMatrixWorldToScreen::getViewProjectionMatrix() {
    Player localPlayer = EntityManager::getInstance().getLocalPlayer();
    Vec3 viewAngle = localPlayer.getViewAngle();
    Vec3 camara = localPlayer.getHead();

    float near = .1f; //nearest distance from which you can see
    float far = 100000.0f; //you cant see more
    float fov = glm::radians(70.f); //field of view
//    float width = ImGui::GetWindowWidth();
//    float height = ImGui::GetWindowHeight();
    float width = 1024;
    float height = 768;
    float aspect = width / height;

    glm::mat4 projection = glm::perspective(fov, aspect, near, far);

//    glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); //rotate around x
//    glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); //rotate around y
//    glm::mat4 rotate_z = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); //rotate around z
//
//    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(camara.x, camara.y, camara.z)); //make translation matrix
//    glm::mat4 model = translate * rotate_z * rotate_y * rotate_x; //here will be stored the model matrix for camera, mult in this order!
//    glm::mat4 view = glm::inverse(model); //unoptimalized for cpu but ez to implement in your hack
//    return view;

    Vec3 towards = Esp::getInstance().viewAngleToOrientation(viewAngle);
    glm::vec3 glmTowards{towards.x, towards.y, towards.z};
    glm::vec3 glmCamara{camara.x, camara.y, camara.z};
    glm::vec3 glmCenter = glmCamara + glmTowards;
    glm::vec3 glmZ{0.0f, 0.0f, 1.0f};
    glm::vec3 cross = glm::normalize(glm::cross(glmTowards, glmZ));
    glm::mat4 upTranslation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), cross);
    glm::vec3 glmUp = upTranslation * glm::vec4(glmTowards.x, glmTowards.y, glmTowards.z, 0.f);
    glmUp = glm::normalize(glmUp);
//    glm::vec3 glmUp = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::mat4 lookAt = glm::lookAt(glmCamara, glmCenter, glmUp);
    return projection * lookAt;
}
