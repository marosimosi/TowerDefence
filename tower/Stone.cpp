#include "Stone.h"
#include "Box.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <common/texture.h>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Stone::Stone(){
    stone = new Drawable("stone.obj");
    diffuseTexture = loadSOIL("diffuso.bmp");
    specularTexture = loadSOIL("rough.bmp");
    startModelMatrix = translate(mat4(), vec3(15, 0, -15)) * scale(mat4(), vec3(0.5, 0.5, 0.5)) * rotate(mat4(), radians(-45.0f), vec3(0, 1, 0));
    modelMatrix = startModelMatrix;

    /*glm::vec3 min = vec3(100, 100, 100);
    glm::vec3 max = vec3(-100, -100, -100);

    for (unsigned int i = 0; i < 5; i++) {
            if (stone->vertices[i].x < min.x) { min.x = stone->vertices[i].x; }
            if (stone->vertices[i].x > max.x) { max.x = stone->vertices[i].x; }
            printf("%f", stone->vertices[i]);

    }

    box = new Box(min, max);
    printf("%f %f",min, max);*/
}

Stone::~Stone() {
    delete stone;
}

void Stone::draw(unsigned int drawable) {
    stone->bind();
    stone->draw();
}

float st_angle = 40;
void Stone::run(int loopNum) {
    //static int firstLoop = loopNum;
    if (loopNum - runFirstLoop == 75) {
        Run = false;
        Attack = true;
        attackFirstLoop = loopNum;
    }
    if ((loopNum - runFirstLoop) % 20 != 0) { return; }

    modelMatrix = translate(mat4(), vec3(-speed, 0, speed)) * modelMatrix *
        rotate(mat4(), radians(st_angle), vec3(0, 1, 0));
    st_angle = -st_angle;
}

float tilt = 30;
void Stone::attack(int loopNum) {
    //static int firstLoop = loopNum;
    if (loopNum - attackFirstLoop > 55) {
        Attack = false;
        dead = true;
    }
    if ((loopNum - attackFirstLoop) % 10 != 0) { return; }
    modelMatrix = modelMatrix * rotate(mat4(), radians(tilt), vec3(1, 0, 0));
    tilt = -tilt;

    
}