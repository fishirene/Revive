#include "Utils.h"
#include "InputManager.h"

//#define VT_DEFAULT_TIMEOUT 10000

uint32_t g_MinorVersion = OVR_MINOR_VERSION;

InputManager* g_InputManager = InputManager::GetInstance();

//bool g_DebugPrinted = FALSE;

VT_PUBLIC_FUNCTION(ovrTrackingState) vt_GetTrackingState(ovrSession session, double absTime, ovrBool latencyMarker)
{
	ovrTrackingState state = g_TrampolineFuncAddress(session, absTime, latencyMarker);
	g_InputManager->GetTrackingState(session, &state, absTime);
	return state;
}

VT_PUBLIC_FUNCTION(unsigned int) vt_GetConnectedControllerTypes(ovrSession session)
{
	return g_InputManager->GetConnectedControllerTypes();
}

VT_PUBLIC_FUNCTION(ovrResult) vt_GetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState* inputState)
{
	if (!session)
		return ovrError_InvalidSession;

	if (!inputState)
		return ovrError_InvalidParameter;
	
	controllerType = (ovrControllerType)vt_GetConnectedControllerTypes(session);
	
	ovrResult result = g_InputManager->GetInputState(session, controllerType, inputState);
	return result;
}

//REV_PUBLIC_FUNCTION(void) rev_EmulateTouchPositionOffset(bool l, float x, float y)
//{
//	if (l)
//	{
//		g_InputManager->EmulateTouchesPositionOffset(ovrControllerType_LTouch, x, y);
//	}
//	else
//	{
//		g_InputManager->EmulateTouchesPositionOffset(ovrControllerType_RTouch, x, y);
//	}
//}

//REV_PUBLIC_FUNCTION(void) rev_EmulateResetTouchesPose()
//{
//	g_InputManager->EmulateResetTouchesPose();
//}

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
