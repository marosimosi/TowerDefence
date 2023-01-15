#include "Tower.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <common/texture.h>

using namespace glm;

Tower::Tower() {
    tower = new Drawable("tower.obj");
    diffuseTexture = loadSOIL("towerdif.jpg");
    specularTexture = loadSOIL("towerspec.png");
}

Tower::~Tower() {
    delete tower;
}

void Tower::draw(unsigned int drawable) {
    tower->bind();
    tower->draw();
}