#include <iostream>
#include <memory>
#include <thread>
#include <type_traits>
#include "module.h"
#include "service_config.h"
#include "cache/cache.h"
#include "mem/buffer_pool.h"
#include "controller/controller.h"
#include "proc/process_helper.h"
#include "settings.h"
#include "entry.h"

void entry() {
    // change default settings
    changeDefaultSettings();

    // call the game specified initialization interface
    // to get the config (currently only gui callbacks and fast loop callbacks)

    // do initialization of the core module
    uint32_t pid;
    while ((pid = ProcessHelper::getProcessIdByName(
            CONF_PROCESS_NAME, CONF_PROCESS_INDEX)) == 0) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(100ms);
    }

    Module::process->attach(pid);
    Module::view->initialize(pid);

    std::vector<std::unique_ptr<ServiceInterface>> services = ServiceList::make();
    // resolve GUI callbacks
    // for each in services, add them to gui callback
    for (std::unique_ptr<ServiceInterface> &service : services) {
        Controller::getInstance().addServiceCallback([&service] {
            service->serviceCallback();
        });
    }

    // we need to add a buffer pool refresh callback
    // because the cache need to be flushed each frame
    Controller::getInstance().addServiceCallback(Cache::refresh);

    // add menu callback
    for (std::unique_ptr<ServiceInterface> &service : services) {
        Controller::getInstance().addMenuCallback(service->getName(), [&service] {
            service->serviceCallback();
        });
    }

    // create the fast loop thread
    // the code inside the fast loop is a busy-wait loop
    std::thread fastLoopThread([] {
        Controller::getInstance().callFastLoopCallbacks();
    });

    // create the gui thread
    Module::view->loop();

    // set the exit flag in case the code in 'View' doesn't set it
    {
        std::lock_guard lock(Settings::mutex);
        Settings::exit = true;
    }

    // when the gui thread exits, the fast loop thread should also exit
    fastLoopThread.join();

    // do clean up here
    // because the ProcessMemory and ProcessInfo is singleton
    // we can't use RAII to do the cleanup
    // so just do it manually, although it doesn't look good
    Module::process->detach();

}
