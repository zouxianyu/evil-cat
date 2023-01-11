#include <set>
#include <memory>
#include "cache.h"

namespace Cache::Detail {
    std::set<CacheRegistry *> *objects;

    std::set<CacheRegistry *> *getObjects();
}

using namespace Cache::Detail;

// no concurrency here, so no need to use call_once
// we do this because the initialization order of static variables is undefined
// we should allocate the set before any CacheRegistry is constructed
std::set<CacheRegistry *> *Cache::Detail::getObjects() {
    if (!objects) {
        objects = new std::set<CacheRegistry *>();
    }
    return objects;
}

void Cache::ctor(CacheRegistry *registry) {
    getObjects()->insert(registry);
}

void Cache::dtor(CacheRegistry *registry) {
    getObjects()->erase(registry);
    if (getObjects()->empty()) {
        delete objects;
        objects = nullptr;
    }
}

void Cache::refresh() {
    for (CacheRegistry *object: *getObjects()) {
        object->refresh();
    }
}