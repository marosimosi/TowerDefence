#include "Snake.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <common/texture.h>
#include <glfw3.h>

using namespace glm;

Snake::Snake() {
    snake = new Drawable("snake.obj");
    diffuseTexture = loadSOIL("snakedif.bmp");
    specularTexture = loadSOIL("snakespec.bmp");
    modelMatrix = translate(mat4(), vec3(15, -0.5, 15)) * rotate(mat4(), radians(45.0f), vec3(0, 1, 0));

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
    if (loopNum % 10 != 0) { return; }

    modelMatrix = translate(mat4(), vec3(-speed, 0, -speed)) * 
        modelMatrix * scale(mat4(), vec3(side, 1, 1));
    side = -side; 
    
}