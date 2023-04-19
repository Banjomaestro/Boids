#include <new>
#include <stdlib.h>
#include <vector>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/trigonometric.hpp"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "Flock.hpp"

// INFO BOIDS

float Deg2Rad(int deg)
{
    return static_cast<float>(deg) * (p6::PI / static_cast<float>(180));
}

int Rad2Deg(float rad)
{
    return static_cast<int>((rad * 180) / p6::PI);
}

// Won't be necessary once in opengl (I think)
glm::vec2 newPosFromAngle(float centerX, float centerY, int angle, float speed)
{
    glm::vec2 newPos;

    newPos.x = (speed * cos(Deg2Rad(angle))) / 2.f + centerX;
    newPos.y = (speed * sin(Deg2Rad(angle))) / 2.f + centerY;

    return newPos;
}

int main(int argc, char* argv[])
{

    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();


    // to adapt with the screen (logically won't be necessary once in opengl)
    float screenWidth = ctx.aspect_ratio();
    Flock theFlock = Flock(screenWidth, screenWidth, screenWidth, 100);


    
    // INIT THE BOIDS

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // unecessary bg, won't keep
          ctx.background(p6::NamedColor::Black);
        ctx.use_stroke = false;
        ctx.fill       = {0.f, 0.f, 0.f, 0.f};
        //ctx.rectangle(p6::FullScreen{});

   
        // boids routine
        for (auto& i : theFlock.getMyBoids())
        {
          
            ctx.fill = {i.getColor(), 0.f, 0.f};
            ctx.square(
                p6::Center{i.getPos().x, i.getPos().y},
                p6::Radius{0.03f}
            );

            // random future direction

         
        }
       theFlock.displayParam() ;
       theFlock.simulate();
         /*

        // Useful for the behaviours' calculations
        int forceAngle = 0;
        int posAngle   = 0;
        int totalAngle = 0;

        // vector to calculate the angle
        glm::vec2 vect0(1, 0);
        for (size_t i = 0; i < randSquare.size(); i++)
        {
            // empty vector, stays empty only if is alone
            glm::vec2 totalForce = glm::vec2();
            // starts with the current angle
            totalAngle = randSquare[i].getAngle();
            // number of boids in the area
            int boidsInDistance = 1;
            // for distance between
            glm::vec2 posInit = glm::vec2(randSquare[i].getX(), randSquare[i].getY());
            // starts with the current pos
            glm::vec2 averagePos = posInit;
            for (size_t j = 0; j < randSquare.size(); j++)
            {
                if (j == i)
                    continue;

                glm::vec2 posFinal = glm::vec2(randSquare[j].getX(), randSquare[j].getY());
                // glm::vec2 temp     = posFinal;

                // Allows the boids connected throughout the window to be considered
                if ((posInit.x < (-1 + distance) * screenWidth) && (posFinal.x > (1 - distance) * screenWidth))
                    posFinal.x -= 2 * screenWidth;
                else if ((posFinal.x < (-1 + distance) * screenWidth) && (posInit.x > (1 - distance) * screenWidth))
                    posFinal.x += 2 * screenWidth;
                // if ((posInit.y < (-1 + distance)) && (posFinal.y > (1 - distance)))
                //     posFinal.y -= 2;
                // else if ((posFinal.y < (-1 + distance)) && (posInit.y > (1 - distance)))
                //     posFinal.y += 2;

                float distanceBetween = abs(glm::distance(posInit, posFinal));
                // the boids in the area
                if (distanceBetween < distance)
                {
                    // HELPER
                    //  if (i == 0)
                    //  {
                    //      ctx.fill = {0.f, 0.f, 0.2f};
                    //      ctx.circle(
                    //          p6::Center{temp.x, temp.y},
                    //          p6::Radius{0.05f}
                    //      );
                    //  }

                    totalForce += (posInit - posFinal);
                    averagePos += posFinal;
                    totalAngle += randSquare[j].getAngle();
                    boidsInDistance++;
                }
            }

            totalAngle /= boidsInDistance;
            averagePos /= boidsInDistance;

            // prevent the angle from being null
            forceAngle = randSquare[i].getAngle();
            posAngle   = randSquare[i].getAngle();

            // if there isn't any boids in the area

            if (totalForce != glm::vec2())
            {
                float newAngle = glm::acos((glm::dot(totalForce, vect0)) / glm::l2Norm(glm::vec3(totalForce, 0)));

                if (totalForce.y <= 0)
                    newAngle = -newAngle;

                forceAngle = Rad2Deg(newAngle);
            }

            if (averagePos != posInit)
            {
                float newAngle = glm::acos((glm::dot(averagePos - posInit, vect0)) / glm::l2Norm(glm::vec3(averagePos - posInit, 0)));

                if (averagePos.y - posInit.y < 0)
                    newAngle = -newAngle;

                posAngle = Rad2Deg(newAngle);
            }

            // if no behaviour the angle does not change
            int finalAngle = randSquare[i].getAngle();

            // Behaviours management (need to be improved)
            if (ALIGNMENT && SEPARATION && COHESION)
            {
                finalAngle = WANDER_STRENGTH * finalAngle + SEPARATION_STRENGTH * forceAngle + COHESION_STRENGTH * posAngle + ALIGNMENT_STRENGTH * totalAngle;
                finalAngle /= WANDER_STRENGTH + SEPARATION_STRENGTH + COHESION_STRENGTH + ALIGNMENT_STRENGTH;
            }
            else if (ALIGNMENT && SEPARATION)
            {
                finalAngle = WANDER_STRENGTH * finalAngle + SEPARATION_STRENGTH * forceAngle + ALIGNMENT_STRENGTH * totalAngle;
                finalAngle /= WANDER_STRENGTH + SEPARATION_STRENGTH + ALIGNMENT_STRENGTH;
            }
            else if (ALIGNMENT && COHESION)
            {
                finalAngle = WANDER_STRENGTH * finalAngle + ALIGNMENT_STRENGTH * totalAngle + COHESION_STRENGTH * posAngle;
                finalAngle /= WANDER_STRENGTH + ALIGNMENT_STRENGTH + COHESION_STRENGTH;
            }
            else if (COHESION && SEPARATION)
            {
                finalAngle = WANDER_STRENGTH * finalAngle + SEPARATION_STRENGTH * forceAngle + COHESION_STRENGTH * posAngle;
                finalAngle /= WANDER_STRENGTH + SEPARATION_STRENGTH + COHESION_STRENGTH;
            }
            else if (SEPARATION)
            {
                finalAngle = WANDER_STRENGTH * finalAngle + SEPARATION_STRENGTH * forceAngle;
                finalAngle /= WANDER_STRENGTH + SEPARATION_STRENGTH;
            }
            else if (ALIGNMENT)
            {
                finalAngle = WANDER_STRENGTH * finalAngle + ALIGNMENT_STRENGTH * totalAngle;
                finalAngle /= WANDER_STRENGTH + ALIGNMENT_STRENGTH;
            }
            else if (COHESION)
            {
                finalAngle = WANDER_STRENGTH * finalAngle + COHESION_STRENGTH * posAngle;
                finalAngle /= WANDER_STRENGTH + COHESION_STRENGTH;
            }

            // avoid the Y borders
            if (randSquare[i].getY() < -1 + border)
                finalAngle = (finalAngle + 90) / 2;
            if (randSquare[i].getY() > 1 - border)
                finalAngle = (finalAngle - 90) / 2;

            randSquare[i].setAngle(finalAngle);

            // HELPER
            //  {
            //      ctx.stroke        = {0.f, 1.f, 0.f};
            //      ctx.stroke_weight = 0.001f;
            //      glm::vec2 newPos  = newPosFromAngle(randSquare[i].getX(), randSquare[i].getY(), randSquare[i].getAngle(), randSquare[i].getSpeed() * 100);
            //      ctx.line(posInit, newPos);
            //  }
        }*/
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
