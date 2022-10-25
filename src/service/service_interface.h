#ifndef EVIL_CAT_SERVICE_SERVICE_INTERFACE_H
#define EVIL_CAT_SERVICE_SERVICE_INTERFACE_H

#include <string>

class ServiceInterface {
public:
    virtual std::string getName() = 0;

    virtual void menuCallback() = 0;

    virtual void serviceCallback() = 0;

    virtual ~ServiceInterface() = default;
};

#endif //EVIL_CAT_SERVICE_SERVICE_INTERFACE_H
