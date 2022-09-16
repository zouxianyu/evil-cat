#ifndef EVIL_CAT_CONTROLLER_CONTROLLER_H
#define EVIL_CAT_CONTROLLER_CONTROLLER_H

#include <vector>
#include <functional>
#include <mutex>
#include "singleton.h"

class Controller : public Singleton<Controller> {

    std::vector<std::function<void()>> guiCallbacks;

    std::mutex guiCallbacksMutex;

    std::vector<std::function<bool()>> fastLoopCallbacks;

    std::mutex fastLoopCallbacksMutex;

    std::condition_variable fastLoopCV;

public:
    bool addGuiCallback(const std::function<void()>& callback);

    void callGuiCallbacks();

    bool addFastLoopCallback(const std::function<bool()>& callback);

    void callFastLoopCallbacks();

};

#endif //EVIL_CAT_CONTROLLER_CONTROLLER_H
