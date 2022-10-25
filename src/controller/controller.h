#ifndef EVIL_CAT_CONTROLLER_CONTROLLER_H
#define EVIL_CAT_CONTROLLER_CONTROLLER_H

#include <vector>
#include <functional>
#include <mutex>
#include "singleton.h"

class Controller : public Singleton<Controller> {

    std::vector<std::function<void()>> serviceCallbacks;

    std::vector<std::pair<std::string, std::function<void()>>> menuCallbacks;

    std::vector<std::function<bool()>> fastLoopCallbacks;

    std::mutex fastLoopCallbacksMutex;

    std::condition_variable fastLoopCV;

public:
    void addServiceCallback(const std::function<void()> &callback);

    const std::vector<std::function<void()>> &getServiceCallbacks() const;

    void addMenuCallback(const std::string &name, const std::function<void()> &callback);

    const std::vector<std::pair<std::string, std::function<void()>>> &getMenuCallbacks() const;

    void addFastLoopCallback(const std::function<bool()> &callback);

    void callFastLoopCallbacks();

};

#endif //EVIL_CAT_CONTROLLER_CONTROLLER_H
