#include "Skeleton.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <common/texture.h>

using namespace glm;

Skeleton::Skeleton() {
    skeleton = new Drawable("skeleton.obj");
    float hp = 3.0;
    float speed = 5.0;
    float damage = 1.5;
    float load_time = 3.0;
}

Skeleton::~Skeleton() {
    delete skeleton;
}

void Skeleton::draw(unsigned int drawable) {
    skeleton->bind();
    skeleton->draw(3);
}