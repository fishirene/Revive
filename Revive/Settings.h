#pragma once

enum revGripType
{
	revGrip_Normal = 0,
	revGrip_Toggle = 1,
	revGrip_Hybrid = 2,
};

#define REV_PUBLIC_FUNCTION(rval) extern "C" __declspec(dllexport) rval _cdecl

#define REV_SETTINGS_SECTION				"revive"
#define REV_ROUND(x)						round((double)x * pow(10.0, 4)) / pow(10.0, 4);

#define REV_KEY_HEAD_HAND_OFFSET_L_X		"HeadHandOffsetLX"
#define REV_DEFAULT_HEAD_HAND_OFFSET_L_X	-0.5f

#define REV_KEY_HEAD_HAND_OFFSET_L_Y		"HeadHandOffsetLY"
#define REV_DEFAULT_HEAD_HAND_OFFSET_L_Y	0.0f

#define REV_KEY_HEAD_HAND_OFFSET_L_Z		"HeadHandOffsetLZ"
#define REV_DEFAULT_HEAD_HAND_OFFSET_L_Z	1.0f

#define REV_KEY_HEAD_HAND_OFFSET_R_X		"HeadHandOffsetRX"
#define REV_DEFAULT_HEAD_HAND_OFFSET_R_X	0.5f

#define REV_KEY_HEAD_HAND_OFFSET_R_Y		"HeadHandOffsetRY"
#define REV_DEFAULT_HEAD_HAND_OFFSET_R_Y	0.0f

#define REV_KEY_HEAD_HAND_OFFSET_R_Z		"HeadHandOffsetRZ"
#define REV_DEFAULT_HEAD_HAND_OFFSET_R_Z	1.0f

#define REV_KEY_IGNORE_ACTIVITYLEVEL		"IgnoreActivityLevel"
#define REV_DEFAULT_IGNORE_ACTIVITYLEVEL	false
