#ifndef SPIDER_H
#define SPIDER_H

#include <glm/glm.hpp>
#include <common/texture.h>

class Drawable;
class Spider {
public:
	Drawable* spider;
	GLuint diffuseTexture;
	GLuint specularTexture;
	float hp, speed, damage, load_time;

	Spider();
	~Spider();
	void draw(unsigned int drawable = 0);
};

#endif
