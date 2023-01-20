#ifndef TOWER_H
#define TOWER_H

#include <glm/glm.hpp>
#include <common/texture.h>
#include "Box.h"


class Drawable;
class Tower {
public:
	Drawable* tower;
	GLuint diffuseTexture;
	GLuint specularTexture;
	glm::mat4 modelMatrix;

	Tower();
	~Tower();
	void draw(unsigned int drawable = 0);
	void snakeAttack(int l, int fl);
	void stoneAttack(int l);
};

#endif
