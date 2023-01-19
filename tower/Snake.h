#ifndef SNAKE_H
#define SNAKE_H

#include <glm/glm.hpp>
#include <common/texture.h>

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
	glm::mat4 modelMatrix;
	bool Run = false;

	Snake();
	~Snake();
	void draw(unsigned int drawable = 0);
	void run(int l);
};

#endif

