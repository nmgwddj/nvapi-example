#include "stdafx.h"
#include "NvController.h"
#include <iostream>

NvController::NvController()
{
}


NvController::~NvController()
{
}

bool NvController::Initialize()
{
	hModule = LoadLibraryW(TEXT("nvapi.dll"));
	if (hModule == nullptr)
	{
		std::cerr << "Failed to load nvapi.dll." << std::endl;
		return false;
	}

	NvAPI_QueryInterface = (NvAPI_QueryInterface_t)GetProcAddress(hModule, "nvapi_QueryInterface");
	NvAPI_Initialize = (NvAPI_Initialize_t)(*NvAPI_QueryInterface)(_NvAPI_Initialize);
	NvAPI_EnumNvidiaDisplayHandle = (NvAPI_EnumNvidiaDisplayHandle_t)(*NvAPI_QueryInterface)(_NvAPI_EnumNvidiaDisplayHandle);
	NvAPI_GetAssociatedNvidiaDisplayName = (NvAPI_GetAssociatedNvidiaDisplayName_t)(*NvAPI_QueryInterface)(_NvAPI_GetAssociatedNvidiaDisplayName);
	NvAPI_GetDVCInfoEx = (NvAPI_GetDVCInfoEx_t)(*NvAPI_QueryInterface)(_NvAPI_GetDVCInfoEx);
	NvAPI_SetDVCLevelEx = (NvAPI_SetDVCLevelEx_t)(*NvAPI_QueryInterface)(_NvAPI_SetDVCLevelEx);
	NvAPI_GetHUEInfo = (NvAPI_GetHUEInfo_t)(*NvAPI_QueryInterface)(_NvAPI_GetHUEInfo);
	NvAPI_SetHUEAngle = (NvAPI_SetHUEAngle_t)(*NvAPI_QueryInterface)(_NvAPI_SetHUEAngle);

	_NvAPI_Status status = (_NvAPI_Status)(*NvAPI_Initialize)();
	if (status != NVAPI_OK)
	{
		std::cerr << "NvAPI initialization failed." << std::endl;
		return false;
	}

	return true;
}

void NvController::UnInitialize()
{
	if (hModule != nullptr)
	{
		FreeLibrary(hModule);
		hModule = nullptr;
	}
}

int NvController::EnumNvidiaDisplayHandle(int thisEnum, int* pNvDispHandle)
{
	int dispCount = 0;

	int enumNvDispHandle;

	_NvAPI_Status status = NVAPI_OK;
	while (status == NVAPI_OK)
	{
		status = (_NvAPI_Status)(*NvAPI_EnumNvidiaDisplayHandle)(dispCount, &enumNvDispHandle);
		if (status == NVAPI_OK)
		{
			if (dispCount == thisEnum)
			{
				*pNvDispHandle = enumNvDispHandle;
			}

			++dispCount;
		}
		else if (status == NVAPI_END_ENUMERATION)
		{
			std::cout << "NvAPI display handle enumeration finished. No more items to enumerate." << std::endl;
			break;
		}
		else if (status == NVAPI_NVIDIA_DEVICE_NOT_FOUND)
		{
			std::cerr << "NvAPI display handle enumeration failed. No NVIDIA display driver, or NVIDIA GPU driving a display, was found." << std::endl;
			break;
		}
		else if (status == NVAPI_ERROR)
		{
			std::cerr << "NvAPI display handle enumeration failed. Generic error." << std::endl;
			break;
		}
	}

	return dispCount;
}

bool NvController::SetDVCLevelEx(int nDisp, int level)
{
	int NvDispHandle;
	if (EnumNvidiaDisplayHandle(nDisp, &NvDispHandle) != 0)
	{
		NV_DISPLAY_DVC_INFO_EX oldInfo = GetDvcInfoEx(nDisp);

		NV_DISPLAY_DVC_INFO_EX info;
		info.version = oldInfo.version;
		info.currentLevel = level;
		info.minLevel = oldInfo.minLevel;
		info.maxLevel = oldInfo.maxLevel;
		info.defaultLevel = oldInfo.defaultLevel;

		_NvAPI_Status status = (_NvAPI_Status)(*NvAPI_SetDVCLevelEx)(NvDispHandle, 0, &info);
		if (status != NVAPI_OK)
		{
			return false;
		}

		return true;
	}

	return false;
}

NV_DISPLAY_DVC_INFO_EX NvController::GetDvcInfoEx(int nDisp)
{
	NV_DISPLAY_DVC_INFO_EX info = { 0 };

	int nvDispHandle;
	if (EnumNvidiaDisplayHandle(nDisp, &nvDispHandle) != 0)
	{
		info.version = sizeof(NV_DISPLAY_DVC_INFO_EX) | 0x10000;
		(*NvAPI_GetDVCInfoEx)(nvDispHandle, 0, &info);
	}

	return info;
}

NV_DISPLAY_HUE_INFO NvController::GetHUEInfo(int nDisp)
{
	NV_DISPLAY_HUE_INFO info = { 0 };

	int nvDispHandle;
	if (EnumNvidiaDisplayHandle(nDisp, &nvDispHandle) != 0)
	{
		info.version = sizeof(NV_DISPLAY_HUE_INFO) | 0x10000;
		(*NvAPI_GetHUEInfo)(nvDispHandle, 0, &info);
	}

	return info;
}

bool NvController::SetHUEAngle(int nDisp, int level)
{
	int NvDispHandle;
	if (EnumNvidiaDisplayHandle(nDisp, &NvDispHandle) != 0)
	{
		_NvAPI_Status status = (_NvAPI_Status)(*NvAPI_SetHUEAngle)(NvDispHandle, 0, level);
		if (status != NVAPI_OK)
		{
			return false;
		}

		return true;
	}

	return false;
}

