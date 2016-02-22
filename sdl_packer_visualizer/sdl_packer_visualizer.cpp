// sdl_packer_visualizer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SDL.h>
#include <iostream>
#include "PackRect.h"
#include <vector>
#include <stdlib.h>
#include "Packer.h"
#include <algorithm>


const int NUM_RECTS = 1000;

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SDL_Init(SDL_INIT_EVERYTHING);
	const int winW = 1024;
	const int winH = 1024;
	SDL_Window *window = SDL_CreateWindow("Packing Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winW, winH, SDL_WINDOW_OPENGL);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	std::cout << "Generating a bunch of random rectangles..." << std::endl;

	PackRect::boundRenderer = renderer;

	std::vector<PackRect> rects;
	for (int i = 0; i < NUM_RECTS; i++)
	{
		PackRect nextRect;
		nextRect.setSize(2 + rand() % 510, 2 + rand() % 510);
		nextRect.placeAt(rand() % 300, rand() % 300);
		SDL_Color c{128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128, 255};
		nextRect.setColor(c);
		rects.push_back(nextRect);
	}

	std::cout << "Sorting these random rectangles by size..." << std::endl;

	//std::sort(rects.rbegin(), rects.rend());

	std::cout << "Attempting to push them into the tree..." << std::endl;
	Packer packer(winW, winH);

	long long totalPixels = 0;

	for (auto& rect : rects)
	{
		packer.putImg(rect);
		//std::cout << "Rectangle { " << rect.getRect()->w << ", " << rect.getRect()->h << " } placed on page " << rect.pageNum << "\n";
		totalPixels += rect.getRect()->w * rect.getRect()->h;
	}
	int numPagesOptimal = totalPixels / (winH * winW) + 1;

	std::cout << "Finished packing; images took " << packer.getNumPages() << " pages to allocate" << std::endl;
	std::cout << "Packed " << totalPixels << " pixels (estimated optimal packing:  " << numPagesOptimal << std::endl;

	int currentPage = 0;
	std::cout << "Displaying page " << currentPage << std::endl;

	bool quit = false;

	while (!quit)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_KEYDOWN:
				//std::cout << "Key pressed: " << ev.key.keysym.sym << std::endl;
				switch (ev.key.keysym.sym)
				{
				case SDLK_LEFT:
					if (currentPage > 0) { currentPage--; }
					break;
				case SDLK_RIGHT:
					if (currentPage < (packer.getNumPages() - 1)) { currentPage++; }
					break;
				}
				std::cout << "Displaying page " << currentPage << std::endl;
				break;
			case SDL_WINDOWEVENT:
				switch (ev.window.type)
				{
				case SDL_WINDOWEVENT_CLOSE:
					quit = true;
					break;
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			}

		}
		SDL_Delay(1);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		for (auto& rect : rects)
		{
			if (currentPage == rect.pageNum)
			{
				rect.drawRect();
			}
		}
		SDL_RenderPresent(renderer);
	}
	SDL_Quit();
	return 0;
}

