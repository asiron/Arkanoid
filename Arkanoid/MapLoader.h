//
//  MapLoader.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/25/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__MapLoader__
#define __Arkanoid__MapLoader__

#include <iostream>
#include <list>
#include "GameObject.h"
#include "ConfigFile.h"

using namespace std;

class MapLoader {

private:
    
    const char* filename;
    ConfigFile *configfile;
public:
    MapLoader();
    ~MapLoader();

    list<GameObject*> LoadMap(const char* filename);
    
};

#endif /* defined(__Arkanoid__MapLoader__) */
