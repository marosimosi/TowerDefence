#include "Skeleton.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <common/texture.h>
#include <glfw3.h>

using namespace glm;

Skeleton::Skeleton() {
    skeleton = new Drawable("snake.obj");
    diffuseTexture = loadSOIL("snakedif.bmp");
    specularTexture = loadSOIL("snakespec.bmp");
    modelMatrix = translate(mat4(), vec3(15, -0.5, 15)) * rotate(mat4(), radians(45.0f), vec3(0, 1, 0));

}

Skeleton::~Skeleton() {
    delete skeleton;
}

void Skeleton::draw(unsigned int drawable) {
    skeleton->bind();
    skeleton->draw();
}

float side = 1;
void Skeleton::run(int loopNum) {
    if (loopNum % 10 != 0) { return; }

    modelMatrix = translate(mat4(), vec3(-speed, 0, -speed)) * 
        modelMatrix * scale(mat4(), vec3(side, 1, 1));
    side = -side; 
    
}