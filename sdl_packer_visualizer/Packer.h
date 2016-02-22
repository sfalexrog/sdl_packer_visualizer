#pragma once
#include <vector>
#include "PackRect.h"
#include <SDL.h>

struct Node;

class Packer
{
private:
	std::vector<Node*> pages;
	int _w, _h;
public:
	int getNumPages() { return pages.size(); }
	int putImg(PackRect& img);
	int getPageWidth() { return _w; }
	int getPageHeight() { return _h; }
	Packer(int w, int h) : _w(w), _h(h) {}
	~Packer();
};

