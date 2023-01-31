#include <map>
#include "mem/memory_accessor.h"
#include "offset.h"
#include "decrypt.h"
#include "structure.h"
#include "name_pool.h"

namespace PUBG::NamePool::Detail {
    FNamePool getNamePool();

    std::string solveFName(uint64_t ptr);

    std::string getNameFromGame(uint32_t id);

    std::map<uint32_t, std::string> namePoolCache;
}

using namespace PUBG::NamePool::Detail;

FNamePool PUBG::NamePool::Detail::getNamePool() {
    uint64_t pNamePool = PUBG::decryptPtr(MemoryAccessor<uint64_t>(
            "TslGame.exe",
            Offset_FNameEntry
    ));
    FNamePool namePool = MemoryAccessor<FNamePool>(pNamePool);

    namePool.chunks = PUBG::decryptPtr(namePool.chunks);
    namePool.numElements = PUBG::decryptPtr(namePool.numElements);
    namePool.numChunks = PUBG::decryptPtr(namePool.numChunks);

    return namePool;
}

std::string PUBG::NamePool::Detail::solveFName(uint64_t ptr) {
    FNameEntry entry = MemoryAccessor<FNameEntry>(ptr);
//    uint64_t id = PUBG::decryptPtr(entry.id) >> 1;
    *entry.buf.rbegin() = '\0';
    return {reinterpret_cast<char *>(entry.buf.data())};
}

std::string PUBG::NamePool::Detail::getNameFromGame(uint32_t id) {
    uint32_t chunkIndex = id / Offset_ChunkSize;
    uint32_t entryIndex = id % Offset_ChunkSize;
    FNamePool namePool = getNamePool();

    // sanity check
    if (chunkIndex >= namePool.numChunks) {
        return "";
    }

    uint64_t chunk = MemoryAccessor<uint64_t>(
            namePool.chunks + chunkIndex * sizeof(uint64_t)
    );
    uint64_t entry = MemoryAccessor<uint64_t>(
            chunk + entryIndex * sizeof(uint64_t)
    );

    return solveFName(entry);
}

std::string PUBG::NamePool::getName(uint32_t id) {
    if (auto it = namePoolCache.find(id);
            it != namePoolCache.end()) {
        return it->second;
    }
    return namePoolCache[id] = getNameFromGame(id);
}
