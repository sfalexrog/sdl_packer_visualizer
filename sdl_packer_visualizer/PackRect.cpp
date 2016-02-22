#include "stdafx.h"

#include "PackRect.h"

SDL_Renderer* PackRect::boundRenderer = 0;

void PackRect::drawRect()
{
	SDL_SetRenderDrawColor(boundRenderer, _color.r, _color.g, _color.b, _color.a);
	SDL_RenderFillRect(boundRenderer, &_rect);
}

void PackRect::setSizeW(int width)
{
	_rect.w = width;
}

void PackRect::setSizeH(int height)
{
	_rect.h = height;
}

void PackRect::placeAtX(int x)
{
	_rect.x = x;
}

void PackRect::placeAtY(int y)
{
	_rect.y = y;
}

void PackRect::setSize(int width, int height)
{
	setSizeH(height);
	setSizeW(width);
}

void PackRect::placeAt(int x, int y)
{
	placeAtX(x);
	placeAtY(y);
}

void PackRect::setColor(SDL_Color color)
{
	_color = color;
}

void PackRect::rotate()
{
	int oldH = _rect.h;
	int oldW = _rect.w;
	_rect.h = oldW;
	_rect.w = oldH;
}

PackRect::PackRect() : _rect({ 0, 0, 0, 0 }), _color({ 0, 0, 0, 0 }), pageNum(0)
{
}


PackRect::~PackRect()
{
}
