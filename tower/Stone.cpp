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
    float hp = 5.0;
    float speed = 3.0;
    float damage = 7.0;
    float load_time = 5.0;
    modelMatrix = translate(mat4(), vec3(12, 0, 0)) * scale(mat4(), vec3(0.5, 0.5, 0.5));

}

Stone::~Stone() {
    delete stone;
}

void Stone::draw(unsigned int drawable) {
    stone->bind();
    stone->draw();
}
