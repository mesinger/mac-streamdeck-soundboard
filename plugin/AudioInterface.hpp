#pragma once
#include <string>

typedef int DeviceId;

class AudioInterface {
public:
    AudioInterface() = default;
    virtual ~AudioInterface();

    void initialize();
    void playWav(const char *context, const std::string &file, DeviceId deviceId) const;
private:
    bool isInitialized = false;
};
