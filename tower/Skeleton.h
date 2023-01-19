#ifndef SKELETON_H
#define SKELETON_H

#include <glm/glm.hpp>
#include <common/texture.h>

class Drawable;
class Skeleton {
public:
	Drawable* skeleton;
	GLuint diffuseTexture;
	GLuint specularTexture;
	float hp = 3.0;
	float speed = 1.0;
	float damage = 1.5;
	float load_time = 3.0;
	glm::mat4 modelMatrix;
	bool Run = false;

	Skeleton();
	~Skeleton();
	void draw(unsigned int drawable = 0);
	void run(int l);
};

#endif

