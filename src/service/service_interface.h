#ifndef EVIL_CAT_SERVICE_SERVICE_INTERFACE_H
#define EVIL_CAT_SERVICE_SERVICE_INTERFACE_H

class ServiceInterface {
public:
    virtual void callback() = 0;

    virtual ~ServiceInterface() = default;
};


#endif //EVIL_CAT_SERVICE_SERVICE_INTERFACE_H
