#include "InputManager.h"
#include "Session.h"

#include "OVR_CAPI.h"
#include "REV_Math.h"

#include <openvr.h>
#include <Windows.h>

InputManager::InputManager()
	//: m_InputDevices()
	: m_TouchL(new OculusTouch(ovrControllerType_LTouch))
	, m_TouchR(new OculusTouch(ovrControllerType_RTouch))
{
	/*m_InputDevices.push_back(new OculusTouch(ovrControllerType_LTouch));
	m_InputDevices.push_back(new OculusTouch(ovrControllerType_RTouch));*/
	//m_TouchL = new OculusTouch(ovrControllerType_LTouch);
}

InputManager::~InputManager()
{
	/*for (OculusTouch* device : m_InputDevices)
		delete device;*/
	delete m_TouchL;
	delete m_TouchR;
}

unsigned int InputManager::GetConnectedControllerTypes()
{
	uint32_t types = 0;
	/*for (OculusTouch* device : m_InputDevices)
	{
		if (device->ControllerType != ovrControllerType_None)
			types |= device->ControllerType;
	}*/

	types |= m_TouchL->ControllerType;
	types |= m_TouchR->ControllerType;

	return types;
}

//bool InputManager::IsConnected()
//{
//	bool touchs_connected = true;
//	for (OculusTouch* device : m_InputDevices)
//	{
//		if (device->TrackedDeviceType != ovrTrackedDevice_None)
//		{
//			touchs_connected = false;
//			return touchs_connected;
//		}
//	}
//	return touchs_connected;
//}

ovrResult InputManager::GetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState* inputState)
{
	memset(inputState, 0, sizeof(ovrInputState));

	//uint32_t types = 0;
	/*for (OculusTouch* device : m_InputDevices)
	{
		if (controllerType & device->ControllerType)
		{
			if (device->GetInputState(session, inputState))
				types |= device->ControllerType;
		}
	}*/
	/*types |= m_TouchL->ControllerType;
	types |= m_TouchR->ControllerType;*/

	inputState->ControllerType = (ovrControllerType)GetConnectedControllerTypes();
	return ovrSuccess;
}

void InputManager::GetTrackingState(ovrSession session, ovrTrackingState* outState, double absTime)
{
	/*if (IsConnected())
	{*/
	/*for (int i = 0; i < ovrHand_Count; i++)
	{
		outState->HandPoses[i] = m_InputDevices[i]->GetPose(absTime);
		outState->HandStatusFlags[i] = m_InputDevices[i]->GetStatusFlag();
	}*/
	//}
	outState->HandPoses[0] = m_TouchL->GetPose(absTime);
	outState->HandPoses[1] = m_TouchR->GetPose(absTime);
	outState->HandStatusFlags[0] = m_TouchL->GetStatusFlag();
	outState->HandStatusFlags[1] = m_TouchR->GetStatusFlag();
}

ovrResult InputManager::GetDevicePoses(ovrTrackedDeviceType* deviceTypes, int deviceCount, double absTime, ovrPoseStatef* outDevicePoses)
{
	//float relTime = float(absTime - ovr_GetTimeInSeconds());

	//if (IsConnected())
	//{
	for (int i = 0; i < deviceCount; i++)
	{
		if (deviceTypes[i] == ovrTrackedDevice_LTouch)
		{
			/*for (OculusTouch* device : m_InputDevices)
			{
				if (device->ControllerType == ovrControllerType_LTouch)
				{
					outDevicePoses[i] = device->GetPose(absTime);
				}
			}*/
			outDevicePoses[i] = m_TouchL->GetPose(absTime);
		}
		else if (deviceTypes[i] == ovrTrackedDevice_RTouch)
		{
			/*for (OculusTouch* device : m_InputDevices)
			{
				if (device->ControllerType == ovrControllerType_RTouch)
				{
					outDevicePoses[i] = device->GetPose(absTime);
				}
			}*/
			outDevicePoses[i] = m_TouchR->GetPose(absTime);
		}
	}
	//}
	return ovrSuccess;
}

void InputManager::EmulateTouchsPositionOffset(ovrControllerType controllerType, float x, float y)
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

void InputManager::EmulateTouchsOrientationOffset(ovrControllerType controllerType, float x, float y, float z, float w)
{
	//m_EmulatedOffset.Orientation;
}

InputManager::OculusTouch::OculusTouch(ovrControllerType role)
	: ControllerType(role)
	, m_LastState()
	, m_LastPose()
	, m_HeadHandOffset()
	, m_EmulatedOffset()
{
	m_LastPose = { OVR::Posef::Identity() };
	m_EmulatedOffset = { OVR::Posef::Identity() };
	if (role == ovrControllerType_LTouch) 
	{
		m_HeadHandOffset = { REV_DEFAULT_HEAD_HAND_OFFSET_L_X, REV_DEFAULT_HEAD_HAND_OFFSET_L_Y, REV_DEFAULT_HEAD_HAND_OFFSET_L_Z };
		m_TrackedDeviceType = ovrTrackedDevice_LTouch;
	}
	else
	{
		m_HeadHandOffset = { REV_DEFAULT_HEAD_HAND_OFFSET_R_X, REV_DEFAULT_HEAD_HAND_OFFSET_R_Y, REV_DEFAULT_HEAD_HAND_OFFSET_R_Z };
		m_TrackedDeviceType = ovrTrackedDevice_RTouch;
	}
}

//bool InputManager::ConnectTouchs()
//{
//	for (OculusTouch* device : m_InputDevices)
//	{
//		//device->IsConnected = true;
//		if (device->ControllerType == ovrControllerType_LTouch)
//		{
//			device->TrackedDeviceType = ovrTrackedDevice_LTouch;
//		}
//		else
//		{
//			device->TrackedDeviceType = ovrTrackedDevice_RTouch;
//		}
//	}
//	return true;
//}

//bool InputManager::DisconnectTouchs()
//{
//	for (OculusTouch* device : m_InputDevices)
//	{
//		//device->IsConnected = false;
//		device->TrackedDeviceType = ovrTrackedDevice_None;
//	}
//	return true;
//}

unsigned int InputManager::OculusTouch::GetStatusFlag()
{
	unsigned int result = 0;

	/*if (TrackedDeviceType != ovrTrackedDevice_None)
	{*/
	result |= ovrStatus_OrientationTracked;
	result |= ovrStatus_PositionTracked;
	//}

	return result;
}

ovrPoseStatef InputManager::OculusTouch::GetPose(double absTime)
{
	/*ovrPoseStatef result = { OVR::Posef::Identity() };
	if (TrackedDeviceType != ovrTrackedDevice_None)
		return result;*/

	m_LastPose.TimeInSeconds = absTime;

	m_LastPose.ThePose.Position.x = m_LastPose.ThePose.Position.x + m_EmulatedOffset.ThePose.Position.x + m_HeadHandOffset.x;
	m_LastPose.ThePose.Position.y = m_LastPose.ThePose.Position.y + m_EmulatedOffset.ThePose.Position.y + m_HeadHandOffset.y;
	m_LastPose.ThePose.Position.z = m_LastPose.ThePose.Position.z + m_EmulatedOffset.ThePose.Position.z + m_HeadHandOffset.z;
	
	//result = m_LastPose;

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

/* Controller child-classes */

//ovrTouch InputManager::OculusTouch::AxisToTouch(vr::VRControllerAxis_t axis)
//{
//	if (m_Role == vr::TrackedControllerRole_LeftHand)
//	{
//		if (axis.y < axis.x) {
//			if (axis.y < -axis.x)
//				return ovrTouch_X;
//			else
//				return ovrTouch_Y;
//		}
//		else {
//			return ovrTouch_LThumb;
//		}
//	}
//	else
//	{
//		if (axis.y < -axis.x) {
//			if (axis.y < axis.x)
//				return ovrTouch_A;
//			else
//				return ovrTouch_B;
//		}
//		else {
//			return ovrTouch_RThumb;
//		}
//	}
//}

//bool InputManager::OculusTouch::IsPressed(vr::VRControllerState_t newState, vr::EVRButtonId button)
//{
//	return newState.ulButtonPressed & vr::ButtonMaskFromId(button) &&
//		!(m_LastState.ulButtonPressed & vr::ButtonMaskFromId(button));
//}
//
//bool InputManager::OculusTouch::IsReleased(vr::VRControllerState_t newState, vr::EVRButtonId button)
//{
//	return !(newState.ulButtonPressed & vr::ButtonMaskFromId(button)) &&
//		m_LastState.ulButtonPressed & vr::ButtonMaskFromId(button);
//}

bool InputManager::OculusTouch::GetInputState(ovrSession session, ovrInputState* inputState)
{
	// Get controller index

	//vr::VRControllerState_t state;
	//vr::VRSystem()->GetControllerState(touch, &state, sizeof(state));

	//unsigned int buttons = 0, touches = 0;

	//uint64_t buttonSupport = vr::VRSystem()->GetUint64TrackedDeviceProperty(touch, vr::Prop_SupportedButtons_Uint64);
	//const bool allButtonsSupported = (buttonSupport & vr::ButtonMaskFromId(vr::k_EButton_A) && buttonSupport & vr::ButtonMaskFromId(k_EButton_B));

	//if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu))
	//	buttons |= (hand == ovrHand_Left) ? ovrButton_Enter : ovrButton_Home;

	//if (state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_A))
	//	buttons |= (hand == ovrHand_Left) ? ovrButton_X : ovrButton_A;

	//if (state.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_A))
	//	touches |= (hand == ovrHand_Left) ? ovrTouch_X : ovrTouch_A;

	//if (state.ulButtonPressed & vr::ButtonMaskFromId(k_EButton_B))
	//	buttons |= (hand == ovrHand_Left) ? ovrButton_Y : ovrButton_B;

	//if (state.ulButtonTouched & vr::ButtonMaskFromId(k_EButton_B))
	//	touches |= (hand == ovrHand_Left) ? ovrTouch_Y : ovrTouch_B;

	//// Allow users to enable a toggled grip.
	//if (session->ToggleGrip == revGrip_Hybrid)
	//{
	//	if (IsPressed(state, vr::k_EButton_Grip))
	//	{
	//		// Only set the timestamp on the first grip toggle, we don't want to toggle twice
	//		if (!m_Gripped)
	//			m_GrippedTime = ovr_GetTimeInSeconds();

	//		m_Gripped = true;
	//	}

	//	if (IsReleased(state, vr::k_EButton_Grip))
	//	{
	//		if (ovr_GetTimeInSeconds() - m_GrippedTime > session->ToggleDelay)
	//			m_Gripped = false;

	//		// Next time we always want to release grip
	//		m_GrippedTime = 0.0;
	//	}
	//}
	//else if (session->ToggleGrip == revGrip_Toggle)
	//{
	//	if (IsPressed(state, vr::k_EButton_Grip))
	//		m_Gripped = !m_Gripped;
	//}
	//else
	//{
	//	m_Gripped = !!(state.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip));
	//}


	//// When we release the grip we need to keep it just a little bit pressed, because games like Toybox
	//// can't handle a sudden jump to absolute zero.
	//if (m_Gripped)
	//	inputState->HandTrigger[hand] = 1.0f;
	//else
	//	inputState->HandTrigger[hand] = 0.1f;

	//// Convert the axes
	//for (int j = 0; j < vr::k_unControllerStateAxisCount; j++)
	//{
	//	vr::ETrackedPropertyError err;
	//	vr::EVRButtonId button = (vr::EVRButtonId)(vr::k_EButton_Axis0 + j);
	//	vr::ETrackedDeviceProperty prop = (vr::ETrackedDeviceProperty)(vr::Prop_Axis0Type_Int32 + j);
	//	vr::EVRControllerAxisType type = (vr::EVRControllerAxisType)vr::VRSystem()->GetInt32TrackedDeviceProperty(touch, prop, &err);
	//	vr::VRControllerAxis_t axis = state.rAxis[j];

	//	if (err != vr::TrackedProp_Success)
	//		break;

	//	if (type == vr::k_eControllerAxis_Joystick)
	//	{
	//		//determine how far the controller is pushed
	//		float magnitude = sqrt(axis.x*axis.x + axis.y*axis.y);
	//		inputState->ThumbstickNoDeadzone[hand].x = axis.x;
	//		inputState->ThumbstickNoDeadzone[hand].y = axis.y;

	//		//check if the controller is outside a circular dead zone
	//		if (magnitude > session->Deadzone)
	//		{
	//			//clip the magnitude at its expected maximum value
	//			if (magnitude > 1.0f) magnitude = 1.0f;

	//			//adjust magnitude relative to the end of the dead zone
	//			magnitude -= session->Deadzone;

	//			//optionally normalize the magnitude with respect to its expected range
	//			//giving a magnitude value of 0.0 to 1.0
	//			float normalizedMagnitude = magnitude / (1.0f - session->Deadzone);
	//			inputState->Thumbstick[hand].x = normalizedMagnitude * axis.x;
	//			inputState->Thumbstick[hand].y = normalizedMagnitude * axis.y;
	//		}

	//		if (state.ulButtonTouched & vr::ButtonMaskFromId(button))
	//			touches |= (hand == ovrHand_Left) ? ovrTouch_LThumb : ovrTouch_RThumb;

	//		if (state.ulButtonPressed & vr::ButtonMaskFromId(button))
	//			buttons |= (hand == ovrHand_Left) ? ovrButton_LThumb : ovrButton_RThumb;
	//	}
	//	else if (type == vr::k_eControllerAxis_TrackPad)
	//	{
	//		ovrTouch quadrant = AxisToTouch(axis);
	//		vr::VRControllerAxis_t lastAxis = m_LastState.rAxis[j];

	//		if (state.ulButtonTouched & vr::ButtonMaskFromId(button))
	//		{
	//			if (m_LastState.ulButtonTouched & vr::ButtonMaskFromId(button))
	//			{
	//				OVR::Vector2f delta(lastAxis.x - axis.x, lastAxis.y - axis.y);
	//				m_ThumbStick -= delta * session->Sensitivity;

	//				// Determine how far the controller is pushed
	//				float magnitude = sqrt(m_ThumbStick.x*m_ThumbStick.x + m_ThumbStick.y*m_ThumbStick.y);
	//				if (magnitude > 0.0f)
	//				{
	//					// Determine the direction the controller is pushed
	//					OVR::Vector2f normalized = m_ThumbStick / magnitude;

	//					// Clip the magnitude at its expected maximum value and recenter
	//					if (magnitude > 1.0f) magnitude = 1.0f;
	//					m_ThumbStick = normalized * magnitude;

	//					if (magnitude > session->Deadzone)
	//					{
	//						// Adjust magnitude relative to the end of the dead zone
	//						magnitude -= session->Deadzone;

	//						// Optionally normalize the magnitude with respect to its expected range
	//						// giving a magnitude value of 0.0 to 1.0
	//						float normalizedMagnitude = magnitude / (1.0f - session->Deadzone);
	//						inputState->Thumbstick[hand].x = m_ThumbStick.x * normalizedMagnitude;
	//						inputState->Thumbstick[hand].y = m_ThumbStick.y * normalizedMagnitude;

	//						// Since we don't have a physical thumbstick we always want a deadzone
	//						// before we activate the stick, but we don't normalize it here
	//						inputState->ThumbstickNoDeadzone[hand].x = m_ThumbStick.x;
	//						inputState->ThumbstickNoDeadzone[hand].y = m_ThumbStick.y;
	//					}
	//				}
	//			}

	//			if (allButtonsSupported)
	//				touches |= (hand == ovrHand_Left) ? ovrTouch_LThumb : ovrTouch_RThumb;
	//			else
	//				touches |= quadrant;
	//		}
	//		else
	//		{
	//			// Touchpad was released, reset the thumbstick
	//			m_ThumbStick.x = m_ThumbStick.y = 0.0f;

	//			touches |= (hand == ovrHand_Left) ? ovrTouch_LThumbUp : ovrTouch_RThumbUp;
	//		}

	//		if (state.ulButtonPressed & vr::ButtonMaskFromId(button))
	//		{
	//			if (allButtonsSupported)
	//				buttons |= (hand == ovrHand_Left) ? ovrButton_LThumb : ovrButton_RThumb;
	//			else
	//				buttons |= quadrant;
	//		}
	//	}
	//	else if (type == vr::k_eControllerAxis_Trigger)
	//	{
	//		if (state.ulButtonTouched & vr::ButtonMaskFromId(button))
	//			touches |= (hand == ovrHand_Left) ? ovrTouch_LIndexTrigger : ovrTouch_RIndexTrigger;
	//		else if (m_Gripped)
	//			touches |= (hand == ovrHand_Left) ? ovrTouch_LIndexPointing : ovrTouch_RIndexPointing;

	//		inputState->IndexTrigger[hand] = axis.x;
	//	}
	//}

	//// We don't apply deadzones yet on triggers and grips
	//inputState->IndexTriggerNoDeadzone[hand] = inputState->IndexTrigger[hand];
	//inputState->HandTriggerNoDeadzone[hand] = inputState->HandTrigger[hand];

	//// We have no way to get raw values
	//inputState->ThumbstickRaw[hand] = inputState->ThumbstickNoDeadzone[hand];
	//inputState->IndexTriggerRaw[hand] = inputState->IndexTriggerNoDeadzone[hand];
	//inputState->HandTriggerRaw[hand] = inputState->HandTriggerNoDeadzone[hand];

	//// Commit buttons/touches
	//inputState->Buttons |= buttons;
	//inputState->Touches |= touches;

	//// Save the state
	//m_LastState = state;
	return true;
}

//InputManager::XboxGamepad::XboxGamepad()
//{
//	m_XInput = LoadLibrary(L"xinput1_3.dll");
//	if (m_XInput)
//	{
//		GetState = (_XInputGetState)GetProcAddress(m_XInput, "XInputGetState");
//		SetState = (_XInputSetState)GetProcAddress(m_XInput, "XInputSetState");
//	}
//}

//InputManager::XboxGamepad::~XboxGamepad()
//{
//	FreeLibrary(m_XInput);
//}

//bool InputManager::XboxGamepad::IsConnected()
//{
//	if (!m_XInput)
//		return false;
//
//	// Check for Xbox controller
//	XINPUT_STATE input;
//	return GetState(0, &input) == ERROR_SUCCESS;
//}

//bool InputManager::XboxGamepad::GetInputState(ovrSession session, ovrInputState* inputState)
//{
//	if (!m_XInput)
//		return false;
//
//	// Use XInput for Xbox controllers.
//	XINPUT_STATE state;
//	if (GetState(0, &state) == ERROR_SUCCESS)
//	{
//		// Convert the buttons
//		bool active = false;
//		WORD buttons = state.Gamepad.wButtons;
//		if (buttons & XINPUT_GAMEPAD_DPAD_UP)
//			inputState->Buttons |= ovrButton_Up;
//		if (buttons & XINPUT_GAMEPAD_DPAD_DOWN)
//			inputState->Buttons |= ovrButton_Down;
//		if (buttons & XINPUT_GAMEPAD_DPAD_LEFT)
//			inputState->Buttons |= ovrButton_Left;
//		if (buttons & XINPUT_GAMEPAD_DPAD_RIGHT)
//			inputState->Buttons |= ovrButton_Right;
//		if (buttons & XINPUT_GAMEPAD_START)
//			inputState->Buttons |= ovrButton_Enter;
//		if (buttons & XINPUT_GAMEPAD_BACK)
//			inputState->Buttons |= ovrButton_Back;
//		if (buttons & XINPUT_GAMEPAD_LEFT_THUMB)
//			inputState->Buttons |= ovrButton_LThumb;
//		if (buttons & XINPUT_GAMEPAD_RIGHT_THUMB)
//			inputState->Buttons |= ovrButton_RThumb;
//		if (buttons & XINPUT_GAMEPAD_LEFT_SHOULDER)
//			inputState->Buttons |= ovrButton_LShoulder;
//		if (buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
//			inputState->Buttons |= ovrButton_RShoulder;
//		if (buttons & XINPUT_GAMEPAD_A)
//			inputState->Buttons |= ovrButton_A;
//		if (buttons & XINPUT_GAMEPAD_B)
//			inputState->Buttons |= ovrButton_B;
//		if (buttons & XINPUT_GAMEPAD_X)
//			inputState->Buttons |= ovrButton_X;
//		if (buttons & XINPUT_GAMEPAD_Y)
//			inputState->Buttons |= ovrButton_Y;
//
//		active = (buttons != 0);
//
//		// Convert the axes
//		SHORT deadzones[] = { XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE };
//		for (int i = 0; i < ovrHand_Count; i++)
//		{
//			float X, Y, trigger;
//			if (i == ovrHand_Left)
//			{
//				X = state.Gamepad.sThumbLX;
//				Y = state.Gamepad.sThumbLY;
//				trigger = state.Gamepad.bLeftTrigger;
//			}
//			if (i == ovrHand_Right)
//			{
//				X = state.Gamepad.sThumbRX;
//				Y = state.Gamepad.sThumbRY;
//				trigger = state.Gamepad.bRightTrigger;
//			}
//
//			//determine how far the controller is pushed
//			float magnitude = sqrt(X*X + Y*Y);
//
//			//determine the direction the controller is pushed
//			float normalizedX = X / magnitude;
//			float normalizedY = Y / magnitude;
//			inputState->ThumbstickNoDeadzone[i].x = normalizedX;
//			inputState->ThumbstickNoDeadzone[i].y = normalizedY;
//
//			//check if the controller is outside a circular dead zone
//			if (magnitude > deadzones[i])
//			{
//				//clip the magnitude at its expected maximum value
//				if (magnitude > 32767) magnitude = 32767;
//
//				//adjust magnitude relative to the end of the dead zone
//				magnitude -= deadzones[i];
//
//				//optionally normalize the magnitude with respect to its expected range
//				//giving a magnitude value of 0.0 to 1.0
//				float normalizedMagnitude = magnitude / (32767 - deadzones[i]);
//				inputState->Thumbstick[i].x = normalizedMagnitude * normalizedX;
//				inputState->Thumbstick[i].y = normalizedMagnitude * normalizedY;
//				active = true;
//			}
//
//			if (trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
//			{
//				//clip the magnitude at its expected maximum value
//				if (trigger > 255) trigger = 255;
//				inputState->IndexTriggerNoDeadzone[i] = trigger / 255.0f;
//
//				//adjust magnitude relative to the end of the dead zone
//				trigger -= XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
//
//				//optionally normalize the magnitude with respect to its expected range
//				//giving a magnitude value of 0.0 to 1.0
//				float normalizedTrigger = trigger / (255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
//				inputState->IndexTrigger[i] = normalizedTrigger;
//				active = true;
//			}
//		}
//
//		return active;
//	}
//	return false;
//}
