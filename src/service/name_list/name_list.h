#ifndef EVIL_CAT_SERVICE_NAME_LIST_NAME_LIST_H
#define EVIL_CAT_SERVICE_NAME_LIST_NAME_LIST_H

#include "service/service_interface.h"

namespace Settings::NameList {
    extern bool on;
}

class NameList : public ServiceInterface {
public:
    std::string getName() override;

    void menuCallback() override;

    void serviceCallback() override;
};

#endif //EVIL_CAT_SERVICE_NAME_LIST_NAME_LIST_H
