//
//  ConfigFile.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/25/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "ConfigFile.h"

#include <typeinfo>
#include <algorithm>

ConfigFile::ConfigFile(string filename){
    ConfigFile::filename = filename;
    Parse();
}


template <typename T>
T ConfigFile::String_to_T(string const &val){
    
    std::istringstream istr(val);
    T returnVal;
    
    if (!(istr >> returnVal))        
        ExitWithError("ConfigFile: Not a valid " + (std::string)typeid(T).name() + " received!\n");
        
        return returnVal;
}

void ConfigFile::ExitWithError(const string &error){
    
	cerr << error;
	cin.ignore();
	cin.get();
    
	exit(EXIT_FAILURE);
}



void ConfigFile::RemoveComment(string &line) const{
    if(line.find(";") != line.npos)
        line.erase(line.find(";"));
}

bool ConfigFile::HasOnlyWhiteSpace(string &line) const{
    return (line.find_first_not_of(' ') == line.npos);
}

bool ConfigFile::IsValidLine(string &line) const{
    
    // find first '='
  char target[] = {'='};
  std::string::iterator start_position = line.end();
  start_position = std::find_first_of(line.begin(), line.end(), target, target + 1);
  if (start_position == line.end())
    return false;

    //count commas
    std::string::iterator::difference_type count = 0;
    count = std::count(start_position + 1, line.end(), ',');

    return (count == 9);
}

bool ConfigFile::KeyExists(char key) const{
    return content.find(key) != content.end();
}

Value ConfigFile::GetValue_at_Key(char key){
    return content.find(key)->second;
}


void ConfigFile::ExtractKey(char &key, string &line) const{
    key = line[0];
    line.erase(0,2);
}

void ConfigFile::ExtractValue(Value &value, string &line) const{
    
    size_t it = 0;
    
    value.filename = line.substr(0, it=line.find_first_of(','));
    line.erase(0, it+1);
    value.maxFrame = String_to_T<int>(line.substr(0, it=line.find_first_of(',')));
    line.erase(0, it+1);
    value.frameDelay = String_to_T<int>(line.substr(0, it=line.find_first_of(',')));
    line.erase(0, it+1);
    value.frameWidth = String_to_T<int>(line.substr(0, it=line.find_first_of(',')));
    line.erase(0, it+1);
    value.frameHeight = String_to_T<int>(line.substr(0, it=line.find_first_of(',')));
    line.erase(0, it+1);
    value.animationColumns = String_to_T<int>(line.substr(0, it=line.find_first_of(',')));
    line.erase(0, it+1);
    value.animationDirection = String_to_T<int>(line.substr(0, it=line.find_first_of(',')));
    line.erase(0, it+1);
    value.speed = String_to_T<int>(line.substr(0, it=line.find_first_of(',')));
    line.erase(0, it+1);
    value.dirX = String_to_T<int>(line.substr(0, it=line.find_first_of(',')));
    line.erase(0, it+1);
    value.health = String_to_T<int>(line.substr(0, it=line.find_first_of(',')));
    
}

struct is_whitespace_t
 : public std::unary_function<char, bool>
{
    bool operator() (char character_in)
    {
      char target[] = {' ', '\t', 0};

      return (std::find(target, target + 2, character_in) != 0);
    }
};

void ConfigFile::ExtractContents(string &line, size_t line_number){

  // remove any whitespace
//  // *TODO*: find a solution using function binding...
//  is_whitespace_t predicate;
//  std::remove_if(line.begin(), line.end(), predicate);
  line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
  std::cerr << line;

    char key;
    Value value;
    
    ExtractKey(key, line);
    ExtractValue(value, line);
    
    if(!KeyExists(key))
        content.insert(pair<char, Value>(key, value));
    else
        ExitWithError("ConfigFile: Only unique keys are allowed at " + line_number);
    
}

void ConfigFile::Parse(){
    ifstream file;
    file.open(filename.c_str());
    
    if(!file)
        ExitWithError("ConfigFile: File" + filename + "couldn't be found!\n");
    
    string line ;
    size_t line_number = 0;
    while(getline(file, line)){
        ++line_number;

      // support comments
        RemoveComment(line);
        if(HasOnlyWhiteSpace(line)) // empty line so we continue
            continue;

        // validate schema
        if(!IsValidLine(line))
            ExitWithError("ConfigFile: Found invalid line at " + line_number);

        // parse line
        ExtractContents(line, line_number);
    }

    file.close();
}
