#include "SoundboardPlugin.hpp"

#include <StreamDeckSDK/ESDConnectionManager.h>

void
SoundboardPlugin::KeyDownForAction(const std::string &inAction, const std::string &inContext, const json &inPayload,
                                   const std::string &inDeviceID) {
    auto file = audioFileForContext.find(inContext);

    if (file == audioFileForContext.cend()) {
        return;
    }

    mConnectionManager->SetTitle(file->second, inContext, kESDSDKTarget_HardwareAndSoftware);
}

void SoundboardPlugin::SendToPlugin(const std::string &inAction, const std::string &inContext, const json &inPayload,
                                    const std::string &inDeviceID) {
    std::string file = inPayload["file"];
    audioFileForContext[inContext] = file;

    mConnectionManager->SetTitle("Select:" + file, inContext, kESDSDKTarget_HardwareAndSoftware);
}

void
SoundboardPlugin::WillAppearForAction(const std::string &inAction, const std::string &inContext, const json &inPayload,
                                      const std::string &inDeviceID) {
    mVisibleContextsMutex.lock();
    mVisibleContexts.insert(inContext);
    mVisibleContextsMutex.unlock();
}

void SoundboardPlugin::WillDisappearForAction(const std::string &inAction, const std::string &inContext,
                                              const json &inPayload, const std::string &inDeviceID) {
    mVisibleContextsMutex.lock();
    mVisibleContexts.erase(inContext);
    mVisibleContextsMutex.unlock();
}

void SoundboardPlugin::KeyUpForAction(const std::string &inAction, const std::string &inContext, const json &inPayload,
                                      const std::string &inDeviceID) {
}

void SoundboardPlugin::DeviceDidConnect(const std::string &inDeviceID, const json &inDeviceInfo) {
}

void SoundboardPlugin::DeviceDidDisconnect(const std::string &inDeviceID) {
}
