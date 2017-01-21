#ifndef __INC_PLAY_FILE_H__
#define __INC_PLAY_FILE_H__

#include "includes.h"

using namespace std;
class PlaygroundFile{
public:
    PlaygroundFile(const char* filename);
    PlaygroundFile(ifstream *file);
    void ParseFile(ifstream *file);
    vector<string> *keys,*values,*lines;
    string IdentifyValue(const char* key);
};
    
#endif
