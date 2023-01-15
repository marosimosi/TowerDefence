#ifndef FLOOR_H
#define FLOOR_H

#include <glm/glm.hpp>
#include <common/texture.h>

class Drawable;
class Floor {
public:
	Drawable* floor;
	GLuint diffuseTexture;
	GLuint specularTexture;

	Floor();
	~Floor();
	void draw(unsigned int drawable = 0);
};

class Mountain {
public:
	Drawable* mountain;
	GLuint diffuseTexture;
	GLuint specularTexture;

	Mountain();
	~Mountain();
	void draw(unsigned int drawable = 0);
};

#endif
