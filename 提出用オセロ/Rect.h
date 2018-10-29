#pragma once

struct Rect
{
	float left;
	float right;
	float top;
	float bottom;

	float GetWidth()const;
	float GetHeight()const;
};

