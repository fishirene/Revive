#include "InputManager.h"
#include "Session.h"

#include "OVR_CAPI.h"
#include "REV_Math.h"

#include <Windows.h>

InputManager::InputManager()
	: m_TouchL(new OculusTouch(ovrControllerType_LTouch))
	, m_TouchR(new OculusTouch(ovrControllerType_RTouch))
	, m_LastState({ 0 })
{
	m_LastState.ControllerType = (ovrControllerType)GetConnectedControllerTypes();
}

InputManager::~InputManager()
{
	delete m_TouchL;
	delete m_TouchR;
}

unsigned int InputManager::GetConnectedControllerTypes()
{
	unsigned int types = 0;

	types |= ovrControllerType_LTouch | ovrControllerType_RTouch;

	return types;
}

ovrResult InputManager::GetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState* inputState)
{
	*inputState = m_LastState;

	return ovrSuccess;
}

void InputManager::GetTrackingState(ovrSession session, ovrTrackingState* outState, double absTime)
{
	outState->HandPoses[0] = m_TouchL->GetPose(absTime);
	outState->HandPoses[1] = m_TouchR->GetPose(absTime);
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
			outDevicePoses[i] = m_TouchL->GetPose(absTime);
			break;
		case ovrTrackedDevice_RTouch:
			outDevicePoses[i] = m_TouchR->GetPose(absTime);
			break;
		default:
			break;
		}
	}

	return ovrSuccess;
}

void InputManager::EmulateResetTouchesPose()
{
	delete m_TouchL;
	delete m_TouchR;
	m_TouchL = new OculusTouch(ovrControllerType_LTouch);
	m_TouchR = new OculusTouch(ovrControllerType_RTouch);
	m_LastState = { 0 };
	m_LastState.ControllerType = (ovrControllerType)GetConnectedControllerTypes();
}

void InputManager::EmulateTouchesPositionOffset(ovrControllerType controllerType, float x, float y)
{
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

void InputManager::EmulateTouchesOrientationOffset(ovrControllerType controllerType, float x, float y, float z, float w)
{
	//m_EmulatedOffset.Orientation;
}

void InputManager::EmulateTouchesInputState(unsigned int touchKey, bool state, float value, float x, float y)
{
	ovrVector2f thumbstick;
	switch (touchKey)
	{
	case ovrTouch_A:
		if (state)
			m_LastState.Touches |= ovrTouch_A;
		else
			m_LastState.Touches &= ~ovrTouch_A;
		break;
	case ovrTouch_B:
		if (state)
			m_LastState.Touches |= ovrTouch_B;
		else
			m_LastState.Touches &= ~ovrTouch_B;
		break;
	case ovrTouch_RThumb:
		if (state)
			m_LastState.Touches |= ovrTouch_RThumb;
		else
			m_LastState.Touches &= ~ovrTouch_RThumb;
		break;
	case ovrTouch_RThumbRest:
		if (state)
			m_LastState.Touches |= ovrTouch_RThumbRest;
		else
			m_LastState.Touches &= ~ovrTouch_RThumbRest;
		break;
	case ovrTouch_X:
		if (state)
			m_LastState.Touches |= ovrTouch_X;
		else
			m_LastState.Touches &= ~ovrTouch_X;
		break;
	case ovrTouch_Y:
		if (state)
			m_LastState.Touches |= ovrTouch_Y;
		else
			m_LastState.Touches &= ~ovrTouch_Y;
		break;
	case ovrTouch_LThumb:
		if (state)
			m_LastState.Touches |= ovrTouch_LThumb;
		else
			m_LastState.Touches &= ~ovrTouch_LThumb;
		break;
	case ovrTouch_LThumbRest:
		if (state)
			m_LastState.Touches |= ovrTouch_LThumbRest;
		else
			m_LastState.Touches &= ~ovrTouch_LThumbRest;
		break;
	case ovrTouch_RIndexTrigger:
		if (state)
		{
			m_LastState.IndexTrigger[ovrHand_Right] = value;
			m_LastState.IndexTriggerNoDeadzone[ovrHand_Right] = value;
			m_LastState.IndexTriggerRaw[ovrHand_Right] = value;
			if(value > 0.1176f)
				m_LastState.Touches |= ovrTouch_RIndexTrigger;
			else
				m_LastState.Touches &= ~ovrTouch_RIndexTrigger;
		}	
		break;
	case ovrTouch_LIndexTrigger:		
		if (state)
		{
			m_LastState.IndexTrigger[ovrHand_Left] = value;
			m_LastState.IndexTriggerNoDeadzone[ovrHand_Left] = value;
			m_LastState.IndexTriggerRaw[ovrHand_Left] = value;
			if (value > 0.1176f)
				m_LastState.Touches |= ovrTouch_LIndexTrigger;
			else
				m_LastState.Touches &= ~ovrTouch_LIndexTrigger;
		}
		break;
	case ovrTouch_LIndexPointing:
		if (state)
			m_LastState.Touches |= ovrTouch_LIndexPointing;
		else
			m_LastState.Touches &= ~ovrTouch_LIndexPointing;
		break;
	case ovrTouch_LThumbUp:
		if (state)
			m_LastState.Touches |= ovrTouch_LThumbUp;
		else
			m_LastState.Touches &= ~ovrTouch_LThumbUp;
		break;
	case ovrTouch_RThumbUp:
		if (state)
			m_LastState.Touches |= ovrTouch_RThumbUp;
		else
			m_LastState.Touches &= ~ovrTouch_RThumbUp;
		break;
	case ovrTouch_RIndexPointing:
		if (state)
			m_LastState.Touches |= ovrTouch_RIndexPointing;
		else
			m_LastState.Touches &= ~ovrTouch_RIndexPointing;
		break;
	case ovrTouch_LThumbstick:
		thumbstick.x = x;
		thumbstick.y = y;
		m_LastState.Thumbstick[ovrHand_Left] = thumbstick;
		m_LastState.ThumbstickNoDeadzone[ovrHand_Left] = thumbstick;
		m_LastState.ThumbstickRaw[ovrHand_Left] = thumbstick;
		break;
	case ovrTouch_LHandTrigger:
		m_LastState.HandTrigger[ovrHand_Left] = value;
		m_LastState.HandTriggerNoDeadzone[ovrHand_Left] = value;
		m_LastState.HandTriggerRaw[ovrHand_Left] = value;
		break;
	case ovrTouch_RThumbstick:
		thumbstick.x = x;
		thumbstick.y = y;
		m_LastState.Thumbstick[ovrHand_Right] = thumbstick;
		m_LastState.ThumbstickNoDeadzone[ovrHand_Right] = thumbstick;
		m_LastState.ThumbstickRaw[ovrHand_Right] = thumbstick;
		break;
	case ovrTouch_RHandTrigger:
		m_LastState.HandTrigger[ovrHand_Right] = value;
		m_LastState.HandTriggerNoDeadzone[ovrHand_Right] = value;
		m_LastState.HandTriggerRaw[ovrHand_Right] = value;
		break;
	default:
		break;
	}
}

InputManager::OculusTouch::OculusTouch(ovrControllerType role)
	: ControllerType(role)
	, m_LastPose()
	, m_HeadHandOffset()
	, m_EmulatedOffset()
{
	m_LastPose = { OVR::Posef::Identity() };
	m_EmulatedOffset = { OVR::Posef::Identity() };

	switch (role)
	{
	case ovrControllerType_LTouch:
		m_HeadHandOffset = { REV_DEFAULT_HEAD_HAND_OFFSET_L_X, REV_DEFAULT_HEAD_HAND_OFFSET_L_Y, REV_DEFAULT_HEAD_HAND_OFFSET_L_Z };
		break;
	case ovrControllerType_RTouch:
		m_HeadHandOffset = { REV_DEFAULT_HEAD_HAND_OFFSET_R_X, REV_DEFAULT_HEAD_HAND_OFFSET_R_Y, REV_DEFAULT_HEAD_HAND_OFFSET_R_Z };
		break;
	default:
		break;
	}
}

unsigned int InputManager::OculusTouch::GetStatusFlag()
{
	unsigned int result = 0;

	result |= ovrStatus_OrientationTracked | ovrStatus_PositionTracked;

	return result;
}

ovrPoseStatef InputManager::OculusTouch::GetPose(double absTime)
{
	m_LastPose.TimeInSeconds = absTime;

	m_LastPose.ThePose.Position.x = m_LastPose.ThePose.Position.x + m_EmulatedOffset.ThePose.Position.x + m_HeadHandOffset.x;
	m_LastPose.ThePose.Position.y = m_LastPose.ThePose.Position.y + m_EmulatedOffset.ThePose.Position.y + m_HeadHandOffset.y;
	m_LastPose.ThePose.Position.z = m_LastPose.ThePose.Position.z + m_EmulatedOffset.ThePose.Position.z + m_HeadHandOffset.z;

	return m_LastPose;
}

void InputManager::OculusTouch::EmulateTouchPositionOffset(float x, float y)
{
	m_EmulatedOffset.ThePose.Position.x = x;
	m_EmulatedOffset.ThePose.Position.y = y;
	m_EmulatedOffset.ThePose.Position.z = 0;
}

void InputManager::OculusTouch::EmulateTouchOrientationOffset(float x, float y, float z, float w)
{
	//m_EmulatedOffset.Orientation;
}