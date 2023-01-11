#ifndef EVIL_CAT_CACHE_REGISTRY_H
#define EVIL_CAT_CACHE_REGISTRY_H

class CacheRegistry {
public:
    CacheRegistry();

    virtual ~CacheRegistry();

    virtual void refresh() = 0;
};

#endif //EVIL_CAT_CACHE_REGISTRY_H
