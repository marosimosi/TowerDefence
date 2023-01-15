#include "Dragon.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <common/texture.h>

using namespace glm;

Dragon::Dragon() {
    dragon = new Drawable("dragon.obj");
    diffuseTexture = loadSOIL("dragondif.bmp");
    specularTexture = loadSOIL("dragonspec.bmp");
    float hp = 2.0;
    float speed = 5.0;
    float damage = 9.0;
    float load_time = 6.0;
}

Dragon::~Dragon() {
    delete dragon;
}

void Dragon::draw(unsigned int drawable) {
    dragon->bind();
    dragon->draw();
}