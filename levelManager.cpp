#include "levelManager.h"
#include "planets.h"

LevelManager& LevelManager::getInstance(){
  static LevelManager instance;
  return instance;
}

LevelManager::LevelManager():
  jgdata(JSONGamedata::getInstance()),
  levelNames(),
  levelList(),
  planetDataList()
{
  loadFile("levels/defualt.json");
}

int LevelManager::loadFile(const std::string& file) {
  JSONValue *fileData = JSON::Parse(file);

  int retval = 0;

  if(fileData && fileData->HasChild("levels")){
    for(JSONValue* val : fileData->Child("levels")->AsArray()){
      ++retval;
      levelNames.push_back(val->Child("name")->AsString());
      levelList.push_back(val);
      planetDataList.push_back(fileData->Child("Planets"));
    }
  }

  return retval;
}

bool LevelManager::loadLevelNum(int num){
  Planets &planets = Planets::getInstance();
  return planets.loadPlanets(&levelList[num],&planetDataList[num]);
}

std::vector<std::string> LevelManager::getLevelList(){
  return levelNames;
}
