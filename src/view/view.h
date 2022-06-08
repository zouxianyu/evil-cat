#ifndef ASSAULT_CUBE_HACKING_VIEW_VIEW_H
#define ASSAULT_CUBE_HACKING_VIEW_VIEW_H

#include <string>
#include <functional>

class View {
public:
    static View &getInstance();

    virtual bool initialize(const std::string &processName) = 0;

    virtual bool registerCallback(std::function<void()> callback) = 0;

    virtual bool loop() = 0;
};

#endif //ASSAULT_CUBE_HACKING_VIEW_VIEW_H
