#ifndef _JSON_H_
#define _JSON_H_

#include <vector>
#include <string>
#include <unordered_map>

// Simple function to check a string 's' has at least 'n' characters
static inline bool strminlen(const char *s, size_t n) {
  if (s == 0)
    return false;

  const char *save = s;
  while (n-- > 0)
  {
    if (*(save++) == 0) return false;
  }

  return true;
}

// Custom types
class JSONValue;
typedef std::vector<JSONValue*> JSONArray;
typedef std::unordered_map<std::string, JSONValue*> JSONObject;

#include "JSONValue.h"

class JSON
{
  friend class JSONValue;

public:
  static JSONValue* Parse(const std::string &file);
  static JSONValue* Parse(const char *data);
  static std::string Stringify(const JSONValue *value);
protected:
  static bool SkipWhitespace(const char **data);
  static bool ExtractString(const char **data, std::string &str);
  static double ParseInt(const char **data);
  static double ParseDecimal(const char **data);
private:
  JSON();
};

#endif
