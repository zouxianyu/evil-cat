cmake_minimum_required(VERSION 3.21)
project(assault_cube_hacking)

set(CMAKE_CXX_STANDARD 17)

#our headers
include_directories(src)
#lib headers
include_directories(lib/glm)
include_directories(lib/imgui)
include_directories("lib/Microsoft\ DirectX\ SDK/Include")


file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/lib/*.cpp")

#compile as exe or dll
add_executable(assault_cube_hacking ${SOURCES})
#add_library(assault_cube_hacking SHARED ${SOURCES})


#external headers and libs
file(GLOB_RECURSE LIBS "lib/Microsoft\ DirectX\ SDK/Lib/x86/*.lib")
#file(GLOB_RECURSE LIBS "lib/Microsoft\ DirectX\ SDK/Lib/x64/*.lib")
target_link_libraries(assault_cube_hacking ${LIBS})

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