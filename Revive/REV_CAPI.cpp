#include <wrl/client.h>
#include <Shlwapi.h>
#include <Shlobj.h>

#include <MinHook.h>

#include "OVR_CAPI.h"
#include "OVR_Version.h"

#include "Utils.h"
#include "InputManager.h"

#define REV_DEFAULT_TIMEOUT 10000

uint32_t g_MinorVersion = OVR_MINOR_VERSION;

//InputManager g_InputManager;
InputManager* g_InputManager = InputManager::GetInstance();

bool g_DebugPrinted = FALSE;

REV_PUBLIC_FUNCTION(ovrTrackingState) rev_GetTrackingState(ovrSession session, double absTime, ovrBool latencyMarker)
{
	fprintf(g_LogFileRevive, "g_InputManager when get tracking: %p\n", g_InputManager);
	ovrTrackingState state = g_TrampolineFuncAddress(session, absTime, latencyMarker);
	g_InputManager->GetTrackingState(session, &state, absTime);
	return state;
}

REV_PUBLIC_FUNCTION(unsigned int) rev_GetConnectedControllerTypes(ovrSession session)
{
	return g_InputManager->GetConnectedControllerTypes();
}

REV_PUBLIC_FUNCTION(ovrResult) rev_GetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState* inputState)
{
	if (!session)
		return ovrError_InvalidSession;

	if (!inputState)
		return ovrError_InvalidParameter;
	
	controllerType = (ovrControllerType)rev_GetConnectedControllerTypes(session);
	
	ovrResult result = g_InputManager->GetInputState(session, controllerType, inputState);
	fprintf(g_LogFileRevive, "g_InputManager: %p\n", g_InputManager);
	//fprintf(g_LogFileRevive, "Hand trigger: %g\n", inputState->HandTrigger[0]);
	//fprintf(g_LogFileRevive, "Hand trigger n deadzone: %g\n", inputState->HandTriggerNoDeadzone[0]);
	//fprintf(g_LogFileRevive, "Hand trigger raw: %g\n", inputState->HandTriggerRaw[0]);
	return result;
}

REV_PUBLIC_FUNCTION(void) rev_EmulateTouchPositionOffset(bool l, float x, float y)
{
	if (l)
	{
		g_InputManager->EmulateTouchesPositionOffset(ovrControllerType_LTouch, x, y);
	}
	else
	{
		g_InputManager->EmulateTouchesPositionOffset(ovrControllerType_RTouch, x, y);
	}
}

REV_PUBLIC_FUNCTION(void) rev_EmulateTouchesInputState(unsigned int touchKey, bool state, float value, float x, float y)
{
	//fprintf(g_LogFileRevive, "External call, Touchkey: %u\n", touchKey);
	//fprintf(g_LogFileRevive, "External call, State: %u\n", state);
	//fprintf(g_LogFileRevive, "External call, Value: %g\n", value);
	//fprintf(g_LogFileRevive, "External call, x: %g\n", x);
	//fprintf(g_LogFileRevive, "External call, y: %g\n", y);
	g_InputManager->EmulateTouchesInputState(touchKey, state, value, x, y);
	fprintf(g_LogFileRevive, "g_InputManager when emulation: %p\n", g_InputManager);
}

REV_PUBLIC_FUNCTION(void) rev_EmulateResetTouchesPose()
{
	g_InputManager->EmulateResetTouchesPose();
}

//OVR_PUBLIC_FUNCTION(unsigned int) ovr_GetTrackerCount(ovrSession session)
//{
//	//REV_TRACE(ovr_GetTrackerCount);
//
//	//uint32_t count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_TrackingReference, nullptr, 0);
//
//	return 2;
//}

//OVR_PUBLIC_FUNCTION(ovrResult) ovr_SetTrackingOriginType(ovrSession session, ovrTrackingOrigin origin)
//{
//	//REV_TRACE(ovr_SetTrackingOriginType);
//
//	if (!session)
//		return ovrError_InvalidSession;
//
//	// Both enums match exactly, so we can just cast them
//	//vr::VRCompositor()->SetTrackingSpace((vr::ETrackingUniverseOrigin)origin);
//	return ovrSuccess;
//}

//OVR_PUBLIC_FUNCTION(ovrTrackingOrigin) ovr_GetTrackingOriginType(ovrSession session)
//{
//	//REV_TRACE(ovr_GetTrackingOriginType);
//
//	//if (!session)
//	//	return ovrTrackingOrigin_EyeLevel;
//	//ovrTrackingOrigin trackingOrigin;
//	// Both enums match exactly, so we can just cast them
//	return ovrTrackingOrigin_EyeLevel;
//}

//OVR_PUBLIC_FUNCTION(ovrResult) ovr_RecenterTrackingOrigin(ovrSession session)
//{
//	//REV_TRACE(ovr_RecenterTrackingOrigin);
//
//	if (!session)
//		return ovrError_InvalidSession;
//
//	//vr::VRSystem()->ResetSeatedZeroPose();
//	return ovrSuccess;
//}

//OVR_PUBLIC_FUNCTION(ovrResult) ovr_SpecifyTrackingOrigin(ovrSession session, ovrPosef originPose)
//{
//	// TODO: Implement through ApplyTransform()
//	return ovrSuccess;
//}

//OVR_PUBLIC_FUNCTION(void) ovr_ClearShouldRecenterFlag(ovrSession session) { /* No such flag, do nothing */ }

//OVR_PUBLIC_FUNCTION(ovrResult) ovr_Lookup(const char* name, void** data)
//{
//	// We don't communicate with the Oculus service.
//	return ovrError_ServiceError;
//	//return ovrSuccess;
//}
