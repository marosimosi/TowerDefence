#include "Stone.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <common/texture.h>

using namespace glm;

Stone::Stone(){
    stone = new Drawable("stone.obj");
    diffuseTexture = loadSOIL("diffuso.bmp");
    specularTexture = loadSOIL("rough.bmp");
    float hp = 5.0;
    float speed = 3.0;
    float damage = 7.0;
    float load_time = 5.0;

}

Stone::~Stone() {
    delete stone;
}

void Stone::draw(unsigned int drawable) {
    stone->bind();
    stone->draw();
}
