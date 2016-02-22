#pragma once
#include <SDL.h>

class PackRect
{
public:
	static SDL_Renderer* boundRenderer;
private:
	SDL_Rect _rect;
	SDL_Color _color;
	
public:
	SDL_Rect *getRect() { return &_rect; }
	void drawRect();
	void setSizeW(int width);
	void setSizeH(int height);
	void placeAtX(int x);
	void placeAtY(int y);
	void setSize(int width, int height);
	void placeAt(int x, int y);
	void setColor(SDL_Color color);
	void rotate();
	int pageNum;
	bool operator<(const PackRect& other) const
	{
		return _rect.w * _rect.h < other._rect.w * other._rect.h;
	}
	PackRect();
	~PackRect();
};

