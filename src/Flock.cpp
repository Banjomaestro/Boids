#include "Flock.hpp"
#include "Tools.hpp"
#include <cmath>

Flock::Flock(float maxX, float maxY, float maxZ, int numberOfBoids): maxX(maxX), maxY(maxY), maxZ(maxZ), numberOfBoids(numberOfBoids){

    for(int i=0; i<numberOfBoids; i++){
        glm::vec3 RandCoord = glm::vec3(getRandomNumber(-1, 1),
                                        getRandomNumber(-1, 1),
                                        getRandomNumber(-1, 1));
        Boid toAdd = Boid(RandCoord.x * maxX, RandCoord.y * maxY, RandCoord.z * maxZ,1.f,maxX,maxY, maxZ);
        myBoids.push_back(toAdd);
    }

}

void Flock :: limitVelocity (Boid& boid){

    if(glm::length(boid.getVelocity())>speedLimit){
        boid.setVelocity((boid.getVelocity()/glm::length(boid.getVelocity()))*speedLimit);
    }
}

glm::vec3 Flock::separation(Boid& boid) {

    glm::vec3 avoidVector = glm::vec3(0,0,0);

    for(Boid& currentBoid : myBoids){

            if(&currentBoid != &boid && glm::distance(currentBoid.getPos(),boid.getPos())<separationDistance){

                avoidVector -= (currentBoid.getPos() - boid.getPos());
            }
    }
    return avoidVector;
    
}

glm::vec3 Flock :: cohesion(Boid& boid){

    glm::vec3 centerOfMass = glm::vec3(0,0,0);
    
        for(Boid& currentBoid : myBoids){

            if(&currentBoid != &boid && glm::distance(currentBoid.getPos(),boid.getPos())<cohesionDistance){

                centerOfMass+=currentBoid.getPos();
            }

    }

    return (centerOfMass/static_cast<float>(myBoids.size()))/100.0f;
}

glm::vec3 Flock :: alignment(Boid& boid){

    glm::vec3 velocityAverage = glm::vec3(0,0,0);
    
        for(Boid& currentBoid : myBoids){

            if(&currentBoid != &boid && glm::distance(currentBoid.getPos(),boid.getPos())<distanceToFollow){

                velocityAverage+=currentBoid.getVelocity();
            }

    }

    return (velocityAverage/static_cast<float>(myBoids.size()))/8.0f;
}

glm::vec3 Flock :: boundaries(Boid& boid){

    glm::vec3 v;
    if(boid.getPos().x < boundaryVecMin.x){
        v.x = BoundaryWallValue;
    }else if(boid.getPos().x > boundaryVecMax.x){
        v.x = -BoundaryWallValue;
    }
    if(boid.getPos().y < boundaryVecMin.y){
        v.y = BoundaryWallValue;
    }else if(boid.getPos().y > boundaryVecMax.y){
        v.y = -BoundaryWallValue;
    }
    if(boid.getPos().z < boundaryVecMin.z){
        v.z = BoundaryWallValue;
    }else if(boid.getPos().z > boundaryVecMax.z){
        v.z = -BoundaryWallValue;
    }
    return v;
}

void Flock :: simulate(){

    if(!isPaused){

        for(Boid& currentBoid : myBoids){

            glm::vec3 cohesionValue = cohesionCoeff*cohesion(currentBoid);
            glm::vec3 separationValue = separationCoeff*separation(currentBoid);
            glm::vec3 alignmentValue = alignmentCoeff*alignment(currentBoid);
            glm::vec3 boundariesValue = boundaryCoeff*boundaries(currentBoid);

            currentBoid.setVelocity(currentBoid.getVelocity()+boundariesValue+separationValue+alignmentValue+cohesionValue);
            limitVelocity(currentBoid);
            currentBoid.setPos(currentBoid.getPos()+currentBoid.getVelocity()) ;
        }

        if(getPercentage(0.5f)){
            cohesionCoeff = -cohesionCoeff;
            std::cout<<"inversion lululullululu";
        }
    }
}

void Flock::displayParam(){
    ImGui::Begin("Flock settings");
    ImGui::Text("maxSpeed");
    ImGui::SliderFloat("Max speed", &speedLimit, .0001f, 0.1f);
    ImGui::SliderFloat("Distance see for alignment", &distanceToFollow, .01f, 1);
    ImGui::SliderFloat("Distance see for cohesion", &cohesionDistance, .01f, 1);
    ImGui::SliderFloat("Distance see for separation", &separationDistance, .01f, 1);

    ImGui::SliderFloat("coefficient for alignment force", &alignmentCoeff, .1f, 10);
    ImGui::SliderFloat("coefficient for cohesion force", &cohesionCoeff, .1f, 10);
    ImGui::SliderFloat("coefficient for separation force", &separationCoeff, .1f, 10);
    ImGui::SliderFloat("coefficient for boundary force", &boundaryCoeff, .1f, 10);
    ImGui::Checkbox("Pause", &isPaused);

    ImGui::End();
}