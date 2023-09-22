#pragma once

#include <StreamDeckSDK/ESDBasePlugin.h>
#include <mutex>
#include <set>
#include <map>

using json = nlohmann::json;

class SoundboardPlugin : public ESDBasePlugin
{
public:

    SoundboardPlugin() = default;
    ~SoundboardPlugin() override = default;

    void KeyDownForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID) override;
    void SendToPlugin(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID) override;

    void KeyUpForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID) override;
    void WillAppearForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID) override;
    void WillDisappearForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID) override;
    void DeviceDidConnect(const std::string& inDeviceID, const json &inDeviceInfo) override;
    void DeviceDidDisconnect(const std::string& inDeviceID) override;
private:
    std::map<std::string, std::string> audioFileForContext;

    std::mutex mVisibleContextsMutex;
    std::set<std::string> mVisibleContexts;
};
