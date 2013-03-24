#ifndef GAMEDATA__H
#define GAMEDATA__H
#include <string>
#include <sstream>
#include <unordered_map>
using std::string;
#include "parseXML.h"

class Gamedata {
public:
  static Gamedata& getInstance();
  void displayData() const;

  bool getXmlBool(const string&) const;
  const string& getXmlStr(const string&) const;
  float getXmlFloat(const string&) const;
  int getXmlInt(const string&) const;
private:
  ParseXML parser;
  const unordered_map<string, string> gameData;

  Gamedata(const string& fn = "xmlSpec/game.xml");
  Gamedata(const Gamedata&);
  Gamedata& operator=(const Gamedata&);
};
#endif
