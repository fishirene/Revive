#pragma once

#include "OVR_CAPI.h"
#include "Extras/OVR_Math.h"

#include <thread>
#include <vector>
#include <Windows.h>
#include "Settings.h"

typedef enum ovrTouchSuppliment_ {
	ovrTouch_LThumbstick = 0x00000080,
	ovrTouch_RThumbstick = 0x00008000,
	ovrTouch_LHandTrigger = 0x00010000,
	ovrTouch_RHandTrigger = 0x00020000,
} ovrTouchSuppliment;

class InputManager
{
public:
	class OculusTouch
	{
	public:
		OculusTouch(ovrControllerType role);
		virtual ~OculusTouch() {}

		ovrControllerType ControllerType;

		unsigned int GetStatusFlag();
		ovrPoseStatef GetPose(double absTime);
		void EmulateTouchPositionOffset(float x, float y);
		void EmulateTouchOrientationOffset(float x, float y, float z, float w);

	private:
		ovrPoseStatef m_LastPose;
		ovrPoseStatef m_EmulatedOffset;
		ovrVector3f m_HeadHandOffset;	
	};

	InputManager();
	~InputManager();

	//bool ConnectTouchs();
	//bool DisconnectTouchs();
	//bool IsConnected();

	unsigned int GetConnectedControllerTypes();
	ovrResult GetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState* inputState);
	void GetTrackingState(ovrSession session, ovrTrackingState* outState, double absTime);
	ovrResult GetDevicePoses(ovrTrackedDeviceType* deviceTypes, int deviceCount, double absTime, ovrPoseStatef* outDevicePoses);
	void EmulateTouchesPositionOffset(ovrControllerType controllerType, float x, float y);
	void EmulateTouchesOrientationOffset(ovrControllerType controllerType, float x, float y, float z, float w);
	void EmulateTouchesInputState(unsigned int touchKey, bool state, float value, float x, float y);
	void EmulateResetTouchesPose();

protected:
	OculusTouch* m_TouchL;
	OculusTouch* m_TouchR;
private:
	ovrInputState m_LastState;
};

