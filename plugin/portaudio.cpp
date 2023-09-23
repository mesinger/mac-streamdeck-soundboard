#include <portaudio.h>
#include <iostream>

int main() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        // Handle the initialization error
        return 1;
    }

    int numDevices = Pa_GetDeviceCount();
    if (numDevices < 0) {
        // Handle the device enumeration error
        return 1;
    }

    for (int i = 0; i < numDevices; ++i) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo != nullptr) {
            std::cout << "Device " << i << ": " << deviceInfo->name << std::endl;
            // You can check deviceInfo->maxOutputChannels for output capabilities
        }
    }

    Pa_Terminate();
}
