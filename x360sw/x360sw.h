#pragma once

#define XINPUT_GAMEPAD_GUIDE 0x0400

typedef DWORD (WINAPI *XInputGetStateProc)(DWORD dwUserIndex, XINPUT_STATE *pState);
typedef DWORD (WINAPI *XInputSetStateProc)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);
typedef DWORD (WINAPI *XInputGetCapabilitiesProc)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities);
typedef void (WINAPI *XInputEnableProc)(BOOL enable);
typedef DWORD (WINAPI *XInputGetDSoundAudioDeviceGuidsProc)(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid);
typedef DWORD (WINAPI *XInputGetBatteryInformationProc)(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION *pBatteryInformation);
typedef DWORD (WINAPI *XInputGetKeystrokeProc)(DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke);