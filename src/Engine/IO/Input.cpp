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
}

bool Input::KeyDown(KeyCode keyCode)
{
	return GetInstance()->KeyDown_Call(keyCode);
}
bool Input::KeyDown_Call(KeyCode keyCode)
{
	return glfwGetKey(window_, keyCode) == GLFW_PRESS;
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
