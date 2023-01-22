#include "Dragon.h"
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

Dragon::Dragon() {
    dragon = new Drawable("models/dragon.obj");
    //diffuseTexture = loadSOIL("dragondif.bmp");
    //specularTexture = loadSOIL("dragonspec.bmp");
    float hp = 2.0;
    float speed = 5.0;
    float damage = 9.0;
    float load_time = 6.0;
    modelMatrix = translate(mat4(), vec3(-15, 10, -15)) * scale(mat4(), vec3(0.07, 0.1, 0.1)) * rotate(mat4(), radians(45.0f), vec3(0,1,0));
}

Dragon::~Dragon() {
    delete dragon;
}

void Dragon::draw(unsigned int drawable) {
    dragon->bind();
    dragon->draw();
}