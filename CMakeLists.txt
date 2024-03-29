cmake_minimum_required(VERSION 3.21)
project(evil-cat)

set(CMAKE_CXX_STANDARD 17)

if(NOT DEFINED GAME_NAME)
    message(FATAL_ERROR "GAME_NAME is not defined")
endif()

#configuration
include("src/game/${GAME_NAME}/config.cmake")

set(TARGET_NAME "${GAME_NAME}_cheat")

#our sources
file(GLOB MY_SOURCES_TOP "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE MY_SOURCES_MODULES
        "${PROJECT_SOURCE_DIR}/src/cache/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/controller/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/driver_loader/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/entry/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/mem/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/proc/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/service/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/view/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/world_to_screen/*.cpp"
        )
file(GLOB_RECURSE MY_SOURCES_GAME "${PROJECT_SOURCE_DIR}/src/game/${GAME_NAME}/*.cpp")
list(APPEND MY_SOURCES ${MY_SOURCES_TOP} ${MY_SOURCES_MODULES} ${MY_SOURCES_GAME})

#imgui sources
file(GLOB IMGUI_SOURCES "${PROJECT_SOURCE_DIR}/lib/imgui/*.cpp")
list(APPEND IMGUI_BACKEND_SOURCES "${PROJECT_SOURCE_DIR}/lib/imgui/backends/imgui_impl_dx9.cpp")
list(APPEND IMGUI_BACKEND_SOURCES "${PROJECT_SOURCE_DIR}/lib/imgui/backends/imgui_impl_dx11.cpp")
list(APPEND IMGUI_BACKEND_SOURCES "${PROJECT_SOURCE_DIR}/lib/imgui/backends/imgui_impl_win32.cpp")
#merge sources
list(APPEND SOURCES ${MY_SOURCES} ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES})

#compile as exe or dll
if(CONF_INTERNAL_COMPILE)
    add_library(${TARGET_NAME} SHARED ${SOURCES})
else()
    add_executable(${TARGET_NAME} ${SOURCES})
endif()

#our headers
list(APPEND HEADERS src)
#lib headers
list(APPEND HEADERS lib/glm)
list(APPEND HEADERS lib/imgui)
list(APPEND HEADERS lib/imgui/backends)
#game specified headers
list(APPEND HEADERS "src/game/${GAME_NAME}")

target_include_directories(${TARGET_NAME} PRIVATE ${HEADERS})

#static libs
file(GLOB_RECURSE DX_STATIC_LIBS_X86 "lib/DirectX-SDK/Lib/x86/*.lib")
file(GLOB_RECURSE DX_STATIC_LIBS_X64 "lib/DirectX-SDK/Lib/x64/*.lib")
list(APPEND DX_STATIC_LIBS ${DX_STATIC_LIBS_X86} ${DX_STATIC_LIBS_X64})
target_link_libraries(${TARGET_NAME} ${DX_STATIC_LIBS})

target_link_libraries(${TARGET_NAME} "ntdll.lib")

# no terminal
target_link_options(${TARGET_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")