#include "OVR_CAPI.h"
#include "OVR_Version.h"
#include "REV_Math.h"

#include "Session.h"
#include "InputManager.h"
#include "Settings.h"

#include <MinHook.h>
#include <wrl/client.h>

#include <assert.h>
#define REV_DEFAULT_TIMEOUT 10000

uint32_t g_MinorVersion = OVR_MINOR_VERSION;

ovrSession g_Session = nullptr;

OVR_PUBLIC_FUNCTION(ovrResult) ovr_Initialize(const ovrInitParams* inputParams)
{
	g_MinorVersion = inputParams->RequestedMinorVersion;

	MH_QueueDisableHook(LoadLibraryW);
	MH_QueueDisableHook(OpenEventW);
	MH_ApplyQueued();

	//vr::VR_Init(&g_InitError, vr::VRApplication_Scene);

	MH_QueueEnableHook(LoadLibraryW);
	MH_QueueEnableHook(OpenEventW);
	MH_ApplyQueued();

	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(void) ovr_Shutdown()
{
	MH_QueueDisableHook(LoadLibraryW);
	MH_QueueDisableHook(OpenEventW);
	MH_ApplyQueued();
}

REV_PUBLIC_FUNCTION(bool) rev_EmulateTouchPositionOffset(bool l, float x, float y)
{
	if (g_Session != nullptr)
	{
		if (l)
		{
			g_Session->Input->EmulateTouchesPositionOffset(ovrControllerType_LTouch, x, y);
		}
		else
		{
			g_Session->Input->EmulateTouchesPositionOffset(ovrControllerType_RTouch, x, y);
		}
		return true;
	}
	return false;
}

REV_PUBLIC_FUNCTION(bool) rev_EmulateTouchesInputState(unsigned int touchKey, bool state, float value, float x, float y)
{
	if (g_Session != nullptr)
	{
		g_Session->Input->EmulateTouchesInputState(touchKey, state, value, x, y);
		return true;
	}
	return false;
}

REV_PUBLIC_FUNCTION(bool) rev_EmulateResetTouchesPose()
{
	if (g_Session != nullptr)
	{
		g_Session->Input->EmulateResetTouchesPose();
		return true;
	}
	return false;
}

OVR_PUBLIC_FUNCTION(void) ovr_GetLastErrorInfo(ovrErrorInfo* errorInfo)
{
}

OVR_PUBLIC_FUNCTION(const char*) ovr_GetVersionString()
{
	return OVR_VERSION_STRING;
}

OVR_PUBLIC_FUNCTION(int) ovr_TraceMessage(int level, const char* message) { return 0; /* Debugging feature */ }

OVR_PUBLIC_FUNCTION(ovrResult) ovr_IdentifyClient(const char* identity) { return ovrSuccess; /* Debugging feature */ }

OVR_PUBLIC_FUNCTION(ovrHmdDesc) ovr_GetHmdDesc(ovrSession session)
{
	ovrHmdDesc desc;

	desc.Type = ovrHmd_CV1;

	// Get HMD name
	//vr::VRSystem()->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_ManufacturerName_String, desc.Manufacturer, 64);

	// Some games require a fake product name
	strncpy(desc.ProductName, "Oculus Rift", 64);

	// TODO: Get HID information
	/*desc.VendorId = 2833;
	desc.ProductId = 0031;*/
	desc.VendorId = 6945;
	desc.ProductId = 4162;

	// Get serial number
	strncpy(desc.SerialNumber, "WMHD302Q500JXK", 24);

	// TODO: Get firmware version
	desc.FirmwareMajor = 708;
	desc.FirmwareMinor = 0;

	// Get capabilities
	desc.AvailableHmdCaps = 0;
	desc.DefaultHmdCaps = 0;
	desc.AvailableTrackingCaps = ovrTrackingCap_Orientation | ovrTrackingCap_Position;
	desc.DefaultTrackingCaps = ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position;

	// Get field-of-view
	/*for (int i = 0; i < ovrEye_Count; i++)
	{
		ovrFovPort eye;
		vr::VRSystem()->GetProjectionRaw((vr::EVREye)i, &eye.LeftTan, &eye.RightTan, &eye.DownTan, &eye.UpTan);
		eye.LeftTan *= -1.0f;
		eye.DownTan *= -1.0f;
		desc.DefaultEyeFov[i] = eye;
		desc.MaxEyeFov[i] = eye;
	}*/

	// Get display properties
	//vr::VRSystem()->GetRecommendedRenderTargetSize((uint32_t*)&desc.Resolution.w, (uint32_t*)&desc.Resolution.h);
	//desc.Resolution.w *= 2; // Both eye ports
	//desc.DisplayRefreshRate = vr::VRSystem()->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_DisplayFrequency_Float);

	return desc;
}

OVR_PUBLIC_FUNCTION(unsigned int) ovr_GetTrackerCount(ovrSession session)
{
	//REV_TRACE(ovr_GetTrackerCount);

	//uint32_t count = vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_TrackingReference, nullptr, 0);

	return 2;
}

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

OVR_PUBLIC_FUNCTION(ovrResult) ovr_Create(ovrSession* pSession, ovrGraphicsLuid* pLuid)
{
	//REV_TRACE(ovr_Create);

	if (!pSession)
		return ovrError_InvalidParameter;

	*pSession = nullptr;

	// Initialize the opaque pointer with our own OpenVR-specific struct
	ovrSession session = new ovrHmdStruct();

	// Get the default universe origin from the settings
	//vr::VRCompositor()->SetTrackingSpace((vr::ETrackingUniverseOrigin)ovr_GetInt(session, REV_KEY_DEFAULT_ORIGIN, REV_DEFAULT_ORIGIN));

	// Get the LUID for the OpenVR adapter
	/*int32_t index;
	vr::VRSystem()->GetDXGIOutputInfo(&index);
	if (index == -1)
		index = 0;*/

	// Get the DXGI adapter matching the index
	/*Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)pFactory.GetAddressOf());
	if (FAILED(hr))
		return ovrError_IncompatibleGPU;*/

	/*Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter;
	hr = pFactory->EnumAdapters(index, pAdapter.GetAddressOf());
	if (FAILED(hr))
		return ovrError_MismatchedAdapters;*/

	/*DXGI_ADAPTER_DESC desc;
	hr = pAdapter->GetDesc(&desc);
	if (FAILED(hr))
		return ovrError_MismatchedAdapters;*/

	// Copy the LUID into the structure
	/*static_assert(sizeof(desc.AdapterLuid) == sizeof(ovrGraphicsLuid),
		"The adapter LUID needs to fit in ovrGraphicsLuid");
	memcpy(pLuid, &desc.AdapterLuid, sizeof(ovrGraphicsLuid));*/

	*pSession = session;
	g_Session = session;
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(void) ovr_Destroy(ovrSession session)
{
	delete session;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetSessionStatus(ovrSession session, ovrSessionStatus* sessionStatus)
{
//	REV_TRACE(ovr_GetSessionStatus);
//
//	if (!session)
//		return ovrError_InvalidSession;
//
//	if (!sessionStatus)
//		return ovrError_InvalidParameter;
//
//	// Check for quit event
//	// TODO: The SteamVR beta has a tendency to hang here for some unknown reason
//	// re-enable this code when the bug is fixed.
//#if 0
//	vr::VREvent_t ev;
//	while (vr::VRSystem()->PollNextEvent(&ev, sizeof(vr::VREvent_t)))
//	{
//		if (ev.eventType == vr::VREvent_Quit)
//		{
//			session->ShouldQuit = true;
//			vr::VRSystem()->AcknowledgeQuit_Exiting();
//		}
//	}
//#endif
//
//	 Don't use the activity level while debugging, so I don't have to put on the HMD
//	/*vr::EDeviceActivityLevel activityLevel = vr::k_EDeviceActivityLevel_Unknown;
//	if (!session->IgnoreActivity)
//		activityLevel = vr::VRSystem()->GetTrackedDeviceActivityLevel(vr::k_unTrackedDeviceIndex_Hmd);*/
//
//	 Detect if the application has focus, but only return false the first time the status is requested.
//	 If this is true from the beginning then some games will assume the Health-and-Safety warning
//	 is still being displayed.
//	/*sessionStatus->IsVisible = vr::VRCompositor()->CanRenderScene() && session->IsVisible;
//	session->IsVisible = true;*/
//
//	 TODO: Detect if the display is lost, can this ever happen with OpenVR?
	sessionStatus->HmdPresent = ovrTrue;
	sessionStatus->HmdMounted = ovrTrue;
	sessionStatus->DisplayLost = ovrFalse;
	sessionStatus->ShouldQuit = ovrFalse;
	sessionStatus->ShouldRecenter = ovrFalse;

	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_SetTrackingOriginType(ovrSession session, ovrTrackingOrigin origin)
{
	//REV_TRACE(ovr_SetTrackingOriginType);

	if (!session)
		return ovrError_InvalidSession;

	// Both enums match exactly, so we can just cast them
	//vr::VRCompositor()->SetTrackingSpace((vr::ETrackingUniverseOrigin)origin);
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrTrackingOrigin) ovr_GetTrackingOriginType(ovrSession session)
{
	//REV_TRACE(ovr_GetTrackingOriginType);

	//if (!session)
	//	return ovrTrackingOrigin_EyeLevel;
	//ovrTrackingOrigin trackingOrigin;
	// Both enums match exactly, so we can just cast them
	return ovrTrackingOrigin_EyeLevel;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_RecenterTrackingOrigin(ovrSession session)
{
	//REV_TRACE(ovr_RecenterTrackingOrigin);

	if (!session)
		return ovrError_InvalidSession;

	//vr::VRSystem()->ResetSeatedZeroPose();
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_SpecifyTrackingOrigin(ovrSession session, ovrPosef originPose)
{
	// TODO: Implement through ApplyTransform()
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(void) ovr_ClearShouldRecenterFlag(ovrSession session) { /* No such flag, do nothing */ }

OVR_PUBLIC_FUNCTION(ovrTrackingState) ovr_GetTrackingState(ovrSession session, double absTime, ovrBool latencyMarker)
{
	//REV_TRACE(ovr_GetTrackingState);

	ovrTrackingState state = { 0 };

	if (!session)
		return state;

	session->Input->GetTrackingState(session, &state, absTime);
	return state;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetDevicePoses(ovrSession session, ovrTrackedDeviceType* deviceTypes, int deviceCount, double absTime, ovrPoseStatef* outDevicePoses)
{
	//REV_TRACE(ovr_GetDevicePoses);

	if (!session)
		return ovrError_InvalidSession;

	return session->Input->GetDevicePoses(deviceTypes, deviceCount, absTime, outDevicePoses);
}

struct ovrSensorData_;
typedef struct ovrSensorData_ ovrSensorData;

OVR_PUBLIC_FUNCTION(ovrTrackingState) ovr_GetTrackingStateWithSensorData(ovrSession session, double absTime, ovrBool latencyMarker, ovrSensorData* sensorData)
{
	//REV_TRACE(ovr_GetTrackingStateWithSensorData);

	// This is a private API, ignore the raw sensor data request and hope for the best.
	//REV_ASSERT(sensorData == nullptr);

	return ovr_GetTrackingState(session, absTime, latencyMarker);
}

OVR_PUBLIC_FUNCTION(ovrTrackerPose) ovr_GetTrackerPose(ovrSession session, unsigned int trackerPoseIndex)
{
	//REV_TRACE(ovr_GetTrackerPose);

	ovrTrackerPose tracker = { 0 };

	//if (!session)
	//	return tracker;

	// Get the index for this tracker.
	/*vr::TrackedDeviceIndex_t trackers[vr::k_unMaxTrackedDeviceCount] = { vr::k_unTrackedDeviceIndexInvalid };
	vr::VRSystem()->GetSortedTrackedDeviceIndicesOfClass(vr::TrackedDeviceClass_TrackingReference, trackers, vr::k_unMaxTrackedDeviceCount);
	vr::TrackedDeviceIndex_t index = trackers[trackerPoseIndex];*/

	// Get the device poses.
	/*vr::TrackedDevicePose_t pose;
	vr::VRCompositor()->GetLastPoseForTrackedDeviceIndex(index, &pose, nullptr);*/

	// TODO: Should the tracker pose always be in the standing universe?
	//vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0.0, poses, vr::k_unMaxTrackedDeviceCount);

	// Set the flags
	/*tracker.TrackerFlags = 0;
	if (index != vr::k_unTrackedDeviceIndexInvalid)
	{
		if (vr::VRSystem()->IsTrackedDeviceConnected(index))
			tracker.TrackerFlags |= ovrTracker_Connected;
		if (pose.bPoseIsValid)
			tracker.TrackerFlags |= ovrTracker_PoseTracked;
	}*/

	// Convert the pose
	/*REV::Matrix4f matrix;
	if (index != vr::k_unTrackedDeviceIndexInvalid && pose.bPoseIsValid)
		matrix = REV::Matrix4f(pose.mDeviceToAbsoluteTracking);*/

	// We need to mirror the orientation along either the X or Y axis
	/*OVR::Quatf quat = OVR::Quatf(matrix);
	OVR::Quatf mirror = OVR::Quatf(1.0f, 0.0f, 0.0f, 0.0f);
	tracker.Pose.Orientation = quat * mirror;
	tracker.Pose.Position = matrix.GetTranslation();*/

	// Level the pose
	/*float yaw;
	quat.GetYawPitchRoll(&yaw, nullptr, nullptr);
	tracker.LeveledPose.Orientation = OVR::Quatf(OVR::Axis_Y, yaw);
	tracker.LeveledPose.Position = matrix.GetTranslation();*/

	return tracker;
}

// Pre-1.7 input state
typedef struct ovrInputState1_
{
	double              TimeInSeconds;
	unsigned int        Buttons;
	unsigned int        Touches;
	float               IndexTrigger[ovrHand_Count];
	float               HandTrigger[ovrHand_Count];
	ovrVector2f         Thumbstick[ovrHand_Count];
	ovrControllerType   ControllerType;
} ovrInputState1;

// Pre-1.11 input state
typedef struct ovrInputState2_
{
	double              TimeInSeconds;
	unsigned int        Buttons;
	unsigned int        Touches;
	float               IndexTrigger[ovrHand_Count];
	float               HandTrigger[ovrHand_Count];
	ovrVector2f         Thumbstick[ovrHand_Count];
	ovrControllerType   ControllerType;
	float               IndexTriggerNoDeadzone[ovrHand_Count];
	float               HandTriggerNoDeadzone[ovrHand_Count];
	ovrVector2f         ThumbstickNoDeadzone[ovrHand_Count];
} ovrInputState2;

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState* inputState)
{
	//REV_TRACE(ovr_GetInputState);

	if (!session)
		return ovrError_InvalidSession;

	if (!inputState)
		return ovrError_InvalidParameter;

	ovrInputState state = { 0 };
	ovrResult result = session->Input->GetInputState(session, controllerType, &state);

	// We need to make sure we don't write outside of the bounds of the struct
	// when the client expects a pre-1.7 version of LibOVR.
	if (g_MinorVersion < 7)
		memcpy(inputState, &state, sizeof(ovrInputState1));
	else if (g_MinorVersion < 11)
		memcpy(inputState, &state, sizeof(ovrInputState2));
	else
		memcpy(inputState, &state, sizeof(ovrInputState));

	return result;
}

OVR_PUBLIC_FUNCTION(unsigned int) ovr_GetConnectedControllerTypes(ovrSession session)
{
	//REV_TRACE(ovr_GetConnectedControllerTypes);

	return session->Input->GetConnectedControllerTypes();
}

OVR_PUBLIC_FUNCTION(ovrTouchHapticsDesc) ovr_GetTouchHapticsDesc(ovrSession session, ovrControllerType controllerType)
{
//	//REV_TRACE(ovr_GetTouchHapticsDesc);
//
//	return session->Input->GetTouchHapticsDesc(controllerType);
	ovrTouchHapticsDesc desc;
	return desc;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_TestBoundary(ovrSession session, ovrTrackedDeviceType deviceBitmask,
	ovrBoundaryType boundaryType, ovrBoundaryTestResult* outTestResult)
{
	/*REV_TRACE(ovr_TestBoundary);

	outTestResult->ClosestDistance = INFINITY;

	vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];
	vr::VRCompositor()->GetLastPoses(poses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

	if (vr::VRChaperone()->GetCalibrationState() != vr::ChaperoneCalibrationState_OK)
		return ovrSuccess_BoundaryInvalid;

	if (deviceBitmask & ovrTrackedDevice_HMD)
	{
		ovrBoundaryTestResult result = { 0 };
		REV::Matrix4f matrix = (REV::Matrix4f)poses[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking;
		ovrVector3f point = matrix.GetTranslation();

		ovrResult err = ovr_TestBoundaryPoint(session, &point, boundaryType, &result);
		if (OVR_SUCCESS(err) && result.ClosestDistance < outTestResult->ClosestDistance)
			*outTestResult = result;
	}


	vr::TrackedDeviceIndex_t hands[] = { vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand),
		vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand) };

	for (int i = 0; i < ovrHand_Count; i++)
	{
		if (deviceBitmask & (ovrTrackedDevice_LTouch << i))
		{
			ovrBoundaryTestResult result = { 0 };
			if (hands[i] != vr::k_unTrackedDeviceIndexInvalid)
			{
				REV::Matrix4f matrix = (REV::Matrix4f)poses[hands[i]].mDeviceToAbsoluteTracking;
				ovrVector3f point = matrix.GetTranslation();

				ovrResult err = ovr_TestBoundaryPoint(session, &point, boundaryType, &result);
				if (OVR_SUCCESS(err) && result.ClosestDistance < outTestResult->ClosestDistance)
					*outTestResult = result;
			}
		}
	}*/

	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_TestBoundaryPoint(ovrSession session, const ovrVector3f* point,
	ovrBoundaryType singleBoundaryType, ovrBoundaryTestResult* outTestResult)
{
	//REV_TRACE(ovr_TestBoundaryPoint);

	//ovrBoundaryTestResult result = { 0 };

	//if (vr::VRChaperone()->GetCalibrationState() != vr::ChaperoneCalibrationState_OK)
	//	return ovrSuccess_BoundaryInvalid;

	//result.IsTriggering = vr::VRChaperone()->AreBoundsVisible();

	//OVR::Vector2f playArea;
	//if (!vr::VRChaperone()->GetPlayAreaSize(&playArea.x, &playArea.y))
	//	return ovrSuccess_BoundaryInvalid;

	//// Clamp the point to the AABB
	//OVR::Vector2f p(point->x, point->z);
	//OVR::Vector2f halfExtents(playArea.x / 2.0f, playArea.y / 2.0f);
	//OVR::Vector2f clamped = OVR::Vector2f::Min(OVR::Vector2f::Max(p, -halfExtents), halfExtents);

	//// If the point is inside the AABB, we need to do some extra work
	//if (clamped.Compare(p))
	//{
	//	if (std::abs(p.x) > std::abs(p.y))
	//		clamped.x = halfExtents.x * (p.x / std::abs(p.x));
	//	else
	//		clamped.y = halfExtents.y * (p.y / std::abs(p.y));
	//}

	//// We don't have a ceiling, use the height from the original point
	//result.ClosestPoint.x = clamped.x;
	//result.ClosestPoint.y = point->y;
	//result.ClosestPoint.z = clamped.y;

	//// Get the normal, closest distance is the length of this normal
	//OVR::Vector2f normal = p - clamped;
	//result.ClosestDistance = normal.Length();

	//// Normalize the normal
	//normal.Normalize();
	//result.ClosestPointNormal.x = normal.x;
	//result.ClosestPointNormal.y = 0.0f;
	//result.ClosestPointNormal.z = normal.y;

	//*outTestResult = result;
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_SetBoundaryLookAndFeel(ovrSession session, const ovrBoundaryLookAndFeel* lookAndFeel)
{
	/*REV_TRACE(ovr_SetBoundaryLookAndFeel);

	vr::HmdColor_t color = (vr::HmdColor_t&)lookAndFeel->Color;
	vr::VRChaperone()->SetSceneColor(color);*/
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_ResetBoundaryLookAndFeel(ovrSession session)
{
	/*REV_TRACE(ovr_ResetBoundaryLookAndFeel);

	vr::HmdColor_t color = { 1.0f, 1.0f, 1.0f, 1.0f };
	vr::VRChaperone()->SetSceneColor(color);*/
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetBoundaryGeometry(ovrSession session, ovrBoundaryType boundaryType, ovrVector3f* outFloorPoints, int* outFloorPointsCount)
{
	/*REV_TRACE(ovr_GetBoundaryGeometry);

	vr::HmdQuad_t playRect;
	bool valid = vr::VRChaperone()->GetPlayAreaRect(&playRect);
	if (outFloorPoints)
		memcpy(outFloorPoints, playRect.vCorners, 4 * sizeof(ovrVector3f));
	if (outFloorPointsCount)
		*outFloorPointsCount = valid ? 4 : 0;*/
	//return valid ? ovrSuccess : ovrSuccess_BoundaryInvalid;
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetBoundaryDimensions(ovrSession session, ovrBoundaryType boundaryType, ovrVector3f* outDimensions)
{
	//REV_TRACE(ovr_GetBoundaryDimensions);

	//outDimensions->y = 0.0f; // TODO: Find some good default height
	//bool valid = vr::VRChaperone()->GetPlayAreaSize(&outDimensions->x, &outDimensions->z);
	//return valid ? ovrSuccess : ovrSuccess_BoundaryInvalid;
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetBoundaryVisible(ovrSession session, ovrBool* outIsVisible)
{
	/*REV_TRACE(ovr_GetBoundaryVisible);

	*outIsVisible = vr::VRChaperone()->AreBoundsVisible();*/
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_RequestBoundaryVisible(ovrSession session, ovrBool visible)
{
	/*REV_TRACE(ovr_RequestBoundaryVisible);

	vr::VRChaperone()->ForceBoundsVisible(!!visible);*/
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetTextureSwapChainLength(ovrSession session, ovrTextureSwapChain chain, int* out_Length)
{
	/*REV_TRACE(ovr_GetTextureSwapChainLength);

	if (!chain)
		return ovrError_InvalidParameter;

	MICROPROFILE_META_CPU("Identifier", chain->Identifier);
	*out_Length = chain->Length;*/
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetTextureSwapChainCurrentIndex(ovrSession session, ovrTextureSwapChain chain, int* out_Index)
{
	/*REV_TRACE(ovr_GetTextureSwapChainCurrentIndex);

	if (!chain)
		return ovrError_InvalidParameter;

	MICROPROFILE_META_CPU("Identifier", chain->Identifier);
	MICROPROFILE_META_CPU("Index", chain->CurrentIndex);
	*out_Index = chain->CurrentIndex;*/
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetTextureSwapChainDesc(ovrSession session, ovrTextureSwapChain chain, ovrTextureSwapChainDesc* out_Desc)
{
	/*REV_TRACE(ovr_GetTextureSwapChainDesc);

	if (!chain)
		return ovrError_InvalidParameter;

	MICROPROFILE_META_CPU("Identifier", chain->Identifier);
	*out_Desc = chain->Desc;*/
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_CommitTextureSwapChain(ovrSession session, ovrTextureSwapChain chain)
{
	/*REV_TRACE(ovr_CommitTextureSwapChain);

	if (!chain)
		return ovrError_InvalidParameter;

	MICROPROFILE_META_CPU("Identifier", chain->Identifier);
	MICROPROFILE_META_CPU("Index", chain->CurrentIndex);
	chain->Submitted = chain->Textures[chain->CurrentIndex].get();
	chain->CurrentIndex++;
	chain->CurrentIndex %= chain->Length;*/
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(void) ovr_DestroyTextureSwapChain(ovrSession session, ovrTextureSwapChain chain)
{
	/*REV_TRACE(ovr_DestroyTextureSwapChain);

	if (!chain)
		return;

	MICROPROFILE_META_CPU("Identifier", chain->Identifier);
	vr::VROverlay()->DestroyOverlay(chain->Overlay);
	delete chain;*/
}

OVR_PUBLIC_FUNCTION(void) ovr_DestroyMirrorTexture(ovrSession session, ovrMirrorTexture mirrorTexture)
{
	/*REV_TRACE(ovr_DestroyMirrorTexture);

	if (!mirrorTexture)
		return;

	session->Compositor->SetMirrorTexture(nullptr);
	delete mirrorTexture;*/
}

//OVR_PUBLIC_FUNCTION(ovrSizei) ovr_GetFovTextureSize(ovrSession session, ovrEyeType eye, ovrFovPort fov, float pixelsPerDisplayPixel)
//{
//	//REV_TRACE(ovr_GetFovTextureSize);
//
	//ovrSizei size;
//	//vr::VRSystem()->GetRecommendedRenderTargetSize((uint32_t*)&size.w, (uint32_t*)&size.h);
//
//	//// Check if an override for pixelsPerDisplayPixel is present
//	//if (session && session->PixelsPerDisplayPixel > 0.0f)
//	//	pixelsPerDisplayPixel = session->PixelsPerDisplayPixel;
//
//	//// Grow the recommended size to account for the overlapping fov
//	//vr::VRTextureBounds_t bounds = CompositorBase::FovPortToTextureBounds(eye, fov);
//	//size.w = int((size.w * pixelsPerDisplayPixel) / (bounds.uMax - bounds.uMin));
//	//size.h = int((size.h * pixelsPerDisplayPixel) / (bounds.vMax - bounds.vMin));
//
//	return size;
//}

OVR_PUBLIC_FUNCTION(ovrEyeRenderDesc) ovr_GetRenderDesc(ovrSession session, ovrEyeType eyeType, ovrFovPort fov)
{
	//REV_TRACE(ovr_GetRenderDesc);

	ovrEyeRenderDesc desc;
	/*desc.Eye = eyeType;
	desc.Fov = fov;

	REV::Matrix4f HmdToEyeMatrix = (REV::Matrix4f)vr::VRSystem()->GetEyeToHeadTransform((vr::EVREye)eyeType);
	float WidthTan = fov.LeftTan + fov.RightTan;
	float HeightTan = fov.UpTan + fov.DownTan;
	ovrSizei size;
	vr::VRSystem()->GetRecommendedRenderTargetSize((uint32_t*)&size.w, (uint32_t*)&size.h);

	desc.DistortedViewport = OVR::Recti(eyeType == ovrEye_Right ? size.w : 0, 0, size.w, size.h);
	desc.PixelsPerTanAngleAtCenter = OVR::Vector2f(size.w / WidthTan, size.h / HeightTan);
	desc.HmdToEyeOffset = HmdToEyeMatrix.GetTranslation();*/

	return desc;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_SubmitFrame(ovrSession session, long long frameIndex, const ovrViewScaleDesc* viewScaleDesc,
	ovrLayerHeader const * const * layerPtrList, unsigned int layerCount)
{
	//REV_TRACE(ovr_SubmitFrame);

	//MICROPROFILE_META_CPU("Submit Frame", (int)frameIndex);

	//// TODO: Implement scaling through ApplyTransform().

	//if (!session || !session->Compositor)
	//	return ovrError_InvalidSession;

	//if (layerCount == 0 || !layerPtrList)
	//	return ovrError_InvalidParameter;

	//// Increment the frame index.
	//if (frameIndex == 0)
	//	session->FrameIndex++;
	//else
	//	session->FrameIndex = frameIndex;

	//// Use our own intermediate compositor to convert the frame to OpenVR.
	//vr::EVRCompositorError err = session->Compositor->SubmitFrame(layerPtrList, layerCount);

	//// Flip the profiler.
	//MicroProfileFlip();

	// The frame has been submitted, so we can now safely refresh some settings from the settings interface.
	//session->LoadSettings();

	// Call WaitGetPoses to block until the running start, also known as queue-ahead in the Oculus SDK.
	/*if (!session->Details->UseHack(SessionDetails::HACK_WAIT_IN_TRACKING_STATE))
		vr::VRCompositor()->WaitGetPoses(nullptr, 0, nullptr, 0);

	vr::VRCompositor()->GetCumulativeStats(&session->Stats[session->FrameIndex % ovrMaxProvidedFrameStats], sizeof(vr::Compositor_CumulativeStats));*/

	//return rev_CompositorErrorToOvrError(err);
	return ovrSuccess;
}

//typedef struct OVR_ALIGNAS(4) ovrPerfStatsPerCompositorFrame1_
//{
//	int     HmdVsyncIndex;
//	int     AppFrameIndex;
//	int     AppDroppedFrameCount;
//	float   AppMotionToPhotonLatency;
//	float   AppQueueAheadTime;
//	float   AppCpuElapsedTime;
//	float   AppGpuElapsedTime;
//	int     CompositorFrameIndex;
//	int     CompositorDroppedFrameCount;
//	float   CompositorLatency;
//	float   CompositorCpuElapsedTime;
//	float   CompositorGpuElapsedTime;
//	float   CompositorCpuStartToGpuEndElapsedTime;
//	float   CompositorGpuEndToVsyncElapsedTime;
//} ovrPerfStatsPerCompositorFrame1;

//typedef struct OVR_ALIGNAS(4) ovrPerfStats1_
//{
//	ovrPerfStatsPerCompositorFrame1  FrameStats[ovrMaxProvidedFrameStats];
//	int                             FrameStatsCount;
//	ovrBool                         AnyFrameStatsDropped;
//	float                           AdaptiveGpuPerformanceScale;
//} ovrPerfStats1;

//OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetPerfStats(ovrSession session, ovrPerfStats* outStats)
//{
//	REV_TRACE(ovr_GetPerfStats);
//
//	ovrPerfStatsPerCompositorFrame FrameStats[ovrMaxProvidedFrameStats];
//
//	// TODO: Implement performance scale heuristics
//	float AdaptiveGpuPerformanceScale = 1.0f;
//	bool AnyFrameStatsDropped = (session->FrameIndex - session->StatsIndex) > ovrMaxProvidedFrameStats;
//	int FrameStatsCount = AnyFrameStatsDropped ? ovrMaxProvidedFrameStats : int(session->FrameIndex - session->StatsIndex);
//	session->StatsIndex = session->FrameIndex;
//
//	float fVsyncToPhotons = vr::VRSystem()->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SecondsFromVsyncToPhotons_Float);
//	float fDisplayFrequency = vr::VRSystem()->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_DisplayFrequency_Float);
//	float fFrameDuration = 1.0f / fDisplayFrequency;
//	for (int i = 0; i < FrameStatsCount; i++)
//	{
//		vr::Compositor_FrameTiming timing;
//		timing.m_nSize = sizeof(vr::Compositor_FrameTiming);
//		if (!vr::VRCompositor()->GetFrameTiming(&timing, i))
//			return ovrError_RuntimeException;
//
//		ovrPerfStatsPerCompositorFrame& stats = FrameStats[i];
//
//		stats.HmdVsyncIndex = timing.m_nFrameIndex - session->ResetStats.HmdVsyncIndex;
//		stats.AppFrameIndex = (int)session->FrameIndex - session->ResetStats.AppFrameIndex;
//		stats.AppDroppedFrameCount = session->Stats[i].m_nNumDroppedFrames - session->ResetStats.AppDroppedFrameCount;
//		// TODO: Improve latency handling with sensor timestamps and latency markers
//		stats.AppMotionToPhotonLatency = fFrameDuration + fVsyncToPhotons;
//		stats.AppQueueAheadTime = timing.m_flCompositorIdleCpuMs / 1000.0f;
//		stats.AppCpuElapsedTime = timing.m_flClientFrameIntervalMs / 1000.0f;
//		stats.AppGpuElapsedTime = timing.m_flPreSubmitGpuMs / 1000.0f;
//
//		stats.CompositorFrameIndex = session->Stats[i].m_nNumFramePresents - session->ResetStats.CompositorFrameIndex;
//		stats.CompositorDroppedFrameCount = (session->Stats[i].m_nNumDroppedFramesOnStartup +
//			session->Stats[i].m_nNumDroppedFramesLoading + session->Stats[i].m_nNumDroppedFramesTimedOut) -
//			session->ResetStats.CompositorDroppedFrameCount;
//		stats.CompositorLatency = fVsyncToPhotons; // OpenVR doesn't have timewarp
//		stats.CompositorCpuElapsedTime = timing.m_flCompositorRenderCpuMs / 1000.0f;
//		stats.CompositorGpuElapsedTime = timing.m_flCompositorRenderGpuMs / 1000.0f;
//		stats.CompositorCpuStartToGpuEndElapsedTime = ((timing.m_flCompositorRenderStartMs + timing.m_flCompositorRenderGpuMs) -
//			timing.m_flNewFrameReadyMs) / 1000.0f;
//		stats.CompositorGpuEndToVsyncElapsedTime = fFrameDuration - timing.m_flTotalRenderGpuMs / 1000.0f;
//
//		// TODO: Asynchronous Spacewap is not supported in OpenVR
//		stats.AswIsActive = ovrFalse;
//		stats.AswActivatedToggleCount = 0;
//		stats.AswPresentedFrameCount = 0;
//		stats.AswFailedFrameCount = 0;
//	}
//
//	// We need to make sure we don't write outside of the bounds of the struct in older version of the runtime
//	if (g_MinorVersion < 11)
//	{
//		ovrPerfStats1* out = (ovrPerfStats1*)outStats;
//		for (int i = 0; i < FrameStatsCount; i++)
//			memcpy(out->FrameStats + i, FrameStats + i, sizeof(ovrPerfStatsPerCompositorFrame1));
//		out->AdaptiveGpuPerformanceScale = AdaptiveGpuPerformanceScale;
//		out->AnyFrameStatsDropped = AnyFrameStatsDropped;
//		out->FrameStatsCount = FrameStatsCount;
//	}
//	else
//	{
//		ovrPerfStats* out = outStats;
//		memcpy(out->FrameStats, FrameStats, sizeof(FrameStats));
//		out->AdaptiveGpuPerformanceScale = AdaptiveGpuPerformanceScale;
//		out->AnyFrameStatsDropped = AnyFrameStatsDropped;
//		out->FrameStatsCount = FrameStatsCount;
//		out->AswIsAvailable = ovrFalse;
//
//		if (g_MinorVersion >= 14)
//			out->VisibleProcessId = vr::VRCompositor()->GetCurrentSceneFocusProcess();
//	}
//
//	return ovrSuccess;
//}

//OVR_PUBLIC_FUNCTION(ovrResult) ovr_ResetPerfStats(ovrSession session)
//{
//	REV_TRACE(ovr_ResetPerfStats);
//
//	ovrPerfStats perfStats = { 0 };
//	ovrResult result = ovr_GetPerfStats(session, &perfStats);
//	if (OVR_SUCCESS(result))
//		session->ResetStats = perfStats.FrameStats[0];
//	return result;
//}

//OVR_PUBLIC_FUNCTION(double) ovr_GetPredictedDisplayTime(ovrSession session, long long frameIndex)
//{
//	REV_TRACE(ovr_GetPredictedDisplayTime);
//
//	MICROPROFILE_META_CPU("Predict Frame", (int)frameIndex);
//
//	if (!session)
//		return ovrError_InvalidSession;
//
//	float fDisplayFrequency = vr::VRSystem()->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_DisplayFrequency_Float);
//	float fVsyncToPhotons = vr::VRSystem()->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SecondsFromVsyncToPhotons_Float);
//
//	// Get the frame count and advance it based on which frame we're predicting
//	float fSecondsSinceLastVsync;
//	uint64_t unFrame;
//	vr::VRSystem()->GetTimeSinceLastVsync(&fSecondsSinceLastVsync, &unFrame);
//
//	// Round up to the next frame so we predict far enough
//	unFrame++;
//
//	// Advance the frame count based on how many frames we're predicting ahead
//	if (frameIndex == 0)
//		unFrame++;
//	else
//		unFrame += frameIndex - session->FrameIndex;
//
//	// Predict the display time based on the display frequency and the vsync-to-photon latency
//	return double(unFrame) / fDisplayFrequency + fVsyncToPhotons;
//	double time = ovr_GetTimeInSeconds();
//	return time;
//}

//OVR_PUBLIC_FUNCTION(double) ovr_GetTimeInSeconds()
//{
//	REV_TRACE(ovr_GetTimeInSeconds);
//
//	float fDisplayFrequency = vr::VRSystem()->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_DisplayFrequency_Float);
//
//	// Get the frame count and the time since the last VSync
//	float fSecondsSinceLastVsync;
//	uint64_t unFrame;
//	vr::VRSystem()->GetTimeSinceLastVsync(&fSecondsSinceLastVsync, &unFrame);
//
//	// Calculate the time since the first frame based on the display frequency
//	return double(unFrame) / fDisplayFrequency + fSecondsSinceLastVsync;
//	double time = 0;
//	return time;
//}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_GetBool(ovrSession session, const char* propertyName, ovrBool defaultVal)
{
//	//REV_TRACE(ovr_GetBool);
//
//	vr::EVRSettingsError error;
//	ovrBool result = vr::VRSettings()->GetBool(REV_SETTINGS_SECTION, propertyName, &error);
//	return (error == vr::VRSettingsError_None) ? result : defaultVal;
	return defaultVal;
}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_SetBool(ovrSession session, const char* propertyName, ovrBool value)
{
//	REV_TRACE(ovr_SetBool);
//
//	vr::EVRSettingsError error;
//	vr::VRSettings()->SetBool(REV_SETTINGS_SECTION, propertyName, !!value, &error);
//	vr::VRSettings()->Sync();
//	return error == vr::VRSettingsError_None;
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(int) ovr_GetInt(ovrSession session, const char* propertyName, int defaultVal)
{
//	//REV_TRACE(ovr_GetInt);
//
//	/*if (strcmp("TextureSwapChainDepth", propertyName) == 0)
//		return REV_SWAPCHAIN_LENGTH;*/
//
//	vr::EVRSettingsError error;
//	int result = vr::VRSettings()->GetInt32(REV_SETTINGS_SECTION, propertyName, &error);
//	return (error == vr::VRSettingsError_None) ? result : defaultVal;
	return defaultVal;
}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_SetInt(ovrSession session, const char* propertyName, int value)
{
//	REV_TRACE(ovr_SetInt);
//
//	vr::EVRSettingsError error;
//	vr::VRSettings()->SetInt32(REV_SETTINGS_SECTION, propertyName, value, &error);
//	vr::VRSettings()->Sync();
//	return error == vr::VRSettingsError_None;
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(float) ovr_GetFloat(ovrSession session, const char* propertyName, float defaultVal)
{
//	//REV_TRACE(ovr_GetFloat);
//
//	if (strcmp(propertyName, "IPD") == 0)
//		return vr::VRSystem()->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_UserIpdMeters_Float);
//
//	// Override defaults, we should always return a valid value for these
//	/*if (strcmp(propertyName, OVR_KEY_PLAYER_HEIGHT) == 0)
//		defaultVal = OVR_DEFAULT_PLAYER_HEIGHT;
//	else if (strcmp(propertyName, OVR_KEY_EYE_HEIGHT) == 0)
//		defaultVal = OVR_DEFAULT_EYE_HEIGHT;*/
//
//	vr::EVRSettingsError error;
//	float result = vr::VRSettings()->GetFloat(REV_SETTINGS_SECTION, propertyName, &error);
//	return (error == vr::VRSettingsError_None) ? result : defaultVal;
	return defaultVal;
}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_SetFloat(ovrSession session, const char* propertyName, float value)
{
//	REV_TRACE(ovr_SetFloat);
//
//	vr::EVRSettingsError error;
//	vr::VRSettings()->SetFloat(REV_SETTINGS_SECTION, propertyName, value, &error);
//	vr::VRSettings()->Sync();
//	return error == vr::VRSettingsError_None;
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(unsigned int) ovr_GetFloatArray(ovrSession session, const char* propertyName, float values[], unsigned int valuesCapacity)
{
//	//REV_TRACE(ovr_GetFloatArray);
//
//	//if (strcmp(propertyName, OVR_KEY_NECK_TO_EYE_DISTANCE) == 0)
//	//{
//	//	if (valuesCapacity < 2)
//	//		return 0;
//
//	//	// We only know the horizontal depth
//	//	values[0] = vr::VRSystem()->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_UserHeadToEyeDepthMeters_Float);
//	//	values[1] = OVR_DEFAULT_NECK_TO_EYE_VERTICAL;
//	//	return 2;
//	//}
//
//	char key[vr::k_unMaxSettingsKeyLength] = { 0 };
//
//	for (unsigned int i = 0; i < valuesCapacity; i++)
//	{
//		vr::EVRSettingsError error;
//		snprintf(key, vr::k_unMaxSettingsKeyLength, "%s[%d]", propertyName, i);
//		values[i] = vr::VRSettings()->GetFloat(REV_SETTINGS_SECTION, key, &error);
//
//		if (error != vr::VRSettingsError_None)
//			return i;
//	}
//
//	return valuesCapacity;
	return valuesCapacity;
}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_SetFloatArray(ovrSession session, const char* propertyName, const float values[], unsigned int valuesSize)
{
//	REV_TRACE(ovr_SetFloatArray);
//
//	char key[vr::k_unMaxSettingsKeyLength] = { 0 };
//
//	for (unsigned int i = 0; i < valuesSize; i++)
//	{
//		vr::EVRSettingsError error;
//		snprintf(key, vr::k_unMaxSettingsKeyLength, "%s[%d]", propertyName, i);
//		vr::VRSettings()->SetFloat(REV_SETTINGS_SECTION, key, values[i], &error);
//
//		if (error != vr::VRSettingsError_None)
//			return false;
//	}
//
//	vr::VRSettings()->Sync();
//	return true;
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(const char*) ovr_GetString(ovrSession session, const char* propertyName, const char* defaultVal)
{
//	//REV_TRACE(ovr_GetString);
//
//	if (!session)
//		return defaultVal;
//
//	// Override defaults, we should always return a valid value for these
//	/*if (strcmp(propertyName, OVR_KEY_GENDER) == 0)
//		defaultVal = OVR_DEFAULT_GENDER;*/
//
//	vr::EVRSettingsError error;
//	vr::VRSettings()->GetString(REV_SETTINGS_SECTION, propertyName, session->StringBuffer, vr::k_unMaxPropertyStringSize, &error);
//	return (error == vr::VRSettingsError_None) ? session->StringBuffer : defaultVal;
	return defaultVal;
}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_SetString(ovrSession session, const char* propertyName, const char* value)
{
//	REV_TRACE(ovr_SetString);
//
//	vr::EVRSettingsError error;
//	vr::VRSettings()->SetString(REV_SETTINGS_SECTION, propertyName, value, &error);
//	vr::VRSettings()->Sync();
//	return error == vr::VRSettingsError_None;
	return ovrSuccess;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_Lookup(const char* name, void** data)
{
	// We don't communicate with the Oculus service.
	return ovrError_ServiceError;
	//return ovrSuccess;
}
