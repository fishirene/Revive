#pragma once

#include "OVR_CAPI.h"
#include "Extras/OVR_Math.h"
#include <thread>
#include <vector>
#include <Windows.h>
#include "Utils.h"

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
		ovrPoseStatef GetOffset(double absTime);
		void EmulateTouchPositionOffset(float x, float y);
		void EmulateTouchOrientationOffset(float x, float y, float z, float w);

	private:
		ovrPoseStatef m_LastOffset;
		ovrPoseStatef m_EmulatedOffset;
		ovrVector3f m_HeadHandPositionOffset;	
	};
	
	InputManager();
	~InputManager();

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

