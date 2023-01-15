#ifndef TOWER_H
#define TOWER_H

#include <glm/glm.hpp>
#include <common/texture.h>

class Drawable;
class Tower {
public:
	Drawable* tower;
	GLuint diffuseTexture;
	GLuint specularTexture;

	Tower();
	~Tower();
	void draw(unsigned int drawable = 0);
};

#endif
