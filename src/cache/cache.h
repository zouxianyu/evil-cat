#ifndef EVIL_CAT_CACHE_CACHE_H
#define EVIL_CAT_CACHE_CACHE_H

#include "registry.h"

namespace Cache {
    void ctor(CacheRegistry *registry);

    void dtor(CacheRegistry *registry);

    void refresh();
}

#endif //EVIL_CAT_CACHE_CACHE_H
