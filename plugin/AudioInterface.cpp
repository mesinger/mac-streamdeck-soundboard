#include "AudioInterface.hpp"
#include <portaudio.h>
#include <StreamDeckSDK/ESDLogger.h>
#include <sndfile.h>

struct AudioData {
    SNDFILE* wavFile;
    SF_INFO wavInfo;
};

// Callback function for audio playback
static int audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void* userData) {
    auto* audioData = static_cast<AudioData*>(userData);

    if (audioData->wavFile == nullptr) {
        return paComplete; // No more data to play, stop the stream
    }

    // Read audio data from the WAV file into the output buffer
    sf_count_t bytesRead = sf_readf_float(audioData->wavFile, static_cast<float*>(outputBuffer), (long long) framesPerBuffer * audioData->wavInfo.channels);

    if (bytesRead < framesPerBuffer * audioData->wavInfo.channels) {
        sf_close(audioData->wavFile); // Close the WAV file when finished
        audioData->wavFile = nullptr;
    }

    return paContinue;
}

AudioInterface::~AudioInterface() {
    if (!isInitialized) {
        return;
    }

    Pa_Terminate();
}

void AudioInterface::initialize() {
    if (isInitialized) {
        return;
    }

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        return;
    }
    isInitialized = true;
}

void AudioInterface::playWav(const char *context, const std::string &file, DeviceId deviceId) const {
    if (!isInitialized) {
        ESDLogger::Get()->LogMessage(context, "Playing wav file without successful initialization");
        return;
    }

    ESDLogger::Get()->LogMessage(context, "Playing wav file");

    AudioData audioData{};
    audioData.wavFile = sf_open(file.c_str(), SFM_READ, &audioData.wavInfo);

    if (audioData.wavFile == nullptr) {
        ESDLogger::Get()->LogMessage(context, "Error opening wav file");
        return;
    }

    PaStream* stream;
    PaStreamParameters outputParams;

    outputParams.device = deviceId;
    outputParams.channelCount = audioData.wavInfo.channels;
    outputParams.sampleFormat = paFloat32;
    outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultHighOutputLatency;
    outputParams.hostApiSpecificStreamInfo = nullptr;

    PaError err = Pa_OpenStream(&stream, nullptr, &outputParams, audioData.wavInfo.samplerate, audioData.wavInfo.frames, paClipOff, audioCallback, &audioData);

    if (err != paNoError) {
        ESDLogger::Get()->LogMessage(context, "PortAudio stream error: %s", Pa_GetErrorText(err));
        sf_close(audioData.wavFile);
        return;
    }

    err = Pa_StartStream(stream);

    if (err != paNoError) {
        ESDLogger::Get()->LogMessage(context, "PortAudio start stream error: %s", Pa_GetErrorText(err));
        Pa_CloseStream(stream);
        sf_close(audioData.wavFile);
        return;
    }

    // Wait for the stream to finish playing
    while (Pa_IsStreamActive(stream)) {
        Pa_Sleep(100); // Sleep for a short period
    }

    err = Pa_StopStream(stream);
    if (err != paNoError) {
        ESDLogger::Get()->LogMessage(context, "PortAudio stop stream error: %s", Pa_GetErrorText(err));
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        ESDLogger::Get()->LogMessage(context, "PortAudio close stream error: %s", Pa_GetErrorText(err));
    }

    sf_close(audioData.wavFile);
}

