#pragma once

struct Rect;
struct Point;

//矩形内部に点がある場合trueを返す関数
bool PointInRect(const Rect & rect, const Point & point);
