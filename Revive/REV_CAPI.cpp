#include "OVR_CAPI.h"
#include "OVR_Version.h"

#include "InputManager.h"
#include "Utils.h"

#include <MinHook.h>
#include <wrl/client.h>
#include <Shlwapi.h>
#include <Shlobj.h>

#define REV_DEFAULT_TIMEOUT 10000

uint32_t g_MinorVersion = OVR_MINOR_VERSION;

static InputManager g_InputManager;

bool g_DebugPrinted = FALSE;

REV_PUBLIC_FUNCTION(ovrTrackingState) rev_GetTrackingState(ovrSession session, double absTime, ovrBool latencyMarker)
{
	ovrTrackingState state = g_TrampolineFuncAddress(session, absTime, latencyMarker);
	g_InputManager.GetTrackingState(session, &state, absTime);
	return state;
}

REV_PUBLIC_FUNCTION(unsigned int) rev_GetConnectedControllerTypes(ovrSession session)
{
	return g_InputManager.GetConnectedControllerTypes();
}

REV_PUBLIC_FUNCTION(ovrResult) rev_GetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState* inputState)
{
	if (!session)
		return ovrError_InvalidSession;

	if (!inputState)
		return ovrError_InvalidParameter;

	ovrInputState state = { 0 };

	//fprintf(g_LogFileRevive, "size of inputState: %zu\n", sizeof(inputState));
	ovrResult result = g_InputManager.GetInputState(session, controllerType, inputState);

	controllerType = (ovrControllerType)rev_GetConnectedControllerTypes(session);

	return result;
}

REV_PUBLIC_FUNCTION(bool) rev_EmulateTouchPositionOffset(bool l, float x, float y)
{
	if (l)
	{
		g_InputManager.EmulateTouchesPositionOffset(ovrControllerType_LTouch, x, y);
	}
	else
	{
		g_InputManager.EmulateTouchesPositionOffset(ovrControllerType_RTouch, x, y);
	}
	return true;
}

REV_PUBLIC_FUNCTION(bool) rev_EmulateTouchesInputState(unsigned int touchKey, bool state, float value, float x, float y)
{
	g_InputManager.EmulateTouchesInputState(touchKey, state, value, x, y);
	return true;
}

REV_PUBLIC_FUNCTION(bool) rev_EmulateResetTouchesPose()
{
	g_InputManager.EmulateResetTouchesPose();
	return true;
}

//OVR_PUBLIC_FUNCTION(unsigned int) ovr_GetTrackerCount(ovrSession session)
//{
//	//REV_TRACE(ovr_GetTrackerCount);
//
//	//uint32_t count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_TrackingReference, nullptr, 0);
//
//	return 2;
//}

//OVR_PUBLIC_FUNCTION(ovrTrackerDesc) ovr_GetTrackerDesc(ovrSession session, unsigned int trackerDescIndex)
//{
//	//REV_TRACE(ovr_GetTrackerDesc);
//
//	// Get the index for this tracker.
//	/*vr::TrackedDeviceIndex_t trackers[vr::k_unMaxTrackedDeviceCount];
//	vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_TrackingReference, trackers, vr::k_unMaxTrackedDeviceCount);
//	vr::TrackedDeviceIndex_t index = trackers[trackerDescIndex];*/
//
//	// Fill the descriptor.
//	ovrTrackerDesc desc;
//
//	// Calculate field-of-view.
//	/*float left = vr::VRSystem()->GetFloatTrackedDeviceProperty(index, vr::Prop_FieldOfViewLeftDegrees_Float);
//	float right = vr::VRSystem()->GetFloatTrackedDeviceProperty(index, vr::Prop_FieldOfViewRightDegrees_Float);
//	float top = vr::VRSystem()->GetFloatTrackedDeviceProperty(index, vr::Prop_FieldOfViewTopDegrees_Float);
//	float bottom = vr::VRSystem()->GetFloatTrackedDeviceProperty(index, vr::Prop_FieldOfViewBottomDegrees_Float);*/
//	/*desc.FrustumHFovInRadians = OVR::DegreeToRad(left + right);
//	desc.FrustumVFovInRadians = OVR::DegreeToRad(top + bottom);*/
//
//	// Get the tracking frustum.
//	/*desc.FrustumNearZInMeters = vr::VRSystem()->GetFloatTrackedDeviceProperty(index, vr::Prop_TrackingRangeMinimumMeters_Float);
//	desc.FrustumFarZInMeters = vr::VRSystem()->GetFloatTrackedDeviceProperty(index, vr::Prop_TrackingRangeMaximumMeters_Float);*/
//
//	ovrTrackerDesc desc;
//	return desc;
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

//OVR_PUBLIC_FUNCTION(ovrTrackingState) ovr_GetTrackingState(ovrSession session, double absTime, ovrBool latencyMarker)
//{
//	ovrTrackingState state = { 0 };
//
//	g_inputManager.GetTrackingState(session, &state, absTime);
//	return state;
//}

//OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetDevicePoses(ovrSession session, ovrTrackedDeviceType* deviceTypes, int deviceCount, double absTime, ovrPoseStatef* outDevicePoses)
//{
//	//REV_TRACE(ovr_GetDevicePoses);
//
//	if (!session)
//		return ovrError_InvalidSession;
//
//	return session->Input->GetDevicePoses(deviceTypes, deviceCount, absTime, outDevicePoses);
//}

//struct ovrSensorData_;
//typedef struct ovrSensorData_ ovrSensorData;
//
//OVR_PUBLIC_FUNCTION(ovrTrackingState) ovr_GetTrackingStateWithSensorData(ovrSession session, double absTime, ovrBool latencyMarker, ovrSensorData* sensorData)
//{
//	//REV_TRACE(ovr_GetTrackingStateWithSensorData);
//
//	// This is a private API, ignore the raw sensor data request and hope for the best.
//	//REV_ASSERT(sensorData == nullptr);
//
//	return ovr_GetTrackingState(session, absTime, latencyMarker);
//}

//OVR_PUBLIC_FUNCTION(ovrTrackerPose) ovr_GetTrackerPose(ovrSession session, unsigned int trackerPoseIndex)
//{
//	//REV_TRACE(ovr_GetTrackerPose);
//
//	ovrTrackerPose tracker = { 0 };
//
//	//if (!session)
//	//	return tracker;
//
//	// Get the index for this tracker.
//	/*vr::TrackedDeviceIndex_t trackers[vr::k_unMaxTrackedDeviceCount] = { vr::k_unTrackedDeviceIndexInvalid };
//	vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_TrackingReference, trackers, vr::k_unMaxTrackedDeviceCount);
//	vr::TrackedDeviceIndex_t index = trackers[trackerPoseIndex];*/
//
//	// Get the device poses.
//	/*vr::TrackedDevicePose_t pose;
//	vr::VRCompositor()->GetLastPoseForTrackedDeviceIndex(index, &pose, nullptr);*/
//
//	// TODO: Should the tracker pose always be in the standing universe?
//	//vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0.0, poses, vr::k_unMaxTrackedDeviceCount);
//
//	// Set the flags
//	/*tracker.TrackerFlags = 0;
//	if (index != vr::k_unTrackedDeviceIndexInvalid)
//	{
//		if (vr::VRSystem()->IsTrackedDeviceConnected(index))
//			tracker.TrackerFlags |= ovrTracker_Connected;
//		if (pose.bPoseIsValid)
//			tracker.TrackerFlags |= ovrTracker_PoseTracked;
//	}*/
//
//	// Convert the pose
//	/*REV::Matrix4f matrix;
//	if (index != vr::k_unTrackedDeviceIndexInvalid && pose.bPoseIsValid)
//		matrix = REV::Matrix4f(pose.mDeviceToAbsoluteTracking);*/
//
//	// We need to mirror the orientation along either the X or Y axis
//	/*OVR::Quatf quat = OVR::Quatf(matrix);
//	OVR::Quatf mirror = OVR::Quatf(1.0f, 0.0f, 0.0f, 0.0f);
//	tracker.Pose.Orientation = quat * mirror;
//	tracker.Pose.Position = matrix.GetTranslation();*/
//
//	// Level the pose
//	/*float yaw;
//	quat.GetYawPitchRoll(&yaw, nullptr, nullptr);
//	tracker.LeveledPose.Orientation = OVR::Quatf(OVR::Axis_Y, yaw);
//	tracker.LeveledPose.Position = matrix.GetTranslation();*/
//
//	return tracker;
//}

//OVR_PUBLIC_FUNCTION(ovrTouchHapticsDesc) ovr_GetTouchHapticsDesc(ovrSession session, ovrControllerType controllerType)
//{
////	//REV_TRACE(ovr_GetTouchHapticsDesc);
////
////	return session->Input->GetTouchHapticsDesc(controllerType);
//	ovrTouchHapticsDesc desc;
//	return desc;
//}

//OVR_PUBLIC_FUNCTION(ovrResult) ovr_Lookup(const char* name, void** data)
//{
//	// We don't communicate with the Oculus service.
//	return ovrError_ServiceError;
//	//return ovrSuccess;
//}
