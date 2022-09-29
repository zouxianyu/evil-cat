#include <iostream>
#include <memory>
#include <thread>
#include "game.h"
#include "init.h"
#include "module.h"
#include "service/service_interface.h"
#include "mem/buffer_pool.h"
#include "controller/controller.h"
#include "settings.h"
#include "entry.h"

void entry() {
    // call the game specified initialization interface
    // to get the config (currently only gui callbacks and fast loop callbacks)
    std::vector<std::unique_ptr<ServiceInterface>> services = init();

    // do initialization of the core module
    Module::processInfo->attach(CONF_PROCESS_NAME);
    Module::processMemory->attach(CONF_PROCESS_NAME);
    Module::view->initialize(CONF_PROCESS_NAME);

    // resolve GUI callbacks
    for (const auto &service: services) {
        Controller::getInstance().addGuiCallback(
                [&service] {
                    service->callback();
                }
        );
    }

    // we need to add a buffer pool refresh callback
    // because the cache need to be flushed each frame
    Controller::getInstance().addGuiCallback(
            [inst = &BufferPool::getInstance()] {
                inst->refresh();
            }
    );

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
    Module::processMemory->detach();
    Module::processInfo->detach();

}
