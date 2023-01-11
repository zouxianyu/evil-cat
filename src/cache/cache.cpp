#include <set>
#include <algorithm>
#include "cache.h"

namespace Cache::Detail {
    std::set<CacheRegistry *> objects;
}

using namespace Cache::Detail;

void Cache::ctor(CacheRegistry *registry) {
    objects.insert(registry);
}

void Cache::dtor(CacheRegistry *registry) {
    objects.erase(registry);
}

void Cache::refresh() {
    for (CacheRegistry *object: objects) {
        object->refresh();
    }
}