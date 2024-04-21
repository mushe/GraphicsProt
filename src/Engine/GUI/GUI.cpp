#include "GUI.h"
#include "Utility/Debug.h"
#include <vector>

void GUI::Float(float& value, string label, float speed)
{
	ImGui::DragFloat(label.c_str(), (float*)&value, speed);
	GUI::GetInstance()->parameters_[label] = value;
}

void GUI::FloatSlider(float& value, string label, float min, float max)
{
	ImGui::SliderFloat(label.c_str(), (float*)&value, min, max);
	GUI::GetInstance()->parameters_[label] = value;
}

void GUI::Vector(Vec2& v, const string label, float speed)
{
	ImGui::DragFloat2(label.c_str(), (float*)&v, speed);
	GUI::GetInstance()->parameters_[label] = v;
}

void GUI::Vector(Vec3& v, const string label, float speed)
{
	ImGui::DragFloat3(label.c_str(), (float*)&v, speed);
	GUI::GetInstance()->parameters_[label] = v;
}

void GUI::VectorSlider(Vec3& v, const string label, float min, float max)
{
	ImGui::SliderFloat3(label.c_str(), (float*)&v, min, max);
	GUI::GetInstance()->parameters_[label] = v;
}

void GUI::Label(string text)
{
	ImGui::Text(text.c_str());
}

void GUI::Color(const Vec3& color, const string label)
{
	ImGui::ColorEdit4(label.c_str(), (float*)&color);
	GUI::GetInstance()->parameters_[label] = color;
}

void GUI::Color(const Vec4& color, const string label)
{
	ImGui::ColorEdit4(label.c_str(), (float*)&color);
	GUI::GetInstance()->parameters_[label] = color;
}

void GUI::ChangeStyleToLight()
{
	GUI::GetInstance()->imGuiWrapper_->ChangeStyleToLight();
}

void GUI::ChangeStyleToDefault()
{
	GUI::GetInstance()->imGuiWrapper_->ChangeStyleToDefault();
}

void GUI::Begin() { GUI::GetInstance()->CallBegin(); }
void GUI::CallBegin()
{
	Debug::Log("GUI::Begin");
}

void GUI::End() { GUI::GetInstance()->CallEnd(); }
void GUI::CallEnd()
{

}

void GUI::ShowFPS() { GUI::GetInstance()->CallShowFPS(); }
void GUI::CallShowFPS()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::Text("%.1f FPS", io.Framerate);
}

void GUI::Space(){ GUI::GetInstance()->CallSpace(); }
void GUI::CallSpace()
{
	ImGui::Spacing();
}

void GUI::PrintParameters()
{
	GUI::GetInstance()->CallPrintParameters();
}
void GUI::CallPrintParameters()
{
	Debug::Log("------------------------------------------------------------------------------");
	for (auto& p : GUI::GetInstance()->parameters_)
	{
		std::visit([p](auto&& arg)
		{
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, float>)
			{
				Debug::Log("float " + p.first + " = " + std::to_string(arg) + ";");
			}
			else if constexpr (std::is_same_v<T, Vec2>)
			{
				Debug::Log("Vec2 " + p.first + " = Vec2(" + std::to_string(arg.x) + "," + std::to_string(arg.y) + ");");
			}
			else if constexpr (std::is_same_v<T, Vec3>)
			{
				Debug::Log("Vec3 " + p.first + " = Vec3(" + std::to_string(arg.x) + "," + std::to_string(arg.y) + "," + std::to_string(arg.z) + ");");
			}
			else if constexpr (std::is_same_v<T, Vec4>)
			{
				Debug::Log("Vec4 " + p.first + " = Vec4(" + std::to_string(arg.x) + "," + std::to_string(arg.y) + "," + std::to_string(arg.z) + "," + std::to_string(arg.w) + ");");
			}
		}, p.second);

	}
	Debug::Log("------------------------------------------------------------------------------");
}


bool GUI::CallButton(const string label)
{
	return ImGui::Button(label.c_str());
}
bool GUI::Button(const string label)
{
	return GUI::GetInstance()->CallButton(label);
}

void GUI::CallSameLine()
{
	ImGui::SameLine();
}
void GUI::SameLine()
{
	GUI::GetInstance()->CallSameLine();
}