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

bool JSONGamedata::getBool(const string& tag) const {
  if(! root->HasChildAtPath(tag)){
    throw std::string("Bad json object path "+tag);
  } else {
    JSONValue *retval = root->ChildAtPath(tag);
    if (retval->IsBool())
      return retval->AsBool();
    else
      throw std::string("Type not bool at path "+tag);
  }
  return false;
}

int JSONGamedata::getInt(const string& tag) const {
  if(! root->HasChildAtPath(tag)){
    throw std::string("Bad json object path "+tag);
  } else {
    JSONValue *retval = root->ChildAtPath(tag);
    if (retval->IsNumber())
      return retval->AsNumber();
    else
      throw std::string("Type not int at path "+tag);
  }
  return -9999;
}

float JSONGamedata::getFloat(const string& tag) const {
  if(! root->HasChildAtPath(tag)){
    throw std::string("Bad json object path "+tag);
  } else {
    JSONValue *retval = root->ChildAtPath(tag);
    if (retval->IsNumber())
      return retval->AsNumber();
    else
      throw std::string("Type not float at path "+tag);
  }
  return -9999.9f;
}

const string JSONGamedata::getStr(const string& tag) const {
  if(! root->HasChildAtPath(tag)){
    throw std::string("Bad json object path "+tag);
  } else {
    JSONValue *retval = root->ChildAtPath(tag);
    if (retval->IsString())
      return retval->AsString();
    else
      throw std::string("Type not string at path "+tag);
  }
  return "";
}

JSONValue* JSONGamedata::getValue(const string& tag) const {
  if(! root->HasChildAtPath(tag)){
    throw std::string("Bad json object path "+tag);
  } else {
    return root->ChildAtPath(tag);
  }
  return NULL;
}

bool JSONGamedata::hasValue(const string& tag) const {
  return root->HasChildAtPath(tag);
}

void JSONGamedata::displayData() const {
  std::cout << root->Stringify() << std::endl;
}
