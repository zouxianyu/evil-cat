#include <iostream>
#include <memory>
#include <thread>
#include <type_traits>
#include "module.h"
#include "service_config.h"
#include "mem/buffer_pool.h"
#include "controller/controller.h"
#include "settings.h"
#include "entry.h"

void entry() {
    // change default settings
    changeDefaultSettings();

    // call the game specified initialization interface
    // to get the config (currently only gui callbacks and fast loop callbacks)
    ServicesTypeList services;

    // do initialization of the core module
    Module::process->attach(CONF_PROCESS_NAME);
    Module::view->initialize(CONF_PROCESS_NAME);

    // resolve GUI callbacks
    // for each in services, add them to gui callback
    // fucking stupid tuple and confusing template
    std::apply([&](auto &&... service) {
        (Controller::getInstance().addServiceCallback( [&service] {
            static_assert(
                    std::is_base_of_v<ServiceInterface, std::decay_t<decltype(service)>>,
                    "service must be derived from ServiceInterface"
            );
            service.serviceCallback();
        }), ...);
    }, services);

    // we need to add a buffer pool refresh callback
    // because the cache need to be flushed each frame
    Controller::getInstance().addServiceCallback(
            [inst = &BufferPool::getInstance()] {
                inst->refresh();
            }
    );

    // add menu callback
    std::apply([&](auto &&... service) {
        (Controller::getInstance().addMenuCallback(service.getName(), [&service] {
            static_assert(
                    std::is_base_of_v<ServiceInterface, std::decay_t<decltype(service)>>,
                    "service must be derived from ServiceInterface"
            );
            service.menuCallback();
        }), ...);
    }, services);

    // create the fast loop thread
    // the code inside the fast loop is a busy-wait loop
    std::thread fastLoopThread{
            [inst = &Controller::getInstance()] {
                inst->callFastLoopCallbacks();
            }
    };

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
