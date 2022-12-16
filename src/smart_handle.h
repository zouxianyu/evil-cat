#ifndef EVIL_CAT_SMART_HANDLE_H
#define EVIL_CAT_SMART_HANDLE_H

#include <memory>
#include <windows.h>

namespace Detail {

    inline bool isValidHandle(HANDLE handle) {
        return handle != nullptr && handle != INVALID_HANDLE_VALUE;
    }

    struct HandleDeleter {
        void operator()(HANDLE handle) {
            if (isValidHandle(handle)) {
                CloseHandle(handle);
            }
        }
    };

    using SmartHandleBase = std::unique_ptr<std::remove_pointer_t<HANDLE>, HandleDeleter>;
}

struct SmartHandle : Detail::SmartHandleBase {
    using Detail::SmartHandleBase::SmartHandleBase;
    [[nodiscard]]
    inline bool is_valid() const {
        return Detail::isValidHandle(get());
    }
};

#endif //EVIL_CAT_SMART_HANDLE_H
