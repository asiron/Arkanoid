//
//  GameObject.h
//  Arkanoid
//
//  Created by Maciej Żurad on 11/18/12.
//  Copyright (c) 2012 Maciej Żurad. All rights reserved.
//

#ifndef __Arkanoid__GameObject__
#define __Arkanoid__GameObject__

#include <iostream>
#include <SDL/SDL.h>
#include "Animation.h"

#define PLAYER 0
#define BALL 1
#define BLOCK 2
#define EFFECT 3

class Animation;

class GameObject {
    
private:
    bool alive;
    bool collidable;
    int ID;
    
protected:
    // object's properties for moving and collision
    float x;
    float y;
    float velX;
    float velY;
    
    int dirX;
    int dirY;
    float boundX;
    float boundY;
    
    Animation* animation;
    
public:
    
    
    GameObject(SDL_Surface *image = NULL ) : x(0), y(0), velX(0), velY(0),
                dirX(0), dirY(0), boundX(0), boundY(0), alive(false), collidable(true) {
        animation = new Animation(image);
    };
    
    void virtual Destroy();
    
    void Init(float x, float y, float velX, float velY, int dirX, int dirY, float boundX, float boundY);
    void virtual Render();
    void virtual Update();
    
    float GetX() { return x; }
    float GetY() { return y; }
    
    void SetX(float x) { GameObject::x = x; }
    void SetY(float y) { GameObject::y = y; }
    
    float GetBoundX() { return boundX; }
    float GetBoundY() { return boundY; }
    
    int GetID() { return ID;}
    void SetID(int ID) {GameObject::ID = ID;}
    
    int isAlive() { return alive ; }
    void SetAlive(bool alive) {GameObject::alive = alive;}
    
    int isCollidable() { return collidable; }
    void setCollidavle(bool collidable) {GameObject::collidable = collidable;}
    
    bool detectCollision(GameObject* otherObject);
    void virtual Collided(int objectID);
    bool Collidable();
};

#endif /* defined(__Arkanoid__GameObject__) */
