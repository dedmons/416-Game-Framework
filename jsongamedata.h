#ifndef JSONGAMEDATA__H
#define JSONGAMEDATA__H
#include <string>
#include <vector>

#include "JSON.h"

using std::string;

class JSONGamedata {
public:
  static JSONGamedata& getInstance();
  ~JSONGamedata();
  void displayData() const;

  bool getBool(const string&) const;
  const string getStr(const string&) const;
  float getFloat(const string&) const;
  int getInt(const string&) const;
  JSONValue* getValue(const string&) const;

  bool hasValue(const string&) const;
private:
  JSONValue *root;

  JSONGamedata(const string& fn = "gameSpecs/game.json");
  JSONGamedata(const JSONGamedata&);
  JSONGamedata& operator=(const JSONGamedata&);
};
#endif
