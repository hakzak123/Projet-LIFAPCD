cmake_minimum_required(VERSION 3.31)
project(SMM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/main.cpp
    src/app/application.cpp
    src/map/mapinit.cpp
    src/map/map.cpp
    src/map/mapio.cpp
    src/render/render.cpp
    src/ui/ui.cpp
    src/screens/debugscreen.cpp
    src/screens/mainMenu.cpp
    src/screens/stats.cpp
    src/screens/settings.cpp
    src/screens/editor.cpp
    src/screens/loading.cpp
    src/screens/test.cpp
    src/update/update.cpp
    src/handling/argumentHandling.cpp
    src/handling/eventHandling.cpp
    src/handling/cursorHandling.cpp
    src/tinyfd/tinyfiledialogs.c
    src/gfx/SDL3_framerate.c
    src/gfx/SDL3_gfxPrimitives.c
    src/gfx/SDL3_imageFilter.c
    src/gfx/SDL3_rotozoom.c
    src/player/player.cpp
    src/globalTextures/globalTextures.cpp

)

include_directories(
    C:/SDL3-3.2.0/x86_64-w64-mingw32/include
    C:/SDL3-3.2.0/x86_64-w64-mingw32/include/SDL3_gfx
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/map
    ${CMAKE_SOURCE_DIR}/src/mapComponents
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/src/app
    ${CMAKE_SOURCE_DIR}/src/camera
    ${CMAKE_SOURCE_DIR}/src/player
    ${CMAKE_SOURCE_DIR}/src/ttf
    ${CMAKE_SOURCE_DIR}/src/windowInfo
    ${CMAKE_SOURCE_DIR}/src/tinyfd
    ${CMAKE_SOURCE_DIR}/src/editorinfo
)

link_directories(
    C:/SDL3-3.2.0/x86_64-w64-mingw32/lib
)

add_executable(SMM ${SOURCES})

target_link_libraries(SMM SDL3 SDL3_ttf)

set_target_properties(SMM PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
