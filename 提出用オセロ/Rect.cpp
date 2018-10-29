#include "Rect.h"

//‰¡•‚Ìæ“¾
float Rect::GetWidth() const
{
	return right-left;
}

//‚‚³‚Ìæ“¾
float Rect::GetHeight() const
{
	return bottom-top;
}
