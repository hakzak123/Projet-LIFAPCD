cmake_minimum_required(VERSION 3.31)
project(SMM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
    src/render/render.cpp
    src/ui/ui.cpp
    src/ui/debugscreen.cpp
    src/ui/mainMenu.cpp
    src/ui/stats.cpp
    src/ui/settings.cpp
    src/ui/editor.cpp
    src/update/update.cpp
    src/handling/argumentHandling.cpp
    src/handling/eventHandling.cpp
    src/handling/cursorHandling.cpp
    src/globalTextures.cpp
)

include_directories(
    C:/SDL3-3.2.0/x86_64-w64-mingw32/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/map
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/src/app
    ${CMAKE_SOURCE_DIR}/src/camera
    ${CMAKE_SOURCE_DIR}/src/player
    ${CMAKE_SOURCE_DIR}/src/ttf
    ${CMAKE_SOURCE_DIR}/src/windowInfo
)

link_directories(
    C:/SDL3-3.2.0/x86_64-w64-mingw32/lib
)

add_executable(SMM ${SOURCES})

target_link_libraries(SMM SDL3 SDL3_ttf)

set_target_properties(SMM PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
