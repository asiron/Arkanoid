//
//  State.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/26/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "State.h"

#include "defines.h"

#include <fstream>
#include <sstream>
#include <algorithm>

State::State(){
    ifstream file;

    file.open(std::string(RESOURCE_DIRECTORY) + "/highscores");
    string line;
    while(getline(file, line)){
        
        size_t it = 0; // character pointer

#ifdef __GNUC__
        line.erase( std::find( line.begin(), line.end(), ' ' ), line.end() );                          //removing whitespaces
#else
        line.erase( remove( line.begin(), line.end(), ' ' ), line.end() );                          //removing whitespaces
#endif
        string name = line.substr(0, it=line.find_first_of(","));                                   //getting players name
        line.erase(0, it+1);
        
        int score = StrToInt(line);
        highsco_list.push_back(pair<string, int>(name, score));
    }
    file.close();
}


int StrToInt(string s){
    istringstream istr(s);
    int returnVal;
    istr >> returnVal;
    
    return returnVal;
}

bool Compare(pair<string, int> a, pair<string, int> b){
    return a > b ;
}

void State::PushScore(string name, int highscore){
    highsco_list.push_back(pair<string, int>(name, highscore));
    highsco_list.sort(Compare);
}
