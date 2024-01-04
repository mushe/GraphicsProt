#pragma once

#include <glm/glm.hpp>

namespace ShapeDrawer
{
	void Init();
	void OnBeginRenderToScreen();
	void OnEndRenderToScreen();
	void Rect(glm::vec2 position, glm::vec2 scale);

	bool IsLimit(int count);
};
