#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_NAME_LIST_NAME_LIST_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_NAME_LIST_NAME_LIST_H

namespace Settings::NameList {
    extern bool on;
}

class NameList {
public:
    static NameList &getInstance();

    void callback();
};

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_NAME_LIST_NAME_LIST_H
