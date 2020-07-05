#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <regex>

using namespace std;

namespace Utils {
  
  static vector<pair<string, string>> replace_map = {
    {"<br.*?>", "\n"},
    {"</p>", "\n"},
    {"</div>", "\n"},
    // {"\\\\n", "\n"},
    
    {"<(/?b).*?>", "[$1]"},
    {"<(/?i).*?>", "[$1]"},
    {"<(/?u).*?>", "[$1]"},
    {"<(/?sub).*?>", "[$1]"},
    {"<(/?sup).*?>", "[$1]"},
    {"<(/?ex).*?>", "[$1]"},
    
    {"<(/?)abr.*?>", "[$1p]"},
    {"<(/?)a.*?>", "[$1ref]"},
    {"<(/?)kref.*?>", "[$1ref]"},
    {"<(/?)iref.*?>", "[$1url]"},
    
    {"<c(( ?)c=['\"](.+?)['\"])?>", "[c$2$3]"},
    
    {"<rref>.+?</rref>", ""},
    {"<!--.*?-->", ""},
  };

  static string rx_remove_tag = "<(/?).*?>";
  
  
  static vector<string> tags_literal = {
    "b",
    "i",
    "u",
    "sub",
    "sup",
    "ex"
  };

  static vector<pair<string, string>> tags_change = {
    {"abr", "p"},
    {"a", "ref"},
    {"kref", "ref"},
    {"iref", "url"},
  };

  static vector< pair<string, pair<string, string>> > tags_replace = {
    {"c", {"<c(( ?)c=['\"](.+?)['\"])?>", "[c$2$3]"}},
  };
  
  
  bool findTag(const string& text, const string& tag, bool openTag);
  string getTagName(const string& text);
  
  string readFile(string filename);
  string trim(const string& str);
  string replace_string(const string& str, const string& from_rx, const string& replace);
  
  string html_to_dsl(const string& inText);
  
  string html_to_dsl_v2(const string& text);
  void html_to_dsl_TEST();
  
}

#endif