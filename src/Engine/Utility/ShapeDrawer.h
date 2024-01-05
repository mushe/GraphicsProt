#pragma once

#include <glm/glm.hpp>

namespace ShapeDrawer
{
	void Init();
	void OnBeginRenderToScreen();
	void OnEndRenderToScreen();
	void Rect(glm::vec2 position, glm::vec2 scale, glm::vec4 color = glm::vec4(1));
	void Circle(glm::vec2 position, glm::vec2 scale, glm::vec4 color = glm::vec4(1));
	void Line(glm::vec2 start, glm::vec2 end, float width = 1.0f, glm::vec4 color = glm::vec4(1));
	void Triangle(glm::vec2 position, glm::vec2 direction, glm::vec2 scale, glm::vec4 color = glm::vec4(1));

	bool IsLimit(int count);
};
