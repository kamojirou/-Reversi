#include "Rect.h"

//横幅の取得
float Rect::GetWidth() const
{
	return right-left;
}

//高さの取得
float Rect::GetHeight() const
{
	return bottom-top;
}
