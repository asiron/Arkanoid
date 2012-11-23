//
//  GameObject.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "GameObject.h"


void GameObject::Destroy() {
    delete animation;
}

void GameObject::Init(float x, float y, float velX, float velY, int dirX, int dirY, float boundX, float boundY){
    
    GameObject::x = x;
    GameObject::y = y;
    GameObject::velY = velY;
    GameObject::velX = velX;
    GameObject::dirX = dirX;
    GameObject::dirY = dirY;
    GameObject::boundX = boundX;
    GameObject::boundY = boundY;
    
    
}
// Assuming no gravity, every object is updated the same way based on its velocity
void GameObject::Update(){
    x += velX * dirX;
    y += velY * dirY;
}

// Every object is rendered in different way
void GameObject::Render(){}

bool GameObject::detectCollision(GameObject* otherObject){
    
    float otherObjectX = otherObject->GetX();
    float otherObjectY = otherObject->GetY();
    
    int otherObjectBoundX = otherObject->GetBoundX();
    int otherObjectBoundY = otherObject->GetBoundY();
    
    if( (x + boundX > otherObjectX - otherObjectBoundX) &&
       ( x - boundX < otherObjectX + otherObjectBoundX) &&
       ( y + boundY > otherObjectY - otherObjectBoundY) &&
       ( y - boundY < otherObjectY + otherObjectBoundY) )
        return true;
    else
        return false;
    
}

//Handle collisions differently for every object
void GameObject::Collided(int objectID){}

bool GameObject::Collidable(){ return alive && collidable ;}
