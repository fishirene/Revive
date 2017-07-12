#include "Session.h"
#include "REV_Math.h"
#include "InputManager.h"
#include "Settings.h"

ovrHmdStruct::ovrHmdStruct()
	: Input(new InputManager())
{
}