#pragma once
#include "Boid.hpp"
#include <vector>
#include "p6/p6.h"
#include "Tools.hpp"
#include <glm/glm.hpp>
#include "imgui.h"


class Flock{

    public:
        Flock(float maxX, float maxY, float maxZ, int numberOfBoids);
        std::vector<Boid> getMyBoids() const { return myBoids; }
        glm::vec3 alignment(Boid& boid);
        void simulate();
        glm::vec3 separation(Boid& boid);
        glm::vec3 cohesion(Boid& boid);
        glm::vec3 boundaries(Boid& boid);
        void limitVelocity (Boid& boid);
        void displayParam();



    private:
        float maxX;
        float maxY;
        float maxZ;
        float distanceToFollow = 0.01f;
        float cohesionDistance = 0.01f;
        const int numberOfBoids;
        float separationDistance = 0.01f;
        float speedLimit = 0.003f;
        glm::vec3 boundaryVecMax = glm::vec3(2.f,2.f,2.f);
        glm::vec3 boundaryVecMin = glm::vec3(-2.f,-2.f,-2.f);
        float BoundaryWallValue = 0.001f;
        bool isPaused = false;
        float cohesionCoeff = 1.f;
        float separationCoeff = 1.0f;
        float alignmentCoeff = 1.0f;
        float boundaryCoeff = 1.0f;

        std::vector<Boid> myBoids;

};