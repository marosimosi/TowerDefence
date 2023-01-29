#include "Spider.h"
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
double x;

Spider::Spider() {
    spider = new Drawable("models/spider.obj");
    //diffuseTexture = loadSOIL("spiderdif.bmp");
    //specularTexture = loadSOIL("spiderspec.jpg");
    startModelMatrix = translate(mat4(), vec3(-15, 0, 15)) * scale(mat4(), vec3(0.2, 0.2, 0.2)) * rotate(mat4(), radians(315.0f), vec3(0, 1, 0));
    modelMatrix = startModelMatrix;
    instancing[0] = translate(mat4(), vec3(8, 0, -3));
    instancing[1] = translate(mat4(), vec3(-8, 0, 3));
}

Spider::~Spider() {
    delete spider;
}

void Spider::draw(unsigned int drawable) {
    spider->bind();
    spider->draw(2);
}

float s_angle = 10;
void Spider::run(int loopNum) {
    if (loopNum - runFirstLoop == 20) {
        Run = false;
        Attack = true;
        attackFirstLoop = loopNum;
    }
    if ((loopNum - runFirstLoop) % 3 != 0) { return; }
   
    modelMatrix = translate(mat4(), vec3(speed, 0, -speed)) * modelMatrix *
            rotate(mat4(), radians(s_angle), vec3(1, 0, 1));
    s_angle = -s_angle;
}

void Spider::attack(int loopNum) {
    modelMatrix = rotate(mat4(), radians(15.0f), vec3(0, 1, 0)) * modelMatrix;
    if (loopNum - attackFirstLoop == 60) {
        Attack = false;
        dead = true;
        reviveFirstLoop = loopNum;
    }
}

void Spider::revive(int loopNum) {
    Run = false;
    Attack = false;
    float a = (loopNum - reviveFirstLoop) / load_time;
    modelMatrix = startModelMatrix * scale(mat4(), vec3(a, a, a));
    if (loopNum - reviveFirstLoop == load_time) {
        modelMatrix = startModelMatrix;
        hp = 2.0f;
        dead = false;
    }
 }
    