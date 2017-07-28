#pragma once

#include "Utils.h"

#define LogVTouch(x, ...) if (m_VTouchLog) fprintf(m_ReviveLog, x, __VA_ARGS__); \
					printf(x, __VA_ARGS__); \
					fflush(m_VTouchLog);

class OculusTouch
{
public:
	OculusTouch(ovrControllerType role);
	virtual ~OculusTouch() {}

	ovrControllerType ControllerType;

	unsigned int GetStatusFlag();
	ovrPoseStatef GetOffset(double absTime);
	void EmulateTouchPositionOffset(float x, float y);
	void EmulateTouchOrientationOffset(unsigned int axis, float degree);

private:
	ovrPoseStatef m_LastOffset;
	ovrVector3f m_HeadHandPositionOffset;
};

class InputManager
{
public:
	~InputManager();
	static InputManager* GetInstance();

	unsigned int GetConnectedControllerTypes();
	ovrResult GetInputState(ovrSession session, ovrControllerType controllerType, ovrInputState* inputState);
	void GetTrackingState(ovrSession session, ovrTrackingState* outState, double absTime);
	ovrResult GetDevicePoses(ovrTrackedDeviceType* deviceTypes, int deviceCount, double absTime, ovrPoseStatef* outDevicePoses);
	void EmulateTouchesPositionOffset(unsigned int controllerType, float x, float y);
	void EmulateTouchesOrientationOffset(unsigned int controllerType, unsigned int axis, float radian);
	void EmulateTouchesInputState(unsigned int touchKey, bool state);
	void EmulateTouchesInputState(unsigned int touchKey, float value);
	void EmulateTouchesInputState(unsigned int touchKey, float x, float y);
	void EmulateResetTouchesPose();

private:
	InputManager();
	static InputManager* m_Instance;
	OculusTouch* m_TouchL;
	OculusTouch* m_TouchR;
	ovrInputState* m_LastState;
	FILE* m_VTouchLog;
};



