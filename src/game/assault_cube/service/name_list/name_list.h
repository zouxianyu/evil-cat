#ifndef ASSAULT_CUBE_HACKING_SRC_GAME_ASSAULT_CUBE_SERVICE_NAME_LIST_NAME_LIST_H
#define ASSAULT_CUBE_HACKING_SRC_GAME_ASSAULT_CUBE_SERVICE_NAME_LIST_NAME_LIST_H

namespace Settings::NameList {
    extern bool on;
}

class NameList {
public:
    static NameList &getInstance();

    void callback();
};

#endif //ASSAULT_CUBE_HACKING_SRC_GAME_ASSAULT_CUBE_SERVICE_NAME_LIST_NAME_LIST_H
