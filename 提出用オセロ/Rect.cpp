#include "Rect.h"

//�����̎擾
float Rect::GetWidth() const
{
	return right-left;
}

//�����̎擾
float Rect::GetHeight() const
{
	return bottom-top;
}
