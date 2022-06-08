#ifndef ASSAULT_CUBE_HACKING_CONTROLLER_CONTROLLER_H
#define ASSAULT_CUBE_HACKING_CONTROLLER_CONTROLLER_H

#include <vector>
#include <functional>

#define ON_CONDITION(condition) []()->bool{return condition;}
#define ON_FLIP(trigger) []()->bool{static bool t = trigger; if (t != trigger) {t = trigger; return true;} else return false;}
#define ON_ALWAYS []()->bool{return true;}

class Controller {

    std::vector<std::pair<std::function<bool()>, std::function<void()>>> callbackList;

public:
    static Controller &getInstance();

    bool registerCallback(std::function<bool()> condition, std::function<void()> callback);

    void callCallback();
};

#endif //ASSAULT_CUBE_HACKING_CONTROLLER_CONTROLLER_H
