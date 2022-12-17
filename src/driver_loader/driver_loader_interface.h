#ifndef EVIL_CAT_DRIVER_LOADER_DRIVER_LOADER_INTERFACE_H
#define EVIL_CAT_DRIVER_LOADER_DRIVER_LOADER_INTERFACE_H

#include <string>

class DriverLoaderInterface {
public:
    virtual bool load(const std::string &path, const std::string &name) = 0;

    virtual bool unload(const std::string &name) = 0;

    virtual ~DriverLoaderInterface() = default;
};

#endif //EVIL_CAT_DRIVER_LOADER_DRIVER_LOADER_INTERFACE_H
