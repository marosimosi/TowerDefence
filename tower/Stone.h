#ifndef STONE_H
#define STONE_H

#include <glm/glm.hpp>
#include <common/texture.h>

class Drawable;
class Stone {
public:
	Drawable* stone;
	GLuint diffuseTexture;
	GLuint specularTexture;
	float hp, speed, damage, load_time;

	Stone();
	~Stone();
	void draw(unsigned int drawable = 0);
};

#endif
