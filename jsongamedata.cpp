#include "jsongamedata.h"

#include <sstream>
#include <iostream>

JSONGamedata::~JSONGamedata(){
  delete root;
}

JSONGamedata& JSONGamedata::getInstance() {
  static JSONGamedata instance;
  return instance;
}

JSONGamedata::JSONGamedata(const string& fn ) : root(JSON::Parse(fn)) {}

std::vector<string> JSONGamedata::getPathValues(const string& path) const {
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
      throw std::string("Bad json object path "+tag);
    }
  }
  if (retval->IsBool())
    return retval->AsBool();
  else
    throw std::string("Type not bool at path "+tag);

  return false;
}

int JSONGamedata::getInt(const string& tag) const {
  JSONValue *retval = root;
  std::vector<string> comp = getPathValues(tag);

  for (std::vector<string>::iterator it = comp.begin(); it != comp.end(); ++it){
    if(retval->HasChild(*it)){
      retval = retval->Child(*it);
    } else {
      throw std::string("Bad json object path "+tag);
    }
  }
  if (retval->IsNumber())
    return static_cast<int>(retval->AsNumber());
  else
    throw std::string("Type not int at paht "+tag);

  return -9999;
}

float JSONGamedata::getFloat(const string& tag) const {
  JSONValue *retval = root;
  std::vector<string> comp = getPathValues(tag);

  for (std::vector<string>::iterator it = comp.begin(); it != comp.end(); ++it){
    if(retval->HasChild(*it)){
      retval = retval->Child(*it);
    } else {
      throw std::string("Bad json object path "+tag);
    }
  }
  if (retval->IsNumber())
    return static_cast<float>(retval->AsNumber());
  else
    throw std::string("Type not float at path "+tag);

  return -9999.9f;
}

const string JSONGamedata::getStr(const string& tag) const {
  JSONValue *retval = root;
  std::vector<string> comp = getPathValues(tag);

  for (std::vector<string>::iterator it = comp.begin(); it != comp.end(); ++it){
    if(retval->HasChild(*it)){
      retval = retval->Child(*it);
    } else {
      throw std::string("Bad json object path "+tag);
    }
  }
  if (retval->IsString())
    return retval->AsString();
  else
    throw std::string("Type not string at path "+tag);

  return "";
}

JSONValue* JSONGamedata::getValue(const string& tag) const {
  JSONValue *retval = root;
  std::vector<string> comp = getPathValues(tag);

  for (std::vector<string>::iterator it = comp.begin(); it != comp.end(); ++it){
    if(retval->HasChild(*it)){
      retval = retval->Child(*it);
    } else {
      throw std::string("Bad json object path "+tag);
    }
  }

  return retval;
}


void JSONGamedata::displayData() const {
  std::cout << root->Stringify() << std::endl;
}
