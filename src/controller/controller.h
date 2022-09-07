#ifndef ASSAULT_CUBE_HACKING_CONTROLLER_CONTROLLER_H
#define ASSAULT_CUBE_HACKING_CONTROLLER_CONTROLLER_H

#include <vector>
#include <functional>
#include <mutex>

class Controller {

    std::vector<std::function<void()>> guiCallbacks;

    std::mutex guiCallbacksMutex;

    std::vector<std::function<bool()>> fastLoopCallbacks;

    std::mutex fastLoopCallbacksMutex;

    std::condition_variable fastLoopCV;

public:
    static Controller &getInstance();

    bool addGuiCallback(const std::function<void()>& callback);

    void callGuiCallbacks();

    bool addFastLoopCallback(const std::function<bool()>& callback);

    void callFastLoopCallbacks();

};

#endif //ASSAULT_CUBE_HACKING_CONTROLLER_CONTROLLER_H
