#ifndef EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_NAME_LIST_NAME_LIST_H
#define EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_NAME_LIST_NAME_LIST_H

#include "service/service_interface.h"

namespace Settings::NameList {
    extern bool on;
}

class NameList : public ServiceInterface {
public:
    void callback() override;

};

#endif //EVIL_CAT_GAME_ASSAULT_CUBE_SERVICE_NAME_LIST_NAME_LIST_H
