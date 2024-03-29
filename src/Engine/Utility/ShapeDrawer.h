#pragma once

#include "Core/Common.h"

namespace ShapeDrawer
{
	void Init();
	void OnBeginRenderToScreen();
	void OnEndRenderToScreen();
	void Rect(Vec2 position, Vec2 scale, Vec4 color = Vec4(1));
	void Circle(Vec2 position, Vec2 scale, Vec4 color = Vec4(1));
	void Line(Vec2 start, Vec2 end, float width = 1.0f, Vec4 color = Vec4(1));
	void Triangle(Vec2 position, Vec2 direction, Vec2 scale, Vec4 color = Vec4(1));

	void Text(std::string str, Vec2 position, float scale = 1.0f, Vec4 color = Vec4(1));
	void Char(char c, Vec2 position, Vec2 scale = Vec2(1.0f, 1.0f), Vec4 color = Vec4(1));

	bool IsLimit(int count);
};

void Text(std::string str, Vec2 position, float scale = 1.0f, Vec4 color = Vec4(1));
