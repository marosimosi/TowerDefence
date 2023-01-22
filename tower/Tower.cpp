#include "Tower.h"
#include "Box.h"
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
    tower = new Drawable("models/tower.obj");
    //diffuse = loadSOIL("towerdif.jpg");
    diffuseTexture = diffuse;
    //specularTexture = loadSOIL("towerspec.jpg");
    //webTexture = loadSOIL("web2.jpg");
    modelMatrix = scale(mat4(), vec3(0.5, 0.6, 0.5));
}

Tower::~Tower() {
    delete tower;
}

void Tower::draw(unsigned int drawable) {
    tower->bind();
    tower->draw();
}

void Tower::snakeAttack(int loopNum, int firstLoop) {
    if (loopNum - firstLoop == 20) {
        modelMatrix = translate(mat4(), vec3(0, 0, 0.5)) * modelMatrix;
    }

    if (loopNum - firstLoop > 20) {
        modelMatrix = rotate(mat4(), radians(45.0f), vec3(0, 1, 0)) * modelMatrix;
    }
    if (loopNum - firstLoop == 36) {
        modelMatrix = translate(mat4(), vec3(0, 0, -0.5)) * modelMatrix;
    }
}

float shake = 0.2f;
void Tower::stoneAttack(int loopNum) {
     modelMatrix = translate(mat4(), vec3(-shake, 0, shake)) * modelMatrix;
     shake = -shake;

}

void Tower::spiderAttack(int loopNum, int firstLoop) {
    if (loopNum - firstLoop == 20) {
        diffuseTexture = webTexture;
    }
    if (loopNum - firstLoop == 60) {
        diffuseTexture = diffuse;
    }
}
