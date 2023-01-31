#include <set>
#include <memory>
#include "cache.h"

namespace Cache::Detail {
    std::set<CacheRegistry *> &getObjects();
}

using namespace Cache::Detail;

// we do this because the initialization order of static variables is undefined
// we should allocate the set before any CacheRegistry is constructed,
// so we use local static variable here
std::set<CacheRegistry *> &Cache::Detail::getObjects() {
    static std::set<CacheRegistry *> objects;
    return objects;
}

void Cache::ctor(CacheRegistry *registry) {
    getObjects().insert(registry);
}

void Cache::dtor(CacheRegistry *registry) {
    getObjects().erase(registry);
}

void Cache::refresh() {
    for (CacheRegistry *object: getObjects()) {
        object->refresh();
    }
}