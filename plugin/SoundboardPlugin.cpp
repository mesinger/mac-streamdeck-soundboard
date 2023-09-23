#include "SoundboardPlugin.hpp"
#include <StreamDeckSDK/ESDConnectionManager.h>
#include <utility>
#include <filesystem>

void
SoundboardPlugin::KeyDownForAction(const std::string &inAction, const std::string &inContext, const json &inPayload,
                                   const std::string &inDeviceID) {
    auto file = audioFileForContext.find(inContext);

    if (file == audioFileForContext.cend()) {
        return;
    }

    audioInterface->playWav(inContext.c_str(), file->second, 7);
}

void SoundboardPlugin::KeyUpForAction(const std::string &inAction, const std::string &inContext, const json &inPayload,
                                      const std::string &inDeviceID) {
}

void SoundboardPlugin::SendToPlugin(const std::string &inAction, const std::string &inContext, const json &inPayload,
                                    const std::string &inDeviceID) {
    std::string filePath = inPayload["file"];
    std::filesystem::path file(filePath);
    audioFileForContext[inContext] = file;

    mConnectionManager->SetTitle(file.stem().string(), inContext, kESDSDKTarget_HardwareAndSoftware);

    mConnectionManager->SetSettings({{"file", file.string()}}, inContext);
}

void
SoundboardPlugin::WillAppearForAction(const std::string &inAction, const std::string &inContext, const json &inPayload,
                                      const std::string &inDeviceID) {
    mVisibleContextsMutex.lock();

    mVisibleContexts.insert(inContext);

    std::string filePath = inPayload["settings"]["file"];
    std::filesystem::path file(filePath);
    audioFileForContext[inContext] = file;
    mConnectionManager->SetTitle(file.stem().string(), inContext, kESDSDKTarget_HardwareAndSoftware);

    mVisibleContextsMutex.unlock();
}

void SoundboardPlugin::WillDisappearForAction(const std::string &inAction, const std::string &inContext,
                                              const json &inPayload, const std::string &inDeviceID) {
    mVisibleContextsMutex.lock();
    mVisibleContexts.erase(inContext);
    mVisibleContextsMutex.unlock();
}

void SoundboardPlugin::DeviceDidConnect(const std::string &inDeviceID, const json &inDeviceInfo) {
    audioInterface->initialize();
}

void SoundboardPlugin::DeviceDidDisconnect(const std::string &inDeviceID) {
}

SoundboardPlugin::SoundboardPlugin(std::shared_ptr<AudioInterface> audioInterface): audioInterface(std::move(audioInterface)) {

}
