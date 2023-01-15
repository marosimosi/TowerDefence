#include "Spider.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <common/texture.h>

using namespace glm;

Spider::Spider() {
    spider = new Drawable("spider.obj");
    diffuseTexture = loadSOIL("spiderdif.bmp");
    specularTexture = loadSOIL("spiderspec.jpg");
    float hp = 4.0;
    float speed = 4.0;
    float damage = 3.0;
    float load_time = 4.0;
}

Spider::~Spider() {
    delete spider;
}

void Spider::draw(unsigned int drawable) {
    spider->bind();
    spider->draw(2);
}