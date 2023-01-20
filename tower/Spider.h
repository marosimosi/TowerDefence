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
	float hp = 4.0;
	float speed = 1.5;
	float damage = 3.0;
	float load_time = 4.0;
	glm::mat4 startModelMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 instancing[2];
	bool Run = false;
	bool Attack = false;
	bool dead = false;
	int runFirstLoop;
	int attackFirstLoop;

	Spider();
	~Spider();
	void draw(unsigned int drawable = 0);
	void run(int l);
	void attack(int l);
};

#endif
