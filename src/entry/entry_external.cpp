#include <thread>
#include "service/entry.h"

int main(int argc, char *argv[]) {
    std::thread t(entry);
    t.join();
    return 0;
}
