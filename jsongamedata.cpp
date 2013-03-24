#include "jsongamedata.h"

#include <sstream>

JSONGamedata::~JSONGamedata(){
  delete root;
}

JSONGamedata& JSONGamedata::getInstance() {
  static JSONGamedata instance;
  return instance;
}

JSONGamedata::JSONGamedata(const string& fn ) : root(JSON::Parse(fn)) {}

std::vector<string> JSONGamedata::getPathValues(const string& path) const {
  // std::cout << "Extracting object path: " << path << std::endl;
  std::istringstream f(path);
  std::string s;
  std::vector<string> retval;
  while (std::getline(f,s,'.')){
    retval.push_back(s);
  }
  return retval;
}

bool JSONGamedata::getBool(const string& tag) const {
  JSONValue *retval = root;
  std::vector<string> comp = getPathValues(tag);

  for (std::vector<string>::iterator it = comp.begin(); it != comp.end(); ++it){
    if(retval->HasChild(*it)){
      retval = retval->Child(*it);
    } else {
      throw std::string("Bad json object path"+tag);
    }
  }
  if (retval->IsBool())
    return retval->AsBool();
  
  return false;
}

int JSONGamedata::getInt(const string& tag) const {
  JSONValue *retval = root;
  std::vector<string> comp = getPathValues(tag);

  for (std::vector<string>::iterator it = comp.begin(); it != comp.end(); ++it){
    if(retval->HasChild(*it)){
      retval = retval->Child(*it);
    } else {
      throw std::string("Bad json object path"+tag);
    }
  }
  if (retval->IsNumber()) 
    return static_cast<int>(retval->AsNumber());
  
  return -9999;
}

float JSONGamedata::getFloat(const string& tag) const {
  JSONValue *retval = root;
  std::vector<string> comp = getPathValues(tag);

  for (std::vector<string>::iterator it = comp.begin(); it != comp.end(); ++it){
    if(retval->HasChild(*it)){
      retval = retval->Child(*it);
    } else {
      throw std::string("Bad json object path"+tag);
    }
  }
  if (retval->IsNumber()) 
    return static_cast<float>(retval->AsNumber());
  
  return -9999.9f;
}

const string JSONGamedata::getStr(const string& tag) const {
  JSONValue *retval = root;
  std::vector<string> comp = getPathValues(tag);

  for (std::vector<string>::iterator it = comp.begin(); it != comp.end(); ++it){
    if(retval->HasChild(*it)){
      retval = retval->Child(*it);
    } else {
      throw std::string("Bad json object path"+tag);
    }
  }
  if (retval->IsString()) 
    return retval->AsString();
  
  return "";
}

void JSONGamedata::displayData() const {
  std::cout << root->Stringify() << std::endl;
}