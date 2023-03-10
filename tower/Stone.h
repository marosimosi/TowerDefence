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
	float hp = 5.0;
	float speed = 3.0;
	float damage = 1.0;
	float load_time = 70.0;
	glm::mat4 startModelMatrix;
	glm::mat4 modelMatrix;
	bool Run = false;
	bool Attack = false;
	bool dead = false;
	int runFirstLoop;
	int attackFirstLoop;
	int reviveFirstLoop;

	Stone();
	~Stone();
	void draw(unsigned int drawable = 0);
	void run(int l);
	void attack(int l);
	void revive(int l);
};

#endif
