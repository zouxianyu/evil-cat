#ifndef EVIL_CAT_VIEW_VIEW_INTERFACE_H
#define EVIL_CAT_VIEW_VIEW_INTERFACE_H

#include <string>
#include <functional>

class ViewInterface {
public:
    virtual bool initialize(const std::string &processName) = 0;

    virtual bool loop() = 0;

    virtual ~ViewInterface() = default;
};

#endif //EVIL_CAT_VIEW_VIEW_INTERFACE_H
