#include "playground_parser.h"

using namespace std;
PlaygroundFile::PlaygroundFile(const char* filename){
    ifstream file(filename);
    ParseFile(&file);
}

PlaygroundFile::PlaygroundFile(ifstream *file){
    ParseFile(file);
}

void PlaygroundFile::ParseFile(ifstream *file){
    //ifstream file=*file_ptr;
    string line;
    vector<string> *splitline= new vector<string>();
    keys=new vector<string>();
    values=new vector<string>();
    lines=new vector<string>();
    while (getline(*file,line)){
        splitline->clear();
        SplitString(line,'=',splitline);
        if(splitline->size()<2){
            //If this isn't a key pair add a line anyway to keep keys,values and lines in sync
            keys->push_back("");
            values->push_back("");
        }else{
            keys->push_back((*splitline)[0]);
            values->push_back((*splitline)[1]);
        }
        lines->push_back(line);
    }
    delete splitline;
}
        
using namespace std;

string PlaygroundFile::IdentifyValue(const char* key){
    int size=keys->size();
    for(int i=0;i<size;i++){
        if ((*keys)[i].compare(key)==0){
            return (*values)[i];
        }
    }
    return "";
}

