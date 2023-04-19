#include "Boid.hpp"
#include "Tools.hpp"


Boid :: Boid(float x, float y, float z,  float color, float maxX, float maxY, float maxZ) : color(color), maxX(maxX), maxY(maxY), maxZ(maxZ) {
    position = glm::vec3(x,y,z);
    velocity = glm::vec3(getRandomNumber(0, 0.01f)-0.005f,getRandomNumber(0, 0.01f)-0.005f,getRandomNumber(0, 0.01f)-0.005f);

}

void Boid::setPos(glm::vec3 newPos)
    {

    if(newPos.x > maxX  ){

        newPos.x = -maxX;

    }else if(newPos.x < -maxX){

        newPos.x = maxX;
    }

    if(newPos.y > maxY){

        newPos.y = -maxY;

    }else if(newPos.y< -maxY){

        newPos.y = maxY;

    }if(newPos.z > maxZ){

        newPos.z = -maxZ;

    }else if(newPos.z< -maxZ){

        newPos.z = maxZ;

    }

    position = newPos;
}