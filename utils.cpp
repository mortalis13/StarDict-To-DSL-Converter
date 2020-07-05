#include "utils.h"

namespace Utils {
  
  bool findTag(const string& text, const string& tag, bool openTag) {
    size_t start_pos = 1;
    if (!openTag) start_pos = 2;
    
    bool result = false;
    if (start_pos < text.size() && text.substr(start_pos, tag.size()) == tag) {
      if (start_pos + tag.size() < text.size()) {
        string last_char = text.substr(start_pos+tag.size(), 1);
        result = (last_char == " " || last_char == ">");
      }
    }
    
    return result;
  }

  string getTagName(const string& text) {
    string tagName = "";
    size_t tagNameStart = text.find_first_not_of("< /");
    size_t tagNameEnd = text.find_first_of(" >", tagNameStart);
    
    if (tagNameStart != string::npos) {
      tagName = text.substr(tagNameStart, tagNameEnd - tagNameStart);
    }
    
    return tagName;
  }
  
  string readFile(string filename) {
    string result = "";
    
    fstream f(filename, ios::in | ios::ate);
    if (f.is_open()) {
      // cout << "File opened" << endl;
      size_t size = f.tellg();
      char buf[size];
      f.seekg(0, ios::beg);
      f.read(buf, size);
      f.close();
      result = string(buf, size);
    }
    else {
      cout << "Error opening file: " << filename << endl;
    }
    
    return result;
  }

  string trim(const string& str) {
    string chars = "\t\n\v\f\r ";
    size_t first = str.find_first_not_of(chars);
    if (string::npos == first) return str;
    size_t last = str.find_last_not_of(chars);
    return str.substr(first, (last - first + 1));
  }
  
  string replace_string(const string& str, const string& from_rx, const string& replace) {
    regex rx(from_rx);
    string res = regex_replace(str, rx, replace);
    return res;
  }
  
  
  // ========================================
  
  string html_to_dsl(const string& inText) {
    string tag;
    string tempTag;
    string text;
    
    bool tagStarted = false;
    bool newLineInserted = false;
    
    size_t len = inText.size();
    for (size_t strPos = 0; strPos < len; strPos++) {
      char c = inText[strPos];
      
      if (tagStarted) {
        tag.push_back(c);
        
        if (c == '>') {
          tagStarted = false;
          bool isOpenTag = (tag[1] != '/');
          string close_ch = isOpenTag ? "" : "/";
          
          bool tagProcessed = false;
          if (isOpenTag) {
            // cout << "OPEN: " << tag << endl;
            tempTag = tag;
            
            if (findTag(tag, "br", isOpenTag)) {
              text += "\n  ";
              newLineInserted = true;
              tagProcessed = true;
            }
          }
          else {
            // cout << "CLOSE: " << tag << endl;
            tempTag.clear();
            
            if (findTag(tag, "p", isOpenTag) || findTag(tag, "div", isOpenTag)) {
              text += "\n  ";
              newLineInserted = true;
              tagProcessed = true;
            }
          }
          
          if (!tagProcessed) {
            for (auto item: tags_literal) {
              if (findTag(tag, item, isOpenTag)) {
                text += "[" + close_ch + item + "]";
                tagProcessed = true;
                break;
              }
            }
          }
          
          if (!tagProcessed) {
            for (auto item: tags_change) {
              if (findTag(tag, item.first, isOpenTag)) {
                text += "[" + close_ch + item.second + "]";
                tagProcessed = true;
                break;
              }
            }
          }
          
          if (!tagProcessed) {
            for (auto item: tags_replace) {
              if (findTag(tag, item.first, isOpenTag)) {
                if (isOpenTag) text += regex_replace(tag, regex(item.second.first), item.second.second);
                else text += "[/" + item.first + "]";
                break;
              }
            }
          }
          
          tag.clear();
        }
      }
      else if (c == '<') {
        tagStarted = true;
        tag.push_back(c);
      }
      else if (c == '[' || c == ']') {
        text.push_back('\\');
        text.push_back(c);
      }
      else if (c == '\n') {
        if (newLineInserted) continue;
        if (c == '\n') text += "\n  ";
        newLineInserted = false;
      }
      else {
        if (c == 0x00) continue;
        if (tempTag.size() && findTag(tempTag, "rref", true)) continue;
        
        text.push_back(c);
      }
    }
    
    return text;
  }
  
  
  string html_to_dsl_v2(const string& text) {
    string html = text;
    for (auto item: replace_map) {
      html = regex_replace(html, regex(item.first), item.second);
    }
    html = regex_replace(html, regex(rx_remove_tag), "");
    html = regex_replace(html, regex("\n"), "\n\t");
    return html;
  }
  
  void html_to_dsl_TEST() {
    string filename = "data/test_body.txt";
    string html = readFile(filename);
    // html = trim(html);
    // html = "<k>abalienato</k>\ntransfer of property, alienation";
    if (html.size() == 0) return;
    cout << endl << endl << html << "\n\n===========\n\n";
    
    for (auto item: replace_map) {
      // cout << ".. replacing: " << item.first << endl;
      html = regex_replace(html, regex(item.first), item.second);
    }
    html = regex_replace(html, regex(rx_remove_tag), "");
    
    cout << html << endl;
  }
  
}
