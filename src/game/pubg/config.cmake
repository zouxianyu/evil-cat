# choose compile type
# if you want to compile as a DLL, then define the following 2 lines
#set(CONF_INTERNAL_COMPILE ON)
#add_compile_definitions(CONF_INTERNAL_COMPILE)

# tell me the process name so I can attach to it
add_compile_definitions(CONF_PROCESS_NAME="TslGame.exe")
add_compile_definitions(CONF_PROCESS_INDEX=1)

# game pointer width
add_compile_definitions(gameptr_t=uint64_t)
add_compile_definitions(gamesize_t=uint64_t)

# use buffer pool can improve performance when you are using
# a external build, especially when using a R/W driver
# to read/write game memory
add_compile_definitions(CONF_USE_BUFFER_POOL)
# cache a page in default
add_compile_definitions(CONF_BUFFER_POOL_CACHE_LINE_SIZE=0x1000)

# if the you are using left handed coordinate system
# you should define the following 2 lines
# so the glm library can use the correct coordinate system
add_compile_definitions(GLM_LEFT_HANDED)
add_compile_definitions(GLM_FORCE_LEFT_HANDED)

# if you want to show something in the console
# when using the dll, you need to define the following line
# only available for internal builds
add_compile_definitions(CONF_USING_STD_CONSOLE)