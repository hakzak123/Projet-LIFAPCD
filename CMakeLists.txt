cmake_minimum_required(VERSION 3.31)
project(SDLapp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
    src/render.cpp
    src/ui.cpp
    src/update.cpp
    src/argumentHandling.cpp
)

include_directories(
    C:/SDL3-3.2.0/x86_64-w64-mingw32/include
    ${CMAKE_SOURCE_DIR}/src
)

link_directories(
    C:/SDL3-3.2.0/x86_64-w64-mingw32/lib
)

add_executable(SDLapp ${SOURCES})

target_link_libraries(SDLapp SDL3 SDL3_ttf)

set_target_properties(SDLapp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
