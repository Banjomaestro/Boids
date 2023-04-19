#include "Tools.hpp"


bool getPercentage(float percentage){
    return getRandomNumber(0.f, 100.f)<percentage;
}

float getRandomNumber(float min, float max){

    return p6::random::number(min, max);
    
}