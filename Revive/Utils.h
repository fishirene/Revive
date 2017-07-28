#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <process.h>
#include <Shlwapi.h>
#include <Shlobj.h>
#include <thread>
#include <vector>
#include <queue>
#include <string>
#include <regex>

#include <MinHook.h>

#include "OVR_CAPI.h"
#include "OVR_Version.h"
#include "OVR_CAPI_Util.h"
#include "Extras/OVR_Math.h"

typedef ovrTrackingState(__cdecl* _GetTrackingState)(ovrSession session, double absTime, ovrBool latencyMarker);
extern _GetTrackingState g_TrampolineFuncAddress;

//extern FILE* g_LogFileRevive;
//#define LOG(x, ...) if (g_LogFileRevive) fprintf(g_LogFileRevive, x, __VA_ARGS__); \
//					fflush(g_LogFileRevive);

#define REV_PUBLIC_FUNCTION(rval) extern "C" __declspec(dllexport) rval __cdecl

#define REV_SETTINGS_SECTION				"revive"
#define REV_ROUND(x)						round((double)x * pow(10.0, 4)) / pow(10.0, 4);

#define REV_KEY_HEAD_HAND_OFFSET_L_X		"HeadHandOffsetLX"
#define REV_DEFAULT_HEAD_HAND_OFFSET_L_X	-0.2f

#define REV_KEY_HEAD_HAND_OFFSET_L_Y		"HeadHandOffsetLY"
#define REV_DEFAULT_HEAD_HAND_OFFSET_L_Y	-0.3f

#define REV_KEY_HEAD_HAND_OFFSET_L_Z		"HeadHandOffsetLZ"
#define REV_DEFAULT_HEAD_HAND_OFFSET_L_Z	-0.8f

#define REV_KEY_HEAD_HAND_OFFSET_R_X		"HeadHandOffsetRX"
#define REV_DEFAULT_HEAD_HAND_OFFSET_R_X	0.2f

#define REV_KEY_HEAD_HAND_OFFSET_R_Y		"HeadHandOffsetRY"
#define REV_DEFAULT_HEAD_HAND_OFFSET_R_Y	-0.3f

#define REV_KEY_HEAD_HAND_OFFSET_R_Z		"HeadHandOffsetRZ"
#define REV_DEFAULT_HEAD_HAND_OFFSET_R_Z	-0.8f

#define REV_KEY_IGNORE_ACTIVITYLEVEL		"IgnoreActivityLevel"
#define REV_DEFAULT_IGNORE_ACTIVITYLEVEL	false

#define REV_TRIGGER_THRESHOLD				0.1176f

typedef enum ovrTouchSuppliment_ {
	ovrTouch_LThumbstick = 0x00000080,
	ovrTouch_RThumbstick = 0x00008000,
	ovrTouch_LHandTrigger = 0x00010000,
	ovrTouch_RHandTrigger = 0x00020000,
} ovrTouchSuppliment;
