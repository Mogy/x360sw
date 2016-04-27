#include "stdafx.h"

XInputGetStateProc _XInputGetState;
XInputSetStateProc _XInputSetState;
XInputGetCapabilitiesProc _XInputGetCapabilities;
XInputEnableProc _XInputEnable;
XInputGetDSoundAudioDeviceGuidsProc _XInputGetDSoundAudioDeviceGuids;
XInputGetBatteryInformationProc _XInputGetBatteryInformation;
XInputGetKeystrokeProc _XInputGetKeystroke;

int currentUser = 0;
bool initialized = false;
bool pushGuide = false;

bool Initialize() 
{
	TCHAR path[MAX_PATH];
	GetSystemDirectory(path, sizeof(path));
	strcat_s(path, "\\xinput1_3.dll");
	HMODULE xinput_lib = LoadLibrary(path);

	_XInputGetState = (XInputGetStateProc) GetProcAddress(xinput_lib, (LPCTSTR)100);
	if(!_XInputGetState) {
		_XInputGetState = (XInputGetStateProc) GetProcAddress(xinput_lib, "XInputGetState");
	}
	_XInputSetState = (XInputSetStateProc) GetProcAddress(xinput_lib, "XInputSetState");
	_XInputGetCapabilities = (XInputGetCapabilitiesProc) GetProcAddress(xinput_lib, "XInputGetCapabilities");
	_XInputEnable = (XInputEnableProc) GetProcAddress(xinput_lib, "XInputEnable");
	_XInputGetDSoundAudioDeviceGuids = (XInputGetDSoundAudioDeviceGuidsProc) GetProcAddress(xinput_lib, "XInputGetDSoundAudioDeviceGuids");
	_XInputGetBatteryInformation = (XInputGetBatteryInformationProc) GetProcAddress(xinput_lib, "XInputGetBatteryInformation");
	_XInputGetKeystroke = (XInputGetKeystrokeProc) GetProcAddress(xinput_lib, "XInputGetKeystroke");

	if(_XInputGetState && _XInputSetState && _XInputGetCapabilities &&
		_XInputEnable && _XInputGetDSoundAudioDeviceGuids &&
		_XInputGetBatteryInformation && _XInputGetKeystroke) {
		initialized = true;
	} else {
		FreeLibrary(xinput_lib);
		xinput_lib = NULL;
	}

	return initialized;
}

inline void ClearDPAD(XINPUT_STATE* pState)
{
	pState->Gamepad.wButtons &= ~(XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_UP |
			XINPUT_GAMEPAD_DPAD_RIGHT | XINPUT_GAMEPAD_DPAD_DOWN);
}

inline void ChangeUser(XINPUT_STATE* pState)
{
	if(pushGuide = (pState->Gamepad.wButtons & XINPUT_GAMEPAD_GUIDE) != 0) {
		if((pState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0) {
			currentUser = 0;
		} else if((pState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0) {
			currentUser = 1;
		} else if((pState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0) {
			currentUser = 2;
		} else if((pState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0) {
			currentUser = 3;
		}
		ClearDPAD(pState);
	}
}

DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState)
{
	if(!initialized && !Initialize()) return ERROR_DEVICE_NOT_CONNECTED;

	DWORD ret = ERROR_DEVICE_NOT_CONNECTED;

	if (currentUser == dwUserIndex) {
		ret = _XInputGetState(0, pState);
		ChangeUser(pState);
	} else if(pushGuide) {
		ret = _XInputGetState(0, pState);
		ClearDPAD(pState);
	} else if(dwUserIndex == 0) {
		ret = _XInputGetState(currentUser, pState);
	} else {
		ret = _XInputGetState(dwUserIndex, pState);
	}

	return ret != ERROR_DEVICE_NOT_CONNECTED ? ret : ERROR_SUCCESS;
}

DWORD WINAPI XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
{
	if(!initialized && !Initialize()) return ERROR_DEVICE_NOT_CONNECTED;

	DWORD ret = ERROR_DEVICE_NOT_CONNECTED;

	if (currentUser == dwUserIndex) {
		ret = _XInputSetState(0, pVibration);
	} else if(dwUserIndex == 0) {
		ret = _XInputSetState(currentUser, pVibration);
	} else {
		ret = _XInputSetState(dwUserIndex, pVibration);
	}

	return ret != ERROR_DEVICE_NOT_CONNECTED ? ret : ERROR_SUCCESS;
}

DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities)
{
	if(!initialized && !Initialize()) return ERROR_DEVICE_NOT_CONNECTED;

	DWORD ret = ERROR_DEVICE_NOT_CONNECTED;

	if (currentUser == dwUserIndex) {
		ret = _XInputGetCapabilities(0, dwFlags, pCapabilities);
	} else if(dwUserIndex == 0) {
		ret = _XInputGetCapabilities(currentUser, dwFlags, pCapabilities);
	} else {
		ret = _XInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities);
	}

	return ret != ERROR_DEVICE_NOT_CONNECTED ? ret : ERROR_SUCCESS;
}

void WINAPI XInputEnable(BOOL enable)
{
	if(!initialized && !Initialize()) return;

	_XInputEnable(enable);
}

DWORD WINAPI XInputGetDSoundAudioDeviceGuids(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid)
{
	if(!initialized && !Initialize()) return ERROR_DEVICE_NOT_CONNECTED;

	DWORD ret = ERROR_DEVICE_NOT_CONNECTED;

	if (currentUser == dwUserIndex) {
		ret = _XInputGetDSoundAudioDeviceGuids(0, pDSoundRenderGuid, pDSoundCaptureGuid);
	} else if(dwUserIndex == 0) {
		ret = _XInputGetDSoundAudioDeviceGuids(currentUser, pDSoundRenderGuid, pDSoundCaptureGuid);
	} else {
		ret = _XInputGetDSoundAudioDeviceGuids(dwUserIndex, pDSoundRenderGuid, pDSoundCaptureGuid);
	}

	return ret != ERROR_DEVICE_NOT_CONNECTED ? ret : ERROR_SUCCESS;
}

DWORD WINAPI XInputGetBatteryInformation(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation)
{
	if(!initialized && !Initialize()) return ERROR_DEVICE_NOT_CONNECTED;

	DWORD ret = ERROR_DEVICE_NOT_CONNECTED;

	if (currentUser == dwUserIndex) {
		ret = _XInputGetBatteryInformation(0, devType, pBatteryInformation);
	} else if(dwUserIndex == 0) {
		ret = _XInputGetBatteryInformation(currentUser, devType, pBatteryInformation);
	} else {
		ret = _XInputGetBatteryInformation(dwUserIndex, devType, pBatteryInformation);
	}

	return ret != ERROR_DEVICE_NOT_CONNECTED ? ret : ERROR_SUCCESS;
}

DWORD WINAPI XInputGetKeystroke(DWORD dwUserIndex, DWORD dwReserved, PXINPUT_KEYSTROKE pKeystroke)
{
	if(!initialized && !Initialize()) return ERROR_DEVICE_NOT_CONNECTED;

	DWORD ret = ERROR_DEVICE_NOT_CONNECTED;

	if (currentUser == dwUserIndex) {
		ret = _XInputGetKeystroke(0, dwReserved, pKeystroke);
	} else if(dwUserIndex == 0) {
		ret = _XInputGetKeystroke(currentUser, dwReserved, pKeystroke);
	} else {
		ret = _XInputGetKeystroke(dwUserIndex, dwReserved, pKeystroke);
	}

	return ret != ERROR_DEVICE_NOT_CONNECTED ? ret : ERROR_EMPTY;
}
