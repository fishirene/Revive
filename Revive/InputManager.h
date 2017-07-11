#pragma once

#include "OVR_CAPI.h"
#include "Extras/OVR_Math.h"

#include <openvr.h>
#include <thread>
#include <vector>
#include <Windows.h>
#include <Xinput.h>
#include "Settings.h"

class InputManager
{
public:
	class OculusTouch
	{
	public:
		OculusTouch(ovrControllerType role);
		virtual ~OculusTouch() { }

		ovrControllerType ControllerType;

		unsigned int GetStatusFlag();
		ovrPoseStatef GetPose(double absTime);
		bool GetInputState(ovrSession session, ovrInputState* inputState);
		void EmulateTouchPositionOffset(float x, float y);
		void EmulateTouchOrientationOffset(float x, float y, float z, float w);
		void EmulateTouchInputState();

	private:
		ovrTrackedDeviceType m_TrackedDeviceType;
		ovrInputState m_LastState;
		ovrPoseStatef m_LastPose;
		ovrPoseStatef m_EmulatedOffset;
		ovrVector3f m_HeadHandOffset;
		
		//bool m_StickTouched;
		//OVR::Vector2f m_ThumbStick;		
		
		//void AddOffsetToOVRPose(ovrPoseStatef& pose);

		//ovrTouch AxisToTouch(vr::VRControllerAxis_t axis);
		/*ovrButton m_TouchButtons;
		bool m_Gripped;
		double m_GrippedTime;*/
		/*bool IsPressed(vr::VRControllerState_t newState, vr::EVRButtonId button);
		bool IsReleased(vr::VRControllerState_t newState, vr::EVRButtonId button);*/
	};

	//class XboxGamepad : public InputDevice
	//{
	//public:
	//	XboxGamepad();
	//	virtual ~XboxGamepad();

	//	virtual ovrControllerType GetType() { return ovrControllerType_XBox; }
	//	virtual bool IsConnected();
	//	virtual bool GetInputState(ovrSession session, ovrInputState* inputState);
	//	virtual void SetVibration(float frequency, float amplitude);

	//private:
	//	typedef DWORD(__stdcall* _XInputSetState)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);
	//	typedef DWORD(__stdcall* _XInputGetState)(DWORD dwUserIndex, XINPUT_STATE* pState);

	//	HMODULE m_XInput;
	//	_XInputSetState SetState;
	//	_XInputGetState GetState;
	//};

	InputManager();
	~InputManager();

	//bool ConnectTouchs();
	//bool DisconnectTouchs();
	//bool IsConnected();

	unsigned int GetConnectedControllerTypes();
	ovrResult GetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState* inputState);
	void GetTrackingState(ovrSession session, ovrTrackingState* outState, double absTime);
	ovrResult GetDevicePoses(ovrTrackedDeviceType* deviceTypes, int deviceCount, double absTime, ovrPoseStatef* outDevicePoses);
	void EmulateTouchsPositionOffset(ovrControllerType controllerType, float x, float y);
	void EmulateTouchsOrientationOffset(ovrControllerType controllerType, float x, float y, float z, float w);

protected:
	//std::vector<OculusTouch*> m_InputDevices;
	OculusTouch* m_TouchL;
	OculusTouch* m_TouchR;
};

