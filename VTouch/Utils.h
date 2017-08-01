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

//extern FILE* g_LogFileVTive;
//#define LOG(x, ...) if (g_LogFileVTouch) fprintf(g_LogFileVTouch, x, __VA_ARGS__); \
//					fflush(g_LogFileVTouch);

#define VT_PUBLIC_FUNCTION(rval) extern "C" __declspec(dllexport) rval __cdecl

#define VT_SETTINGS_SECTION				"VTouch"
#define VT_ROUND(x)						round((double)x * pow(10.0, 4)) / pow(10.0, 4);

#define VT_KEY_HEAD_HAND_OFFSET_L_X		"HeadHandOffsetLX"
#define VT_DEFAULT_HEAD_HAND_OFFSET_L_X	-0.2f

#define VT_KEY_HEAD_HAND_OFFSET_L_Y		"HeadHandOffsetLY"
#define VT_DEFAULT_HEAD_HAND_OFFSET_L_Y	-0.3f

#define VT_KEY_HEAD_HAND_OFFSET_L_Z		"HeadHandOffsetLZ"
#define VT_DEFAULT_HEAD_HAND_OFFSET_L_Z	-0.8f

#define VT_KEY_HEAD_HAND_OFFSET_R_X		"HeadHandOffsetRX"
#define VT_DEFAULT_HEAD_HAND_OFFSET_R_X	0.2f

#define VT_KEY_HEAD_HAND_OFFSET_R_Y		"HeadHandOffsetRY"
#define VT_DEFAULT_HEAD_HAND_OFFSET_R_Y	-0.3f

#define VT_KEY_HEAD_HAND_OFFSET_R_Z		"HeadHandOffsetRZ"
#define VT_DEFAULT_HEAD_HAND_OFFSET_R_Z	-0.8f

#define VT_KEY_IGNORE_ACTIVITYLEVEL		"IgnoreActivityLevel"
#define VT_DEFAULT_IGNORE_ACTIVITYLEVEL	false

#define VT_TRIGGER_THRESHOLD			0.1176f

#define VT_HEAD_MOVE_SPEED				8e-7f

#define VT_HEAD_ROTATE_SPEED			5e-5f

typedef enum ovrTouchSuppliment_ {
	ovrTouch_LThumbstick = 0x00000080,
	ovrTouch_RThumbstick = 0x00008000,
	ovrTouch_LHandTrigger = 0x00010000,
	ovrTouch_RHandTrigger = 0x00020000,
} ovrTouchSuppliment;
