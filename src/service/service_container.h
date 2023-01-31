#ifndef EVIL_CAT_SERVICE_SERVICE_CONTAINER_H
#define EVIL_CAT_SERVICE_SERVICE_CONTAINER_H

#include <vector>
#include <memory>
#include "service_interface.h"

template<typename ...Ts>
struct ServiceContainer {
    using ContainerType = std::vector<std::unique_ptr<ServiceInterface>>;

    static ContainerType make() {
        ContainerType container;
        (container.emplace_back(std::make_unique<Ts>()), ...);
        return container;
    }
};

#endif //EVIL_CAT_SERVICE_SERVICE_CONTAINER_H
