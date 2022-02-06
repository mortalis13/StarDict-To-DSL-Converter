#include <stdio.h>

#include <iostream>
#include <string>

#include "stardict.h"
#include "utils.h"

using namespace std;

#define VERSION "1.1"

int main(int argc, char * argv []) {
  cout << "StarDict to DSL converter v" << VERSION << endl << endl;
  
  if (argc != 2) {
    cout << "Usage: [EXE] [DICT_PATH].ifo\n";
  }
  else {
    convert_stardict_to_dsl(argv[1]);
  }
  
  return 0;
}
