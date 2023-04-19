#pragma once
#include "p6/p6.h"
#include <stdlib.h>
#include <cmath>
#include "Tools.hpp"
#include <glm/glm.hpp>


class Boid {
private:
    glm::vec3 position;
    float color = 1.f;
    float maxX;
    float maxY;
    float maxZ;
    float speed = 0.002f;
    glm::vec3 velocity;

public:
    Boid();

    Boid(float x, float y, float z, float color, float maxX, float maxY, float maxZ);

    glm::vec3 getPos() const { return position;}

    void setPos(glm::vec3 newPos);

    glm::vec3 getVelocity() const { return velocity;}

    void setVelocity(float xSpeed, float ySpeed, float zSpeed){velocity=glm::vec3(xSpeed,ySpeed,zSpeed);}

    void setVelocity(glm::vec3 newVel){velocity=newVel;}

    float getSpeed() const { return speed; }

    float getColor() const { return color; }

    float distanceWithBoid(Boid toCheck)  const ;


};

