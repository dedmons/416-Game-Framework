#include <SDL.h>
#include "ioManager.h"
#include "menu.h"
#include "clock.h"
#include "manager.h"

class MenuManager {
public:
  MenuManager ();
  void start();

private:
  bool env;
  SDL_Surface *screen;
  const Clock& clock;

  void getFile();
  void printLevels();
  int chooseLevel();

  SDL_Color backColor;
  Menu menu;
  Manager manager;

  void drawBackground() const;
  MenuManager(const MenuManager&);
  MenuManager& operator=(const MenuManager&);
};
