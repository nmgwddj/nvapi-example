#pragma once
#include "windows.h"

enum NvAPIs
{
	_NvAPI_Initialize = 0x150E828,
	_NvAPI_EnumNvidiaDisplayHandle = 0x9ABDD40D,
	_NvAPI_GetAssociatedNvidiaDisplayName = 0x22A78B05,
	_NvAPI_GetDVCInfoEx = 0x0E45002D,
	_NvAPI_SetDVCLevelEx = 0x4A82C2B1,
	_NvAPI_GetHUEInfo = 0x95B64341,
	_NvAPI_SetHUEAngle = 0xF5A0F22C
};

typedef struct
{
	unsigned int version;
	int currentLevel;
	int minLevel;
	int maxLevel;
	int defaultLevel;
} NV_DISPLAY_DVC_INFO_EX, *PNV_DISPLAY_DVC_INFO_EX;

typedef struct
{
	unsigned int version;
	int currentHueAngle;
	int defaultHueAngle;
} NV_DISPLAY_HUE_INFO, *PNV_DISPLAY_HUE_INFO;

typedef int*(*NvAPI_QueryInterface_t)(unsigned int offset);
typedef int(*NvAPI_Initialize_t)();
typedef int(*NvAPI_EnumNvidiaDisplayHandle_t)(int thisEnum, int* pNvDispHandle);
typedef int(*NvAPI_GetAssociatedNvidiaDisplayName_t)(int NvDispHandle, char szDisplayName[64]);
typedef int(*NvAPI_GetDVCInfoEx_t)(int hNvDisplay, int outputId, NV_DISPLAY_DVC_INFO_EX* pDVCInfo);
typedef int(*NvAPI_SetDVCLevelEx_t)(int hNvDisplay, int outputId, NV_DISPLAY_DVC_INFO_EX* pDVCInfo);
typedef int(*NvAPI_GetHUEInfo_t)(int hNvDisplay, int outputId, NV_DISPLAY_HUE_INFO* pHUEInfo);
typedef int(*NvAPI_SetHUEAngle_t)(int hNvDisplay, int outputId, int level);

class NvController
{
public:
	NvController();
	~NvController();

	bool Initialize();
	void UnInitialize();
	
	// 枚举显示器句柄
	int	EnumNvidiaDisplayHandle(int thisEnum, int* pNvDispHandle);

	// 设置和获取数字振动（Digital Vibrance Controls）数据
	bool SetDVCLevelEx(int nDisp, int level);
	NV_DISPLAY_DVC_INFO_EX GetDvcInfoEx(int nDisp);

	// 设置 NVIDIA 色调
	NV_DISPLAY_HUE_INFO GetHUEInfo(int nDisp);
	bool SetHUEAngle(int nDisp, int level);

private:
	HMODULE hModule = nullptr;;

	NvAPI_QueryInterface_t NvAPI_QueryInterface = nullptr;
	NvAPI_Initialize_t NvAPI_Initialize = nullptr;
	NvAPI_EnumNvidiaDisplayHandle_t NvAPI_EnumNvidiaDisplayHandle = nullptr;
	NvAPI_GetAssociatedNvidiaDisplayName_t NvAPI_GetAssociatedNvidiaDisplayName = nullptr;
	NvAPI_GetDVCInfoEx_t NvAPI_GetDVCInfoEx = nullptr;
	NvAPI_SetDVCLevelEx_t NvAPI_SetDVCLevelEx = nullptr;
	NvAPI_GetHUEInfo_t NvAPI_GetHUEInfo = nullptr;
	NvAPI_SetHUEAngle_t NvAPI_SetHUEAngle = nullptr;
};

