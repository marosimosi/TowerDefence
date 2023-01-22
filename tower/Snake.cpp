#include "Snake.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <common/texture.h>
#include <glfw3.h>

using namespace glm;

Snake::Snake() {
    snake = new Drawable("models/snake.obj");
    //diffuseTexture = loadSOIL("snakedif.bmp");
    //specularTexture = loadSOIL("snakespec.bmp");
    startModelMatrix = translate(mat4(), vec3(15, -0.5, 15)) * rotate(mat4(), radians(45.0f), vec3(0, 1, 0));
    modelMatrix = startModelMatrix;
}

Snake::~Snake() {
    delete snake;
}

void Snake::draw(unsigned int drawable) {
    snake->bind();
    snake->draw();
}

float side = 1;
void Snake::run(int loopNum) {
    if (loopNum - runFirstLoop == 120) {
        Run = false;
        Attack = true;
        attackFirstLoop = loopNum;
    }
    if ((loopNum - runFirstLoop) % 10 != 0) { return; }

    modelMatrix = translate(mat4(), vec3(-speed, 0, -speed)) * 
        modelMatrix * scale(mat4(), vec3(side, 1, 1));
    side = -side; 
    
}

float spin = 0.1f;
void Snake::attack(int loopNum) {
    modelMatrix =  translate(mat4(), vec3(spin, -0.05, -spin)) * modelMatrix;
    spin = -spin;
    if (loopNum - attackFirstLoop > 35) {
        Attack = false;
        dead = true;
    }
}