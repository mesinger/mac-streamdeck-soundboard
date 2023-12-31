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

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED true)

include_directories("${CMAKE_SOURCE_DIR}")
include("StreamDeckSDK.cmake")

find_library(LIBSNDFILE_LIBRARIES sndfile REQUIRED)
find_library(PORT_AUDIO PortAudio REQUIRED)

add_executable(
        soundboard
        main.cpp
        SoundboardPlugin.cpp
        AudioInterface.cpp
)
target_include_directories(soundboard PRIVATE /opt/homebrew/include)
target_link_libraries(soundboard
        StreamDeckSDK
        ${PORT_AUDIO}
        ${LIBSNDFILE_LIBRARIES})

add_executable(debug portaudio.cpp AudioInterface.cpp)
target_include_directories(debug PRIVATE /opt/homebrew/include)
target_link_libraries(debug
        StreamDeckSDK
        ${LIBSNDFILE_LIBRARIES}
        ${PORT_AUDIO})
