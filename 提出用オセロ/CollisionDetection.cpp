#include "CollisionDetection.h"
#include"Point.h"
#include"Rect.h"


bool PointInRect(const Rect & rect, const Point & point)
{
	if (point.x > rect.left &&
		point.x < rect.right &&
		point.y > rect.top &&
		point.y < rect.bottom)
	{
		return true;
	}
	else
	{
		return false;
	}
}
