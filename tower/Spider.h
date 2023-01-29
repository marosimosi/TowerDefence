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
	float hp = 2.0;
	float speed = 1.5;
	float damage = 0.2;
	float load_time = 50.0f;
	glm::mat4 startModelMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 instancing[2];
	bool Run = false;
	bool Attack = false;
	bool dead = false;
	int runFirstLoop;
	int attackFirstLoop;
	int reviveFirstLoop;

	Spider();
	~Spider();
	void draw(unsigned int drawable = 0);
	void run(int l);
	void attack(int l);
	void revive(int l);
};

#endif
