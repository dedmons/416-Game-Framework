#include "hud.h"
#include "clock.h"
#include "jsongamedata.h"
#include "aaline.h"

void HUD::draw(const int dx, const int dy) {
	IOManager& iom = IOManager::getInstance();

  const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Sint16 w=250,h=115;
  Sint16 x=0+dx,y=0+dy;

  Draw_AALine(screen,   x,   y, x+w,   y, RED);
  Draw_AALine(screen, x+w,   y, x+w, y+h, RED);
  Draw_AALine(screen,   x, y+h, x+w, y+h, RED);
  Draw_AALine(screen,   x,   y,   x, y+h, RED);

  std::stringstream sstm;
  sstm << "FPS: " << Clock::getInstance().getFps();
  iom.printMessageAt(sstm.str(),x+10,y+10);

  iom.printMessageAt("Press t and shift+t to cycle",x+10,y+25);
  iom.printMessageAt("        through sprites.",x+10,y+40);

  iom.printMessageAt("Press o to track player", x+10, y+65);
  // iom.printMessageAt("   throgh sprites in rev.", x+10, y+70);

  iom.printMessageAt("Move player with arrow keys", x+10, y+95);

  // IOManager::getInstance().
  //   printMessageCenteredAt("Press shift+t to cycle backward through sprites",
  //       jgdata.getInt("helpLine.y")+15);
}
