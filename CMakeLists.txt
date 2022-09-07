cmake_minimum_required(VERSION 3.21)
project(evil-cat)

set(CMAKE_CXX_STANDARD 17)

#our sources
file(GLOB_RECURSE MY_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
#imgui sources
file(GLOB_RECURSE IMGUI_SOURCES "${PROJECT_SOURCE_DIR}/lib/imgui/*.cpp")
#merge sources
list(APPEND SOURCES ${MY_SOURCES} ${IMGUI_SOURCES})

#compile as exe or dll
add_executable(${PROJECT_NAME} ${SOURCES})
#add_library(${PROJECT_NAME} SHARED ${SOURCES})

#our headers
list(APPEND HEADERS src)
#lib headers
list(APPEND HEADERS lib/glm)
list(APPEND HEADERS lib/imgui)
list(APPEND HEADERS "lib/Microsoft\ DirectX\ SDK/Include")
#game specified headers
list(APPEND HEADERS "src/game/assault_cube")

target_include_directories(${PROJECT_NAME} PRIVATE ${HEADERS})

#static libs
file(GLOB_RECURSE STATIC_LIBS "lib/Microsoft\ DirectX\ SDK/Lib/x86/*.lib")
#file(GLOB_RECURSE STATIC_LIBS "lib/Microsoft\ DirectX\ SDK/Lib/x64/*.lib")
target_link_libraries(${PROJECT_NAME} ${STATIC_LIBS})

#configuration
add_compile_definitions(GLM_LEFT_HANDED)
add_compile_definitions(GLM_FORCE_LEFT_HANDED)

add_compile_definitions(CONF_INTERNAL=0)
add_compile_definitions(CONF_USING_STD_CONSOLE=1) # only available for internal builds
add_compile_definitions(CONF_USE_GAME_VIEW_MATRIX=0)
add_compile_definitions(CONF_PROCESS_NAME="ac_client.exe")
add_compile_definitions(AIMBOT_POLICY=minAnglePolicy)
add_compile_definitions(USE_BUFFER_POOL)
add_compile_definitions(BUFFER_POOL_CACHE_LINE_SIZE=0x1000)