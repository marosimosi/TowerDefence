#ifndef SNAKE_H
#define SNAKE_H

#include <glm/glm.hpp>
#include <common/texture.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <glfw3.h>

class Drawable;
class Snake {
public:
	Drawable* snake;
	GLuint diffuseTexture;
	GLuint specularTexture;
	float hp = 3.0;
	float speed = 1.0;
	float damage = 1.5;
	float load_time = 3.0;
	glm::mat4 startModelMatrix;
	glm::mat4 modelMatrix;
	bool Run = false;
	bool Attack = false;
	bool dead = false;
	int runFirstLoop;
	int attackFirstLoop;

	Snake();
	~Snake();
	void draw(unsigned int drawable = 0);
	void run(int l);
	void attack(int l);
};

#endif

