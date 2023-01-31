#include <cstdint>
#include <array>
#include "mem/memory_accessor.h"
#include "offset.h"
#include "decrypt.h"

namespace PUBG::Decrypt::Detail {
    template<typename Ty>
    inline Ty ROL(Ty v, unsigned int n) {
        n %= sizeof(Ty) * 8;
        return (v << n) | (v >> (sizeof(Ty) * 8 - n));
    }

    template<typename Ty>
    inline Ty ROR(Ty v, unsigned int n) {
        n %= sizeof(Ty) * 8;
        return (v >> n) | (v << (sizeof(Ty) * 8 - n));
    }

    struct WinDeleter {
        void operator()(void *p) const {
            VirtualFree(p, 0, MEM_RELEASE);
        }
    };

    using WinPtr = std::unique_ptr<void, WinDeleter>;

    using DecryptFuncTy = uint64_t(__fastcall *)(uint64_t, uint64_t);

    enum { COPY_SIZE = 128 };

    WinPtr mem{};

    uint64_t key = 0;

    bool initialized = false;

    bool initialize();
}

using namespace PUBG::Decrypt::Detail;

bool PUBG::Decrypt::Detail::initialize() {
    mem.reset(VirtualAlloc(
            nullptr,
            COPY_SIZE,
            MEM_COMMIT,
            PAGE_READWRITE
    ));

    if (!mem) {
        return false;
    }

    uint64_t pFunction = MemoryAccessor<uint64_t>(
            "TslGame.exe",
            Offset_XenuineDecrypt
    );

    auto decryptFuncBuf = reinterpret_cast<uint8_t *>(mem.get());
    MemoryAccessor<uint8_t>(pFunction).get(decryptFuncBuf, COPY_SIZE);

    // sanity check
    if (memcmp(decryptFuncBuf, "\x48\x8D\x05", 3) != 0) {
        return false;
    }

    // fix assembly code
    // 7FF4AB5A08F4 - 48 8D 05 84EF0000     - lea rax,[7FF4AB5AF87F]
    // ...

    key = pFunction + *reinterpret_cast<uint32_t *>(decryptFuncBuf + 3) + 7;

    // replace the first lea instruction
    // 48 8B C1 - mov rax, rcx
    // 90       - nop
    // 90       - nop
    // 90       - nop
    // 90       - nop

    memcpy(decryptFuncBuf, "\x48\x8B\xC1\x90\x90\x90\x90", 7);

    DWORD oldProtect;
    if (!VirtualProtect(
            mem.get(),
            COPY_SIZE,
            PAGE_EXECUTE_READ,
            &oldProtect
    )) {
        return false;
    }

    return true;
}

uint64_t PUBG::Decrypt::decryptPtr(uint64_t v) {
    // try to initialize the decrypt function
    if (!initialized && !(initialized = initialize())) {
        return 0;
    }

    auto decryptFunc = reinterpret_cast<DecryptFuncTy>(mem.get());
    return decryptFunc(key, v);
}

uint32_t PUBG::Decrypt::decryptId(uint32_t v) {
    auto transform =
            Offset_IsingRor ? ROR<uint32_t> : ROL<uint32_t>;
    v = transform(v ^ Offset_XorKey1, Offset_RorValue);
    v = v ^ (v << 16) ^ Offset_XorKey2;
    return v;
}

