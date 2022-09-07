#include <iostream>
#include <memory>
#include <thread>
#include "game.h"
#include "init.h"
#include "game/interface/init_config.h"
#include "mem/process_memory.h"
#include "mem/buffer_pool.h"
#include "proc/process_info.h"
#include "view/view.h"
#include "controller/controller.h"
#include "settings.h"
#include "entry.h"

void entry() {
    // call the game specified initialization interface
    // to get the config (currently only gui callbacks and fast loop callbacks)
    std::shared_ptr<InitConfig> config = init();

    // do initialization of the core module
    ProcessInfo::getInstance().attach(CONF_PROCESS_NAME);
    ProcessMemory::getInstance().attach(CONF_PROCESS_NAME);
    View::getInstance().initialize(CONF_PROCESS_NAME);

    // resolve GUI callbacks
    for (const auto& callback: config->guiCallbacks) {
        Controller::getInstance().addGuiCallback(callback);
    }

    // we need to add a buffer pool refresh callback
    // because the cache need to be flushed each frame
    Controller::getInstance().addGuiCallback(
            std::bind(&BufferPool::refresh, &BufferPool::getInstance())
    );

    // resolve fast loop callbacks
    for (const auto& callback: config->fastLoopCallbacks) {
        Controller::getInstance().addFastLoopCallback(callback);
    }

    // create the fast loop thread
    // the code inside the fast loop is a busy-wait loop
    std::thread fastLoopThread{
            std::bind(&Controller::callFastLoopCallbacks, &Controller::getInstance())
    };

    // create the gui thread
    View::getInstance().loop();

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
    ProcessMemory::getInstance().detach();
    ProcessInfo::getInstance().detach();

}

