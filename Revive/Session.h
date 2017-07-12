#pragma once

#include <OVR_CAPI.h>
#include <memory>

class InputManager;

struct ovrHmdStruct
{
	std::unique_ptr<InputManager> Input;

	ovrHmdStruct();
};
