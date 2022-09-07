#ifndef EVIL_CAT_VIEW_VIEW_H
#define EVIL_CAT_VIEW_VIEW_H

#include <string>
#include <functional>

class View {
public:
    static View &getInstance();

    virtual bool initialize(const std::string &processName) = 0;

    virtual bool loop() = 0;
};

#endif //EVIL_CAT_VIEW_VIEW_H
