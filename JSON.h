#ifndef _JSON_H_
#define _JSON_H_

#include <vector>
#include <string>

#ifdef __clang__
  #if defined(__has_include) && __has_include(<unordered_map>)
// include unordered_map and set macro
    #include <unordered_map>
    #ifndef JSON_UMAP
      #define JSON_UMAP
    #endif
  #else
// include regular map and set macro
    #include <map>
  #endif
  #if defined(__has_include) && __has_include(<utility>)
    #include <utility>
  #else
    #include <algorithm>
  #endif
#else 
  #define GCC_VERSION (__GNUC__ * 10000 \
                        + __GNUC_MINOR__ * 100 \
                        + __GNUC_PATCHLEVEL__)
  #if GCC_VERSION >= 40600
    #include <unordered_map>
    #include <utility>
    #ifndef JSON_UMAP
      #define JSON_UMAP
    #endif
  #else
    #include <map>
    #include <algorithm>
  #endif
#endif

// Simple function to check a string 's' has at least 'n' characters
static inline bool strminlen(const char *s, size_t n) {
  /*if (s == 0)
    return false;

  const char *save = s;
  while (n-- > 0)
    if (*(save++) == 0) return false;

  return true;
  */
  return std::char_traits<char>::length(s) > n;
}

// Custom types
class JSONValue;
typedef std::vector<JSONValue*> JSONArray;
#ifdef JSON_UMAP
  typedef std::unordered_map<std::string, JSONValue*> JSONObject;
#else
  typedef std::map<std::string, JSONValue*> JSONObject;
#endif

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
