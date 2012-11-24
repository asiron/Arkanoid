//
//  GameObject.cpp
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#include "GameObject.h"

GameObject::GameObject(const char* filename, int maxFrame, int frameDelay, int frameWidth, int frameHeight, int animationColumns,
    int animationDirection ): x(0), y(0), velX(0), velY(0),dirX(0), dirY(0), boundX(0), boundY(0), alive(false), collidable(true)
{
        //creating Animation object if we have provided a filename
        if(filename)
            animation = new Animation(filename, maxFrame, frameDelay, frameWidth, frameHeight, animationColumns, animationDirection);
}

void GameObject::Destroy() {
    //Destorying GameObject
    if(animation)
        // deleting Animation if it existed
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
    //Simple updating for every GameObject
    x += velX * dirX;
    y += velY * dirY;
}

// Every object is rendered in different way, 
void GameObject::Render(){}

// Collision detection function for every object
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
