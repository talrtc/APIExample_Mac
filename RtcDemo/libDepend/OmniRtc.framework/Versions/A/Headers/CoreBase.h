#ifndef __COREBASE_H__
#define __COREBASE_H__
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string>

namespace TTTRtc {
#if defined(RTMP_PUBLISHER)
    typedef enum {
        /*  */
        AV_RECORD_MP4 = 1,
        /*  */
        AV_PUSH_CDN = 2,
        /* record mp4 & push to cdn */
        AV_PUSH_MP4_CDN = 3,
        AV_PUSH_FORCE_DWORD = 0x7fffffff,
    } AV_PUSH_TYPE;
#endif

    typedef enum {
        AUDIO_RECORDING_QUALITY_LOW = 0,
        AUDIO_RECORDING_QUALITY_MEDIUM,
        AUDIO_RECORDING_QUALITY_HIGH,

        AUDIO_RECORDING_QUALITY_DWORD = 0x7fffffff,
    } AUDIO_RECORDING_QUALITY;

    typedef enum {
        CHANNEL_STATUS_INIT = 0,
        CHANNEL_STATUS_JOINING = 1,
        CHANNEL_STATUS_JOINED = 2,
        CHANNEL_STATUS_LEAVING = 3,

        CHANNEL_STATUSE_DWORD = 0x7fffffff,
    } CHANNEL_STATUS_TYPE;

#pragma pack(1)
    typedef struct TimeEventParam {
        void* ptr_callback = nullptr;
        std::string channelId = "";
        int64_t userId = 0;
        int userdata1 = 0;
        int userdata2 = 0;
    }TimeEventParam;
#pragma pack()
}
#endif
