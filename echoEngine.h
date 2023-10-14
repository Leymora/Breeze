//Echo Audio Engine - Vilhelm Hansson / Vespera Chromatic - 26 Dec 2022

// Windows Audio Session API (WASAPI) - Enables client applications to manage the flow of audio data between the application and an audio endpoint device
// https://learn.microsoft.com/en-us/windows/win32/coreaudio/wasapi

// Continue reading: https://learn.microsoft.com/en-us/windows/win32/coreaudio/rendering-a-stream
#pragma once

#include "echoSource.h"

#define RT_PER_SEC 10000000
#define RT_PER_MILLISEC 10000

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

int EchoEngine_PlayAudio(echoSource *echoSource)
{
    REFERENCE_TIME requestedDuration = RT_PER_SEC;
    REFERENCE_TIME actualDuration;
    IMMDeviceEnumerator *pEnumerator = nullptr;
    IMMDevice *pDevice = nullptr;
    IAudioClient *pAudioClient = nullptr;
    IAudioRenderClient *pRenderClient = nullptr;
    WAVEFORMATEX *pwfx = nullptr;
    UINT32 bufferFrameCount;
    UINT32 numFramesAvailable;
    UINT32 numFramesPadding;
    BYTE *pData;
    DWORD flags = 0;

    CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
    pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
    pAudioClient->GetMixFormat(&pwfx);
    pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, requestedDuration, 0, pwfx, NULL);

    //echoSource->SetFormat(pwfx);

    return 0;
} 