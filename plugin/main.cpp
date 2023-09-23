#include <StreamDeckSDK/ESDMain.h>
#include <StreamDeckSDK/ESDLogger.h>
#include "SoundboardPlugin.hpp"
#include <memory>

int main(int argc, const char** argv) {
    ESDLogger::Get()->SetWin32DebugPrefix("[mesi.soundboard] ");

    auto a = std::make_shared<AudioInterface>();
    auto result = esd_main(argc, argv, new SoundboardPlugin(a));

    return result;
}