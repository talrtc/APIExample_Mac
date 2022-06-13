#ifndef __TTTRTCDEVICES_H__
#define __TTTRTCDEVICES_H__

#include "TTTstruct.h"

namespace TTTRtc {

    class IVideoDeviceCollection
    {
    protected:
        virtual ~IVideoDeviceCollection() {}
    public:
        virtual int getCount() = 0;

        virtual int getDevice(int index, char deviceName[MAX_DEVICE_ID_LENGTH], char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int setDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual void release() = 0;
    };

    class IVideoDeviceManager
    {
    protected:
        virtual ~IVideoDeviceManager() {}
    public:

        virtual IVideoDeviceCollection* enumerateVideoDevices() = 0;

        virtual int startDeviceTest(view_t hwnd) = 0;

        virtual int stopDeviceTest() = 0;

        virtual int setDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int getDevice(char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int getCameraResolutionCapability(char * pStrJson, int * outJsonLen, char devicePath[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual void release() = 0;
    };

    class IAudioDeviceCollection
    {
    protected:
        virtual ~IAudioDeviceCollection() {}
    public:

        virtual int getCount() = 0;

        virtual int getDevice(int index, char deviceName[MAX_DEVICE_ID_LENGTH], char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int setDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int setApplicationVolume(int volume) = 0;

        virtual int getApplicationVolume(int& volume) = 0;

        virtual int setApplicationMute(bool mute) = 0;
        virtual int isApplicationMute(bool& mute) = 0;

        virtual void release() = 0;
    };

    class IAudioDeviceManager
    {
    protected:
        virtual ~IAudioDeviceManager() {}
    public:

        virtual IAudioDeviceCollection* enumeratePlaybackDevices() = 0;

        virtual IAudioDeviceCollection* enumerateRecordingDevices() = 0;

        virtual int setPlaybackDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int setRecordingDevice(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int startPlaybackDeviceTest(const char* testAudioFilePath) = 0;

        virtual int stopPlaybackDeviceTest() = 0;

        virtual int setPlaybackDeviceVolume(int volume) = 0;

        virtual int getPlaybackDeviceVolume(int *volume) = 0;

        virtual int setAppDeviceVolume(int volume) = 0;

        virtual int getAppDeviceVolume(int *volume) = 0;

        virtual int setRecordingDeviceVolume(int volume) = 0;

        virtual int getRecordingDeviceVolume(int *volume) = 0;

        virtual int setPlaybackDeviceMute(bool mute) = 0;

        virtual int getPlaybackDeviceMute(bool *mute) = 0;

        virtual int setRecordingDeviceMute(bool mute) = 0;

        virtual int getRecordingDeviceMute(bool *mute) = 0;

        virtual int startRecordingDeviceTest(int indicationInterval) = 0;

        virtual int stopRecordingDeviceTest() = 0;

        virtual int getPlaybackDevice(char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int getPlaybackDeviceInfo(char deviceId[MAX_DEVICE_ID_LENGTH], char deviceName[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int getRecordingDevice(char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int getRecordingDeviceInfo(char deviceId[MAX_DEVICE_ID_LENGTH], char deviceName[MAX_DEVICE_ID_LENGTH]) = 0;

        virtual int startAudioDeviceLoopbackTest(int indicationInterval) = 0;

        virtual int stopAudioDeviceLoopbackTest() = 0;

        virtual void release() = 0;
    };


}


#endif /* __TTTRTCDEVICES_H__ */
