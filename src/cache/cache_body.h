#ifndef EVIL_CAT_CACHE_CACHE_BODY_H
#define EVIL_CAT_CACHE_CACHE_BODY_H

#define CACHE_BODY_TRANSFORM(data, flag, transform, getter, ...) \
    do {                                                   \
        if (flag) {                                        \
            return data;                                   \
        }                                                  \
        data = transform(instance->getter(__VA_ARGS__));   \
        flag = true;                                       \
        return data;                                       \
    } while (0)

#define CACHE_BODY(data, flag, getter, ...) \
    CACHE_BODY_TRANSFORM(data, flag, , getter, __VA_ARGS__)

#endif //EVIL_CAT_CACHE_CACHE_BODY_H
