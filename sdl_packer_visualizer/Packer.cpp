#include "stdafx.h"
#include "Packer.h"
#include <SDL.h>
#include <vector>
#include <memory>

struct Rectangle
{
	int left, top, right, bottom;
	Rectangle() : top(0), bottom(0), left(0), right(0) {}
	Rectangle(int l, int t, int r, int b) : top(t), left(l), right(r), bottom(b) {}
};

struct Node {
	std::unique_ptr<Node> child[2];
	Rectangle rc;
	PackRect *prect;

	enum fitting {
		DOES_NOT_FIT,
		FITS_PERFECTLY,
		FITS_PERFECTLY_ROTATED,
		FITS_WITH_SPACE,
		FITS_WITH_SPACE_ROTATED
	};

	fitting fits(PackRect& img)
	{
		int rcWidth = rc.right - rc.left;
		int rcHeight = rc.bottom - rc.top;
		int imWidth = img.getRect()->w;
		int imHeight = img.getRect()->h;
		if ((rcWidth == imWidth) && (rcHeight == imHeight))
		{
			return FITS_PERFECTLY;
		}
		if ((rcWidth == imHeight) && (rcHeight == imWidth))
		{
			img.rotate();
			return FITS_PERFECTLY_ROTATED;
		}
		if ((rcWidth >= imWidth) && (rcHeight >= imHeight))
		{
			return FITS_WITH_SPACE;
		}
		if ((rcWidth >= imHeight) && (rcHeight >= imWidth))
		{
			img.rotate();
			return FITS_WITH_SPACE_ROTATED;
		}
		return DOES_NOT_FIT;
	}

	Node(Rectangle freeSpace) :rc(freeSpace), prect(0) {}

	Node* insert(PackRect& img)
	{
		if (child[0] || child[1])
		{
			auto newNode = child[1]->insert(img);
			if (newNode != NULL) { return newNode; }
			return child[0]->insert(img);
		}
		else
		{
			if (prect) return NULL;
			auto fit = fits(img);
			if (fit == DOES_NOT_FIT)
			{
				return 0;
			}
			if ((fit == FITS_PERFECTLY) || (fit == FITS_PERFECTLY_ROTATED))
			{
				prect = &img;
				img.placeAt(rc.left, rc.top);
				return this;
			}
			int dw = (rc.right - rc.left) - img.getRect()->w;
			int dh = (rc.bottom - rc.top) - img.getRect()->h;
			if (dw > dh)
			{
				child[0].reset(new Node(Rectangle(rc.left, rc.top, rc.left + img.getRect()->w, rc.bottom)));
				child[1].reset(new Node(Rectangle(rc.left + img.getRect()->w, rc.top, rc.right, rc.bottom)));
			}
			else
			{
				child[0].reset(new Node(Rectangle(rc.left, rc.top, rc.right, rc.top + img.getRect()->h)));
				child[1].reset(new Node(Rectangle(rc.left, rc.top + img.getRect()->h, rc.right, rc.bottom)));
			}
			return child[0]->insert(img);
		}
	}

	~Node()
	{
	}

};

int Packer::putImg(PackRect &img)
{
	if (pages.size() == 0)
	{
		pages.push_back(new Node(Rectangle{ 0, 0, _w, _h }));
	}
	int pageNum = 0;
	while (!pages[pageNum]->insert(img))
	{
		pageNum++;
		if (pageNum == pages.size())
		{
			pages.push_back(new Node(Rectangle{ 0, 0, _w, _h }));
		}
	}
	img.pageNum = pageNum;
	return pageNum;
}


Packer::~Packer()
{
	for (auto& page : pages)
	{
		delete page;
	}
}
