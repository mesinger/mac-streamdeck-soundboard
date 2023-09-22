cmake_minimum_required(VERSION 3.10)

project(mesi.streamdeck.soundboard VERSION 1.2)

set(
        STREAMDECK_PLUGIN_DIR
        "$ENV{HOME}/Library/ApplicationSupport/com.elgato.StreamDeck/Plugins"
)
set(
        CMAKE_INSTALL_PREFIX
        "${STREAMDECK_PLUGIN_DIR}/${CMAKE_PROJECT_NAME}"
        CACHE STRING "See cmake documentation"
)

set(CMAKE_CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED true)

include_directories("${CMAKE_SOURCE_DIR}")
include("StreamDeckSDK.cmake")

set(
        SOURCES
        main.cpp
)
add_executable(
        soundboard
        SoundboardPlugin.cpp
        ${SOURCES}
)
target_link_libraries(soundboard StreamDeckSDK)
