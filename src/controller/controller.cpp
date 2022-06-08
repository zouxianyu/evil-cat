#include <vector>
#include <functional>
#include "controller.h"

Controller &Controller::getInstance() {
    static Controller instance;
    return instance;
}

bool Controller::registerCallback(std::function<bool()> condition, std::function<void()> callback) {
    callbackList.emplace_back(condition, callback);
    return true;
}

void Controller::callCallback() {
    for (const auto&[condition, callback]: callbackList) {
        if (condition()) {
            callback();
        }
    }
}



