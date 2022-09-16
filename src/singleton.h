#ifndef ASSAULT_CUBE_HACKING_SRC_SINGLETON_H
#define ASSAULT_CUBE_HACKING_SRC_SINGLETON_H

template <typename T>
class Singleton {
public:
    static T& getInstance() {
        static T instance;
        return instance;
    }
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

protected:
    Singleton() = default;
    ~Singleton() = default;
};

#endif //ASSAULT_CUBE_HACKING_SRC_SINGLETON_H
