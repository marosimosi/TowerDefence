#ifndef DRAGON_H
#define DRAGON_H

#include <glm/glm.hpp>
#include <common/texture.h>

class Drawable;
class Dragon {
public:
	Drawable* dragon;
	GLuint diffuseTexture;
	GLuint specularTexture;
	float hp, speed, damage, load_time;

	Dragon();
	~Dragon();
	void draw(unsigned int drawable = 0);
};

#endif

