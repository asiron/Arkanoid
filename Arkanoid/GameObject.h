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

#define PLAYER 0
#define BLOCK 1
#define EFFECT 2


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
    int boundX;
    int boundY;
    
    //animation properties
    int maxFrame;
    int curFrame;
    int frameCount;
    int frameDelay;
    int frameWidth;
    int frameHeight;
    int animationColumns;
    int animationDirection;
    SDL_Surface* image;
    
public:
    
    
    GameObject() : x(0), y(0), velX(0), velY(0), dirX(0), dirY(0), boundX(0), boundY(0), maxFrame(0),
    curFrame(0), frameCount(0), frameDelay(0), frameWidth(0), frameHeight(0), animationColumns(0),
    animationDirection(0), image(NULL), alive(true), collidable(true) {};
    
    void virtual Destroy();
    
    void Init(float x, float y, float velX, float velY, int dirX, int dirY, int boundX, int boundY);
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
