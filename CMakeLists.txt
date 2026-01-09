cmake_minimum_required(VERSION 3.16)
project(BebisOnline LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c)

add_executable(BebisOnline ${SOURCES})

set_target_properties(BebisOnline PROPERTIES
    SUFFIX ".html"
)

target_compile_options(BebisOnline PRIVATE
    -Wall -Wextra -pedantic
    -O3
)

target_include_directories(BebisOnline PRIVATE
    ${CMAKE_SOURCE_DIR}/inc
    /home/kosero/.emsdk/upstream/emscripten/cache/sysroot/include
)

target_link_libraries(BebisOnline PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libraylib.web.a
)

target_link_options(BebisOnline PRIVATE
    "SHELL:-s USE_GLFW=3"
    "SHELL:-s WASM=1"
    "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    "SHELL:-s ASYNCIFY"
    "--embed-file=bebis.png"
    "SHELL:-s EXPORTED_FUNCTIONS=['_on_ws_open_c','_on_ws_message_c','_on_ws_close_c','_main']"
    "SHELL:-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','UTF8ToString','stringToUTF8']"
)
