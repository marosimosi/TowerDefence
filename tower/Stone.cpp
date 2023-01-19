#include "Stone.h"
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
    modelMatrix = translate(mat4(), vec3(15, 0, -15)) * scale(mat4(), vec3(0.5, 0.5, 0.5)) * rotate(mat4(), radians(-45.0f), vec3(0, 1, 0));
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
    if (loopNum % 20 != 0) { return; }

    modelMatrix = translate(mat4(), vec3(-speed, 0, speed)) * modelMatrix *
        rotate(mat4(), radians(st_angle), vec3(0, 1, 0));
    st_angle = -st_angle;
}