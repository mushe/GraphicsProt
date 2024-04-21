#include "Input.h"
#include <cassert>

void Input::Init(GLFWwindow* window)
{
	assert(window != nullptr);
	window_ = window;
}

void Input::Update()
{
	wheelDelta_ = 0.0f;
	mousePositionDelta_ = Vec2(0.0f);

	for (const auto& pair : keyDownFrames_) 
	{
		if(glfwGetKey(window_, pair.first) == GLFW_PRESS)
			keyDownFrames_[pair.first] = min(keyDownFrames_[pair.first]+ 1, 2);
		else
			keyDownFrames_[pair.first] = 0;
    }
}

bool Input::KeyDown(KeyCode keyCode)
{
	return GetInstance()->KeyDown_Call(keyCode);
}
bool Input::KeyDown_Call(KeyCode keyCode)
{
	return glfwGetKey(window_, keyCode) == GLFW_PRESS;
}

bool Input::KeyDownNow(KeyCode keyCode)
{
	return GetInstance()->KeyDownNow_Call(keyCode);
}
bool Input::KeyDownNow_Call(KeyCode keyCode)
{
	return keyDownFrames_[keyCode] == 1;
}

float Input::MouseWheelDelta()
{
	return GetInstance()->MouseWheelDelta_Call();
}
float Input::MouseWheelDelta_Call()
{
	return wheelDelta_;
}

Vec2 Input::RightMouseDrag()
{
	return GetInstance()->rightMouseDown_ ? GetInstance()->mousePositionDelta_ : Vec2(0.0f);
}

Vec2 Input::LeftMouseDrag()
{
    return GetInstance()->leftMouseDown_ ? GetInstance()->mousePositionDelta_ : Vec2(0.0f);
}
