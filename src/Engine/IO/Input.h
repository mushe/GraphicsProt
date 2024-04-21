#pragma once

#include "Core/Common.h"
#include "Utility/Singleton.h"
#include "Utility/Debug.h"
#include <unordered_map>

enum KeyCode
{
    Space = GLFW_KEY_SPACE,
    A = GLFW_KEY_A,
    B = GLFW_KEY_B,
    C = GLFW_KEY_C,
    D = GLFW_KEY_D,
    E = GLFW_KEY_E,
    F = GLFW_KEY_F,
    G = GLFW_KEY_G,
    H = GLFW_KEY_H,
    I = GLFW_KEY_I,
    J = GLFW_KEY_J,
    K = GLFW_KEY_K,
    L = GLFW_KEY_L,
    M = GLFW_KEY_M,
    N = GLFW_KEY_N,
    O = GLFW_KEY_O,
    P = GLFW_KEY_P,
    Q = GLFW_KEY_Q,
    R = GLFW_KEY_R,
    S = GLFW_KEY_S,
    T = GLFW_KEY_T,
    U = GLFW_KEY_U,
    V = GLFW_KEY_V,
    W = GLFW_KEY_W,
    X = GLFW_KEY_X,
    Y = GLFW_KEY_Y,
    Z = GLFW_KEY_Z,
    Escape = GLFW_KEY_ESCAPE,
    RightShift = GLFW_KEY_RIGHT_SHIFT,
    LeftShift = GLFW_KEY_LEFT_SHIFT,
    Up = GLFW_KEY_UP,
    Down = GLFW_KEY_DOWN,
    Left = GLFW_KEY_LEFT,
    Right = GLFW_KEY_RIGHT,
};


enum MouseCode
{
    LEFT,
    RIGHT,
    MIDDLE
};


class Input : public Singleton<Input>
{
	friend class Singleton<Input>;

private:
    GLFWwindow* window_;
    float wheelDelta_ = 0.0f;
    Vec2 mousePosition_ = Vec2(0.0f, 0.0f);
    Vec2 mousePositionDelta_ = Vec2(0.0f, 0.0f);
    bool rightMouseDown_ = false;
    bool leftMouseDown_ = false;

    std::unordered_map<KeyCode, int> keyDownFrames_;

public:
    void Init(GLFWwindow* window);

    void Update();

    static bool KeyDown(KeyCode keyCode);
    bool KeyDown_Call(KeyCode keyCode);

    static bool KeyDownNow(KeyCode keyCode);
    bool KeyDownNow_Call(KeyCode keyCode);

    static bool KeyUp(KeyCode keyCode) { return false; }

    static float MouseWheelDelta();
    float MouseWheelDelta_Call();
    static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
		Input::GetInstance()->wheelDelta_ = (float)yoffset;
    }

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
            GetInstance()->rightMouseDown_ = true;
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
            GetInstance()->rightMouseDown_ = false;
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            GetInstance()->leftMouseDown_ = true;
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            GetInstance()->leftMouseDown_ = false;
    }

    static Vec2 RightMouseDrag();
    static Vec2 LeftMouseDrag();

    static void MouseCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        Input::GetInstance()->mousePositionDelta_ = Vec2((float)xpos, (float)ypos) - Input::GetInstance()->mousePosition_;
        Input::GetInstance()->mousePosition_ = Vec2((float)xpos, (float)ypos);
    }
    
    static bool MouseClickedNow(MouseCode mouseCode = MouseCode::LEFT) { return false; }
    static bool MouseDown(MouseCode mouseCode = MouseCode::LEFT) { return false; }
    static bool MouseUp(MouseCode mouseCode = MouseCode::LEFT) { return false; }

protected:
	Input() :
        window_(nullptr)
    {}

    ~Input() {}
};

