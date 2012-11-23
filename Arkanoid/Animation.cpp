//
//  Animation.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/22/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "Animation.h"

Animation::~Animation(){
    if(!image)
        delete image;
}