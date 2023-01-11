#include "cache.h"
#include "registry.h"

CacheRegistry::CacheRegistry() {
    Cache::ctor(this);
}

CacheRegistry::~CacheRegistry() {
    Cache::dtor(this);
}
