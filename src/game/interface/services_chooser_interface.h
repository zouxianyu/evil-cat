#ifndef EVIL_CAT_GAME_INTERFACE_SERVICES_CHOOSER_INTERFACE_H
#define EVIL_CAT_GAME_INTERFACE_SERVICES_CHOOSER_INTERFACE_H

#include <vector>
#include <memory>
#include "service/service_interface.h"

class ServicesChooserInterface {
public:
    virtual std::vector<std::unique_ptr<ServiceInterface>> getServices() = 0;

    virtual ~ServicesChooserInterface() = default;

};

#endif //EVIL_CAT_GAME_INTERFACE_SERVICES_CHOOSER_INTERFACE_H
