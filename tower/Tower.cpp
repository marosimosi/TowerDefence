#include "Tower.h"
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



Tower::Tower() {
    tower = new Drawable("tower.obj");
    diffuseTexture = loadSOIL("towerdif.jpg");
    specularTexture = loadSOIL("towerspec.jpg");
    modelMatrix = scale(mat4(), vec3(0.5, 0.6, 0.5));
}

Tower::~Tower() {
    delete tower;
}

void Tower::draw(unsigned int drawable) {
    tower->bind();
    tower->draw();
}
