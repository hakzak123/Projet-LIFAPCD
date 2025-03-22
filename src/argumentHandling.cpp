#include <string.h>
#include <string>
#include <iostream>
#include <application.h>

int argumentsParserFind(int argc, char* argv[], const char* searched){
    for(int i=0; i < argc ; ++i){
        if(strcmp(argv[i],searched) == 0){
            return i;
        }
    }
    return -1;
}

void SMM::argumentHandling(SMM* app, int argc, char* argv[]){
    int c = argumentsParserFind(argc,argv,"-maxfps");

    try{
        if(c>=0){
            if((app->maxframerate = std::stoi(argv[c+1])) <=0)
                throw std::runtime_error("Invalid maxfps argument\n");
        }
    }
    catch(std::exception& e){
        std::cout << e.what();
        std::exit(1);
    }
}