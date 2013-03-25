#ifndef HUD__H
#define HUD__H

#include <SDL/SDL.h>
#include "ioManager.h"

class HUD {
public:
	static HUD& getHUD() {
		static HUD instance;
		return instance;
	}

	void setScreen(SDL_Surface *newScreen) { screen = newScreen; }

	void draw(const int, const int);

private:
	SDL_Surface * screen;

	HUD() : screen(IOManager::getInstance().getScreen()) {}
	HUD(const HUD&);
	HUD& operator=(const HUD&);

};

#endif
