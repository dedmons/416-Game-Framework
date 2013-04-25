#include <sstream>
#include "menu.h"
#include "jsongamedata.h"
#include "viewport.h"

Menu::~Menu() {
  SDL_FreeSurface(surfaceOff);
  SDL_FreeSurface(surfaceOn);
}

Menu::Menu() :
  io( IOManager::getInstance() ),
  words(),
  position(),
  space(),
  jgdata( JSONGamedata::getInstance() ),
  surfaceOff(io.loadAndSet(
        jgdata.getStr("menu.clickoff.file"),
        jgdata.getBool("menu.clickoff.transparency")
  )),
  surfaceOn(io.loadAndSet(
        jgdata.getStr("menu.clickon.file"),
        jgdata.getBool("menu.clickon.transparency")
  )),
  clicks(),
  nextIcon(0),
  click(0)
{
  unsigned int n = jgdata.getValue("menu.options")->CountChildren();
  position[0] = jgdata.getInt("menu.loc.x");
  position[1] = jgdata.getInt("menu.loc.y");
  std::stringstream strm;
  for ( unsigned i = 0; i < n; ++i ) {
    strm << "menu.options.#" << i;
    words.push_back( jgdata.getStr(strm.str()) );
    strm.clear(); // clear error flags
    strm.str(std::string()); // clear contents
  }

  space[0] = jgdata.getInt("menu.space.x");
  space[1] = jgdata.getInt("menu.space.y");

  Frame clickoff(surfaceOff,
              jgdata.getInt("menu.clickoff.size.w"),
              jgdata.getInt("menu.clickoff.size.h"),
              jgdata.getInt("menu.clickoff.src.x"),
              jgdata.getInt("menu.clickoff.src.y"));
  clicks.push_back( clickoff );

  Frame clickon(surfaceOn,
              jgdata.getInt("menu.clickon.size.w"),
              jgdata.getInt("menu.clickon.size.h"),
              jgdata.getInt("menu.clickon.src.x"),
              jgdata.getInt("menu.clickon.src.y"));
  clicks.push_back( clickon );

}

void Menu::draw() const {
  int x = position[0];
  int y = position[1];
  for (unsigned i = 0; i < words.size(); ++i) {
    io.printMessageAt(words[i], x, y+(space[1]/2.0));
    y += space[1];
  }
  y = position[1] + nextIcon*space[1];
  clicks[click].draw(position[0]-space[0]+Viewport::getInstance().X(), y + Viewport::getInstance().Y());
}

const string& Menu::getIconClicked() const {
  return words[nextIcon];
}

