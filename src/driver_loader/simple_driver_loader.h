#ifndef EVIL_CAT_DRIVER_LOADER_SIMPLE_DRIVER_LOADER_H
#define EVIL_CAT_DRIVER_LOADER_SIMPLE_DRIVER_LOADER_H

#include "driver_loader_interface.h"

// copied from kdmapper

class SimpleDriverLoader : public DriverLoaderInterface {
public:
    bool load(const std::string &path, const std::string &name) override;

    bool unload(const std::string &name) override;
};

#endif //EVIL_CAT_DRIVER_LOADER_SIMPLE_DRIVER_LOADER_H
