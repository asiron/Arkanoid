//
//  MapLoader.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/25/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "MapLoader.h"

MapLoader::MapLoader(string filename){
    configfile = new ConfigFile(filename + ".cfg");
        
}


MapLoader::~MapLoader(){
    delete configfile;
}

list<GameObject*> MapLoader::LoadMap(string filename){
    
    list<GameObject*> return_val;
    
    ifstream file;
    file.open(filename.c_str());
    
    string line;
    
    float posY =0;
    while(getline(file, line)){
        
        float posX = 0;
        for(int i=0; i<20; i++){
            
            //if we have an empty symbol we continue in a row and move posX
            if(line[i] == ' ' || !line[i]){
                posX += g_Game.GetScreen_W()/20;
                continue;
            }
            
            Value value = configfile->GetValue_at_Key(line[i]);
            Block* g_object = new Block(value.filename.c_str(), value.maxFrame, value.frameDelay, value.frameWidth, value.frameHeight,
                                             value.animationColumns, value.animationDirection);
            
            g_object->Init(posX+value.frameWidth/2.0, posY+value.frameHeight/2.0, value.speed, value.dirX, value.health);
            
            return_val.push_back(g_object);
            
            posX += g_Game.GetScreen_W()/20; // calculating X coordinate
            
        }
        
        posY += g_Game.GetScreen_H()/20;     // calculating y coordinate
    }
    
    
    return return_val;
}