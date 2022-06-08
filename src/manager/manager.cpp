#include "manager.h"

Manager &Manager::getInstance() {
    static Manager instance;
    return instance;
}


