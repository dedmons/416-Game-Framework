#ifndef __LEVEL_MANAGER_H
#define __LEVEL_MANAGER_H

#include <string>
#include <vector>

#include "JSONGamedata.h"
#include "JSON.h"

class LevelManager {
public:
  static LevelManager& getInstance();
  int loadFile(const std::string& file);
  bool loadLevelNum(int num);
  bool loadDefaultLevel();
  std::vector<std::string> getLevelList();

private:
  LevelManager();
  LevelManager(LevelManager&) = delete;
  LevelManager& operator=(LevelManager& other);

  const JSONGamedata& jgdata;
  std::vector<std::string> levelNames;
  std::vector<JSONValue> levelList;
  std::vector<JSONValue> planetDataList;
};

#endif
