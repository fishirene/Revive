#include "InputManager.h"

InputManager* InputManager::m_Instance = 0;

OculusTouch::OculusTouch(ovrControllerType role)
	: ControllerType(role)
	, m_LastOffset({ OVR::Posef::Identity() })
	, m_HeadHandPositionOffset()
{
	switch (role)
	{
	case ovrControllerType_LTouch:
		m_HeadHandPositionOffset = { VT_DEFAULT_HEAD_HAND_OFFSET_L_X, VT_DEFAULT_HEAD_HAND_OFFSET_L_Y, VT_DEFAULT_HEAD_HAND_OFFSET_L_Z };
		break;
	case ovrControllerType_RTouch:
		m_HeadHandPositionOffset = { VT_DEFAULT_HEAD_HAND_OFFSET_R_X, VT_DEFAULT_HEAD_HAND_OFFSET_R_Y, VT_DEFAULT_HEAD_HAND_OFFSET_R_Z };
		break;
	default:
		break;
	}

	m_LastOffset.ThePose.Position = m_HeadHandPositionOffset;
}

unsigned int OculusTouch::GetStatusFlag()
{
	unsigned int result = 0;

	result |= ovrStatus_OrientationTracked | ovrStatus_PositionTracked;

	return result;
}

ovrPoseStatef OculusTouch::GetOffset(double absTime)
{
	m_LastOffset.TimeInSeconds = absTime;

	return m_LastOffset;
}

void OculusTouch::EmulateTouchPositionOffset(float x, float y)
{
	OVR::Vector3f tmpOffset(x, y, 0);
	OVR::Vector3f offset = m_LastOffset.ThePose.Position;
	offset += tmpOffset;
	m_LastOffset.ThePose.Position = offset;
}

void OculusTouch::EmulateTouchOrientationOffset(unsigned int axis, float degree)
{
	float radian = OVR::DegreeToRad(degree);
	OVR::Quatf tmpOffset = OVR::Quat<float>((OVR::Axis)axis, radian, OVR::Rotate_CCW, OVR::Handed_R);
	OVR::Quatf offset = m_LastOffset.ThePose.Orientation;
	offset *= tmpOffset;
	m_LastOffset.ThePose.Orientation = offset;
}

InputManager::InputManager()
	: m_TouchL(new OculusTouch(ovrControllerType_LTouch))
	, m_TouchR(new OculusTouch(ovrControllerType_RTouch))
	, m_LastState(new ovrInputState())
{
	m_LastState->ControllerType = (ovrControllerType)GetConnectedControllerTypes();

	WCHAR LogPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, LogPath)))
	{
		wcsncat(LogPath, L"\\VTouch", MAX_PATH);

		BOOL exists = PathFileExists(LogPath);
		if (!exists)
			exists = CreateDirectory(LogPath, NULL);

		wcsncat(LogPath, L"\\VTouch.txt", MAX_PATH);
		if (exists)
			m_VTouchLog = _wfopen(LogPath, L"w");
	}
}

InputManager::~InputManager()
{
	delete m_Instance;
	delete m_TouchL;
	delete m_TouchR;
	delete m_LastState;
}

InputManager* InputManager::GetInstance() {
	if (!m_Instance) {
		m_Instance = new InputManager();
		return m_Instance;
	}
	else {
		return m_Instance;
	}
}

unsigned int InputManager::GetConnectedControllerTypes()
{
	unsigned int types = 0;

	types |= ovrControllerType_LTouch | ovrControllerType_RTouch;

	return types;
}

void InputManager::EmulateResetTouchesPose()
{
	delete m_TouchL;
	delete m_TouchR;
	m_TouchL = new OculusTouch(ovrControllerType_LTouch);
	m_TouchR = new OculusTouch(ovrControllerType_RTouch);
	m_LastState = new ovrInputState();
	m_LastState->ControllerType = (ovrControllerType)GetConnectedControllerTypes();
}

void InputManager::EmulateTouchesInputState(unsigned int touchKey, bool state)
{
	switch (touchKey)
	{
	case ovrTouch_A:
		if (state)
		{
			m_LastState->Buttons |= ovrButton_A;
			m_LastState->Touches |= ovrTouch_A;
		}
		else
		{
			m_LastState->Buttons &= ~ovrButton_A;
			m_LastState->Touches &= ~ovrTouch_A;
		}
		break;
	case ovrTouch_B:
		if (state)
		{
			m_LastState->Buttons |= ovrButton_B;
			m_LastState->Touches |= ovrTouch_B;
		}
		else
		{
			m_LastState->Buttons &= ~ovrButton_B;
			m_LastState->Touches &= ~ovrTouch_B;
		}
		break;
	case ovrTouch_X:
		if (state)
		{
			m_LastState->Buttons |= ovrButton_X;
			m_LastState->Touches |= ovrTouch_X;
		}
		else
		{
			m_LastState->Buttons &= ~ovrButton_X;
			m_LastState->Touches &= ~ovrTouch_X;
		}
		break;
	case ovrTouch_Y:
		if (state)
		{
			m_LastState->Buttons |= ovrButton_Y;
			m_LastState->Touches |= ovrTouch_Y;
		}
		else
		{
			m_LastState->Buttons &= ~ovrButton_Y;
			m_LastState->Touches &= ~ovrTouch_Y;
		}
		break;
	case ovrTouch_LThumbRest:
		if (state)
			m_LastState->Touches |= ovrTouch_LThumbRest;
		else
			m_LastState->Touches &= ~ovrTouch_LThumbRest;
		break;
	case ovrTouch_RThumbRest:
		if (state)
			m_LastState->Touches |= ovrTouch_RThumbRest;
		else
			m_LastState->Touches &= ~ovrTouch_RThumbRest;
		break;
	case ovrTouch_LThumb:
		if (state)
		{
			m_LastState->Buttons |= ovrButton_LThumb;
			m_LastState->Touches |= ovrTouch_LThumb;
		}
		else
		{
			m_LastState->Buttons &= ~ovrButton_LThumb;
			m_LastState->Touches &= ~ovrTouch_LThumb;
		}
		break;
	case ovrTouch_RThumb:
		if (state)
		{
			m_LastState->Buttons |= ovrButton_RThumb;
			m_LastState->Touches |= ovrTouch_RThumb;
		}
		else
		{
			m_LastState->Buttons &= ~ovrButton_RThumb;
			m_LastState->Touches &= ~ovrTouch_RThumb;
		}
		break;
	case ovrTouch_LThumbUp:
		if (state)
			m_LastState->Touches |= ovrTouch_LThumbUp;
		else
			m_LastState->Touches &= ~ovrTouch_LThumbUp;
		break;
	case ovrTouch_RThumbUp:
		if (state)
			m_LastState->Touches |= ovrTouch_RThumbUp;
		else
			m_LastState->Touches &= ~ovrTouch_RThumbUp;
		break;
	case ovrTouch_LIndexPointing:
		if (state)
			m_LastState->Touches |= ovrTouch_LIndexPointing;
		else
			m_LastState->Touches &= ~ovrTouch_LIndexPointing;
		break;
	case ovrTouch_RIndexPointing:
		if (state)
			m_LastState->Touches |= ovrTouch_RIndexPointing;
		else
			m_LastState->Touches &= ~ovrTouch_RIndexPointing;
		break;
	default:
		break;
	}
	//LogRevive("m_LastState Getinput ovrTouch_LHandTrigger when emulation: %u\n", m_LastState->Touches);
}

void InputManager::EmulateTouchesInputState(unsigned int touchKey, float value)
{
	switch (touchKey)
	{
	case ovrTouch_LHandTrigger:
		m_LastState->HandTrigger[ovrHand_Left] = value;
		m_LastState->HandTriggerNoDeadzone[ovrHand_Left] = value;
		m_LastState->HandTriggerRaw[ovrHand_Left] = value;
		break;
	case ovrTouch_RHandTrigger:
		m_LastState->HandTrigger[ovrHand_Right] = value;
		m_LastState->HandTriggerNoDeadzone[ovrHand_Right] = value;
		m_LastState->HandTriggerRaw[ovrHand_Right] = value;
		break;
	case ovrTouch_LIndexTrigger:
		m_LastState->IndexTrigger[ovrHand_Left] = value;
		m_LastState->IndexTriggerNoDeadzone[ovrHand_Left] = value;
		m_LastState->IndexTriggerRaw[ovrHand_Left] = value;
		if (value > VT_TRIGGER_THRESHOLD)
			m_LastState->Touches |= ovrTouch_LIndexTrigger;
		else
			m_LastState->Touches &= ~ovrTouch_LIndexTrigger;
		break;
	case ovrTouch_RIndexTrigger:
		m_LastState->IndexTrigger[ovrHand_Right] = value;
		m_LastState->IndexTriggerNoDeadzone[ovrHand_Right] = value;
		m_LastState->IndexTriggerRaw[ovrHand_Right] = value;
		if (value > VT_TRIGGER_THRESHOLD)
			m_LastState->Touches |= ovrTouch_RIndexTrigger;
		else
			m_LastState->Touches &= ~ovrTouch_RIndexTrigger;
		break;
	default:
		break;
	}
}

void InputManager::EmulateTouchesInputState(unsigned int touchKey, float x, float y)
{
	ovrVector2f thumbstick;
	switch (touchKey)
	{
	case ovrTouch_LThumbstick:
		thumbstick.x = x;
		thumbstick.y = y;
		m_LastState->Thumbstick[ovrHand_Left] = thumbstick;
		m_LastState->ThumbstickNoDeadzone[ovrHand_Left] = thumbstick;
		m_LastState->ThumbstickRaw[ovrHand_Left] = thumbstick;
		break;
	case ovrTouch_RThumbstick:
		thumbstick.x = x;
		thumbstick.y = y;
		m_LastState->Thumbstick[ovrHand_Right] = thumbstick;
		m_LastState->ThumbstickNoDeadzone[ovrHand_Right] = thumbstick;
		m_LastState->ThumbstickRaw[ovrHand_Right] = thumbstick;
		break;
	default:
		break;
	}
}

void InputManager::EmulateTouchesPositionOffset(unsigned int controllerType, float x, float y)
{
	controllerType = (ovrControllerType)controllerType;
	switch (controllerType)
	{
	case ovrControllerType_LTouch:
		m_TouchL->EmulateTouchPositionOffset(x, y);
		break;
	case ovrControllerType_RTouch:
		m_TouchR->EmulateTouchPositionOffset(x, y);
		break;
	}
}

void InputManager::EmulateTouchesOrientationOffset(unsigned int controllerType, unsigned int axis, float radian)
{
	controllerType = (ovrControllerType)controllerType;
	switch (controllerType)
	{
	case ovrControllerType_LTouch:
		m_TouchL->EmulateTouchOrientationOffset(axis, radian);
		break;
	case ovrControllerType_RTouch:
		m_TouchR->EmulateTouchOrientationOffset(axis, radian);
		break;
	}
}

ovrResult InputManager::GetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState* inputState)
{
	//LogRevive("m_LastState Getinput ovrTouch_LHandTrigger: %g\n", m_LastState->HandTrigger[0]);
	inputState->Buttons = m_LastState->Buttons;
	inputState->Touches = m_LastState->Touches;
	inputState->ControllerType = m_LastState->ControllerType;
	memcpy(inputState->IndexTrigger, m_LastState->IndexTrigger, sizeof(inputState->IndexTrigger));
	memcpy(inputState->HandTrigger, m_LastState->HandTrigger, sizeof(inputState->HandTrigger));
	memcpy(inputState->Thumbstick, m_LastState->Thumbstick, sizeof(inputState->Thumbstick));
	memcpy(inputState->IndexTriggerNoDeadzone, m_LastState->IndexTriggerNoDeadzone, sizeof(inputState->IndexTriggerNoDeadzone));
	memcpy(inputState->HandTriggerNoDeadzone, m_LastState->HandTriggerNoDeadzone, sizeof(inputState->HandTriggerNoDeadzone));
	memcpy(inputState->ThumbstickNoDeadzone, m_LastState->ThumbstickNoDeadzone, sizeof(inputState->ThumbstickNoDeadzone));
	memcpy(inputState->IndexTriggerRaw, m_LastState->IndexTriggerRaw, sizeof(inputState->IndexTriggerRaw));
	inputState->HandTriggerRaw[0] = m_LastState->HandTriggerRaw[0];

	//This crash unreal engine
	//OVR::Vector2f thumbstickRaw = m_LastState->ThumbstickRaw[0];
	//inputState->ThumbstickRaw[0] = thumbstickRaw;
	//thumbstickRaw = m_LastState->ThumbstickRaw[1];
	//inputState->ThumbstickRaw[1] = thumbstickRaw;
	//This crash unreal engine as well
	//inputState->HandTriggerRaw[1] = m_LastState->HandTriggerRaw[1];
	// This 2 copy will cause crash.
	//memcpy(inputState->HandTriggerRaw, m_LastState->HandTriggerRaw, sizeof(inputState->HandTriggerRaw));
	//memcpy(inputState->ThumbstickRaw, m_LastState->ThumbstickRaw, sizeof(inputState->ThumbstickRaw));

	return ovrSuccess;
}

void InputManager::GetTrackingState(ovrSession session, ovrTrackingState* outState, double absTime)
{
	OVR::Vector3f headPosition = outState->HeadPose.ThePose.Position;
	OVR::Quatf orientation = outState->HeadPose.ThePose.Orientation;
	OVR::Vector3f lPOffset = m_TouchL->GetOffset(absTime).ThePose.Position;
	OVR::Vector3f rPOffset = m_TouchR->GetOffset(absTime).ThePose.Position;
	OVR::Quatf lQOffset = m_TouchL->GetOffset(absTime).ThePose.Orientation;
	OVR::Quatf rQOffset = m_TouchR->GetOffset(absTime).ThePose.Orientation;

	float yaw, pitch, roll;
	orientation.GetYawPitchRoll(&yaw, &pitch, &roll);

	//OVR::Matrix4f rotationMatrix = rotationMatrix.RotationY(yaw);
	//lPOffset = rotationMatrix.Transform(lPOffset);
	//rPOffset = rotationMatrix.Transform(rPOffset);

	lPOffset = orientation.Rotate(lPOffset);
	rPOffset = orientation.Rotate(rPOffset);
	outState->HandPoses[0].ThePose.Position = headPosition + lPOffset;
	outState->HandPoses[1].ThePose.Position = headPosition + rPOffset;

	//OVR::Quatf rotationQuat = OVR::Quat<float>(OVR::Axis_Y, yaw, OVR::Rotate_CCW, OVR::Handed_R);
	//outState->HandPoses[0].ThePose.Orientation = rotationQuat * lQOffset;
	//outState->HandPoses[1].ThePose.Orientation = rotationQuat * rQOffset;

	outState->HandPoses[0].ThePose.Orientation = orientation * lQOffset;
	outState->HandPoses[1].ThePose.Orientation = orientation * rQOffset;

	outState->HandStatusFlags[0] = m_TouchL->GetStatusFlag();
	outState->HandStatusFlags[1] = m_TouchR->GetStatusFlag();
}

ovrResult InputManager::GetDevicePoses(ovrTrackedDeviceType* deviceTypes, int deviceCount, double absTime, ovrPoseStatef* outDevicePoses)
{
	for (int i = 0; i < deviceCount; i++)
	{
		switch (deviceTypes[i])
		{
		case ovrTrackedDevice_LTouch:
			outDevicePoses[i] = m_TouchL->GetOffset(absTime);
			break;
		case ovrTrackedDevice_RTouch:
			outDevicePoses[i] = m_TouchR->GetOffset(absTime);
			break;
		default:
			break;
		}
	}

	return ovrSuccess;
}
