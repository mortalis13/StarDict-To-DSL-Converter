#include "stardict.h"

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <winsock.h>
// #include <netinet/in.h>

#include <string>
#include <algorithm>

#include "utils.h"

using namespace std;


static string getFileName(const string& s) {
  char sep = '/';
#ifdef _WIN32
  sep = '\\';
#endif

  size_t i = s.rfind(sep, s.length());
  if (i != string::npos) {
    return(s.substr(i+1, s.length() - i));
  }
  return("");
}

string getDictName(const char* ifoFileName) {
  string dictName = "";
  
  char *buffer;
  FILE * pFile = fopen(ifoFileName, "r");
  fseek(pFile, 0, SEEK_END);
  long lSize = ftell(pFile);
  rewind(pFile);

  buffer = (char*) malloc(sizeof(char) *lSize);
  size_t result = fread(buffer, 1, lSize, pFile);
  fclose(pFile);
  
  string bufStr(buffer, lSize);
  free(buffer);
  string namePref = "bookname=";
  int pos1 = bufStr.find(namePref);
  
  if (pos1 != string::npos) {
    int pos2 = bufStr.find("\n", pos1 + namePref.size());
    pos1 = pos1 + namePref.size();
    dictName = bufStr.substr(pos1, pos2-pos1);
  }
  
  return dictName;
}

void convert_stardict_to_dsl(const char* ifoFileName) {
  string dictName = getDictName(ifoFileName);
  
  string idxFileName = ifoFileName;
  idxFileName.replace(idxFileName.length()-sizeof("ifo")+1, sizeof("ifo")-1, "idx");
  struct stat idx_stats;
  if (stat(idxFileName.c_str(), &idx_stats) == -1) {
    printf("File does not exist: '%s'\n", idxFileName.c_str());
    return;
  }
  
  string dictFileName = ifoFileName;
  dictFileName.replace(dictFileName.length()-sizeof("ifo")+1, sizeof("ifo")-1, "dict");
  struct stat dict_stats;
  
  if (stat(dictFileName.c_str(), &dict_stats) == -1) {
    printf("File not exist: '%s'\n", dictFileName.c_str());
    return;
  }
  
  char *idxbuffer = (char *) malloc(idx_stats.st_size);
  char *idxbuffer_end = idxbuffer + idx_stats.st_size;
  FILE *idxFile;
  idxFile = fopen(idxFileName.c_str(), "rb");
  fread(idxbuffer, 1, idx_stats.st_size, idxFile);
  fclose(idxFile);
  
  char *dictbuffer = (char *) malloc(dict_stats.st_size);
  FILE *dictFile;
  dictFile = fopen(dictFileName.c_str(), "rb");
  fread(dictbuffer, 1, dict_stats.st_size, dictFile);
  fclose(dictFile);
  
  
  // === WRITE
  string outFileName = Utils::replace_string(ifoFileName, "\\.(ifo|IFO)$", "") + ".dsl";
  outFileName.replace(outFileName.length()-sizeof("ifo")+1, sizeof("ifo")-1, "dsl");
  printf("Write to file: '%s'\n", outFileName.c_str());
  
  FILE *txtFile = fopen(outFileName.c_str(), "w");
  char *p = idxbuffer;
  int wordlen;
  unsigned int offset, size;
  char *data;
  
  string dictSourceLang = "English";
  string dictTargetLang = "English";
  string headerStr = "#NAME \"" + dictName + "\"\n#INDEX_LANGUAGE \"" + dictSourceLang + "\"\n#CONTENTS_LANGUAGE \"" + dictTargetLang + "\"\n\n";
  fwrite(headerStr.c_str(), 1, headerStr.size(), txtFile);
  
  while (true) {
    if (p == idxbuffer_end) break;

    wordlen = strlen(p);
    fwrite(p, wordlen, 1, txtFile);
    fwrite("\n", 1, 1, txtFile);
    p += wordlen + 1;
    
    offset = *reinterpret_cast<unsigned int *>(p);
    offset = ntohl(offset);
    p += sizeof(unsigned int);
    size = *reinterpret_cast<unsigned int *>(p);
    size = ntohl(size);
    p += sizeof(unsigned int);
    data = dictbuffer + offset;
    
    string dataStr(data, size);
    dataStr = Utils::html_to_dsl(dataStr);
    
    fwrite("  ", 1, 2, txtFile);
    fwrite(dataStr.c_str(), 1, dataStr.size(), txtFile);
    fwrite("\n\n", 1, 2, txtFile);
  }
  
  fclose(txtFile);
  free(idxbuffer);
  free(dictbuffer);
}
