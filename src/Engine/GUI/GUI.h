#pragma once
#include <string>

#include "Utility/Singleton.h"
#include "GUI/ImGuiWrapper.h"

#include "ThirdParty/imgui/imgui_impl_vulkan.h"
#include "ThirdParty/imgui/imgui_impl_glfw.h"
#include "ThirdParty/imgui/imgui_impl_vulkan.h"

#include <glm/glm.hpp>

using namespace std;

#include <variant>
#include <map>

class GUI : public Singleton<GUI>
{
    friend class Singleton<GUI>;
    ImGuiWrapper* imGuiWrapper_;

public:
    void Init(ImGuiWrapper* imGuiWrapper) { imGuiWrapper_ = imGuiWrapper; }

    std::map<std::string, std::variant<float, glm::vec2, glm::vec3, glm::vec4>> parameters_;

    static void Draw() {}
    static void ToggleDisplay(){}
    static void Show(){}
    static void Hide(){}

    static void Float(float& value, string label = "", float speed = 1.0f);
    static void FloatSlider(float& value, string label = "", float min = 0.0f, float max = 1.0f);
    static void Vector(glm::vec2& v, const string label = "", float speed = 1.0f);
    static void Vector(glm::vec3& v, const string label = "", float speed = 1.0f);
    static void VectorSlider(glm::vec3& v, const string label = "", float min = 0.0f, float max = 1.0f);
    static void Label(string text);

    static void Color(const glm::vec3& color, const string label = "");
    static void Color(const glm::vec4& color, const string label = "");

    static void ChangeStyleToLight();

    static void Begin();
    void CallBegin();

    static void End();
    void CallEnd();

    static void ShowFPS();
    void CallShowFPS();

    static void Space();
    void CallSpace();

    static void PrintParameters();
    void CallPrintParameters();

protected:
    GUI():imGuiWrapper_(nullptr){}
	~GUI() {}
};
